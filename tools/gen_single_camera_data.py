#!/usr/bin/env python3

'''
Author Eugene Mamin(mailto: thedzhon@gmail.com)

Test data generator for the 'balloon' task.

Script prerequisites:
- python 3.7+
- pyproj 3.0+

Input data:
    - balloon 2D dimensions (width x height)
    - balloon 3D geolocation (latitude, longitude, altitude)
    - balloon 2D pixel size for camera (width x height)
    - camera parameters (resolution, field of view)
    - camera 2D direction (azimuth and elevation)

Estimation data:
    - camera 3D geolocation (latitude, longitude, altitude)

Output:
    - csv file with header and single data line for the 'sharik' application.

'''

import argparse
import sys
import math
import csv

from dataclasses import dataclass
from pyproj import Geod
from typing import Final

@dataclass
class SizeF2D:
    width: float
    height: float

@dataclass
class GeoCoordinate:
    latitude: float
    longitude: float
    altitude: float

@dataclass
class Size2D:
    width: int
    height: int

@dataclass
class Direction:
    azimuth: float
    elevation: float

def parse_gen_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="'Balloon' params generator")
    
    parser.add_argument('--rb_w', default=1.0, type=float, help='real balloon witdth, meters (default: %(default)s)')
    parser.add_argument('--rb_h', default=1.0, type=float, help='real balloon height, meters (default: %(default)s)')

    parser.add_argument('--b_alt', default=100., type=float, help='balloon altiitude, meters (default: %(default)s)')
    parser.add_argument('--b_lat', required=True, type=float, help='balloon latitude, degrees')
    parser.add_argument('--b_lon', required=True, type=float, help='balloon longitude, degrees')
    parser.add_argument('--b_x', required=True, type=int, help='balloon x coordinate on screen, degrees')
    parser.add_argument('--b_y', required=True, type=int, help='balloon y coordinate on screen, degrees')

    parser.add_argument('--cres_w', default=720, type=int, help='camera resolution (width), pixels (default: %(default)s)')
    parser.add_argument('--cres_h', default=576, type=int, help='camera resolution (height), pixels (default: %(default)s)')
    parser.add_argument('--cfov_w', default=10., type=float, help='camera field of view (width), degrees (default: %(default)s)')
    parser.add_argument('--cfov_h', default=8., type=float, help='camera field of view (height), degrees (default: %(default)s)')
    parser.add_argument('--caz', default=45., type=float, help='camera azimuth, degrees (default: %(default)s)')
    parser.add_argument('--celev', default=30., type=float, help='camera elevation, degrees (default: %(default)s)')
    parser.add_argument('--cdist', default=500., type=float, help='camera distance, meters (default: %(default)s)')

    parser.add_argument('--outfile', required=True, help='Output CSV file')

    return parser.parse_args()


def calculate_px_size(real_size: SizeF2D, res: Size2D, fov: SizeF2D, d: float) -> Size2D:
    angle_w = math.atan ((real_size.width / 2.) / d) * 180. / math.pi * 2
    angle_h = math.atan ((real_size.height / 2.) / d) * 180. / math.pi * 2

    frac_w = angle_w / fov.width
    frac_h = angle_h / fov.height
  
    px_w = round(frac_w * res.width)
    px_h = round(frac_h * res.height)

    return Size2D(px_w, px_h)

def refine_direction(cdir: Direction, res: Size2D, fov: SizeF2D, xy: Size2D) -> Direction:
    dw = fov.width / float(res.width)
    dh = fov.height / float(res.height)
    diff_w = xy.width - (res.width / 2)
    diff_h = (res.height / 2) - xy.height
    return Direction(cdir.azimuth + diff_w * dw, cdir.elevation + diff_h * dh)

def calculate_camera_position(b_position: GeoCoordinate, cdir: Direction, d: float) -> GeoCoordinate:
    MAX_ITERATIONS: Final = 100
    EPSILON: Final = 1e-9
    
    g = Geod(ellps='WGS84')
    
    h = d * math.sin(cdir.elevation * math.pi / 180.)
    d_proj = (d*d - h*h) ** 0.5 
    
    rev_az = cdir.azimuth - 180.
    if rev_az < 0:
        rev_az = 360. + rev_az
       
    for i in range(MAX_ITERATIONS):
        lon,lat,backaz = g.fwd(b_position.longitude, b_position.latitude, rev_az, d_proj)
        if backaz < 0:
            backaz = 360. + backaz
        
        diff = cdir.azimuth - backaz
        if abs(diff) < EPSILON:
            break
        
        rev_az = rev_az + diff
        if rev_az < 0:
            rev_az = 360. + rev_az 
        if rev_az > 360.:
            rev_az = rev_az - 360.      
                    
    return GeoCoordinate(lat, lon, b_position.altitude - h)
    
def write_csv(d,fname):
    with open(fname, 'w') as f:
        w = csv.DictWriter(f, d.keys(), delimiter=';')
        w.writeheader()
        w.writerow(d)

if __name__ == "__main__":
    args = parse_gen_args()

    real_size = SizeF2D(args.rb_w, args.rb_h)
    distance = args.cdist
    fov = SizeF2D(args.cfov_w, args.cfov_h)
    res = Size2D(args.cres_w, args.cres_h)

    pix_size = calculate_px_size(real_size, res, fov, distance)

    b_position = GeoCoordinate(args.b_lat, args.b_lon, args.b_alt)
    cdir = Direction(args.caz, args.celev)
    xy = Size2D(args.b_x, args.b_y)
    cdir_refined = refine_direction(cdir, res, fov, xy)

    camera_position = calculate_camera_position(b_position, cdir_refined, distance)

    d = dict(LATITUDE_DEG=camera_position.latitude, LONGITUDE_DEG=camera_position.longitude, ALTITUDE_M=camera_position.altitude, \
    AZIMUTH_DEG=cdir.azimuth, ELEVATION_DEG=cdir.elevation, \
    RESOLUTION_W=res.width, RESOLUTION_H=res.height, \
    POSITION_X=xy.width, POSITION_Y=xy.height, \
    SIZE_W=pix_size.width,SIZE_H=pix_size.height, \
    FOW_W=fov.width, FOW_H=fov.height) 

    write_csv(d, args.outfile)

    
