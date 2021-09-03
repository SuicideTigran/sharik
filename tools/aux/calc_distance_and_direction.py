#!/usr/bin/env python3

'''
Author Eugene Mamin(mailto: thedzhon@gmail.com)

Auxiliary script for distance estimation

Script prerequisites:
- python 3.7+
- pyproj 3.0+

'''

# Data block [edit]

latB=47.176928
lonB=39.425619
altB=120.000000

latC1=47.179694
lonC1=39.421134
altC1=5.000000

latC2=47.180747
lonC2=39.424546
altC2=7.000000

# Actual script

import math

from pyproj import Geod
from dataclasses import dataclass

@dataclass
class DistanceAndDirection:
    distance: float
    azimuth: float
    elevation: float

g = Geod(ellps='WGS84')

def handle_camera(lat: float, lon: float, alt: float) -> DistanceAndDirection:
    azC,_,projdC = g.inv(lon,lat,lonB,latB)
    if(azC<0):
        azC = azC + 360.
    
    hC = (altB-alt)
    dC = (projdC**2 + hC**2) ** 0.5

    elC = math.atan(hC / projdC) * 180. / math.pi
    
    return DistanceAndDirection(dC,azC,elC) 

c1Data = handle_camera(latC1,lonC1,altC1)
c2Data = handle_camera(latC2,lonC2,altC2)

info_str  = "Camera #{c} -> balloon: {d:.2f} meters, " \
	"azimuth {az:.2f} deg, " \
        "elevation {el:.2f} deg"

print (info_str.format(c=1,d=c1Data.distance,az=c1Data.azimuth,el=c1Data.elevation))
print (info_str.format(c=2,d=c2Data.distance,az=c2Data.azimuth,el=c2Data.elevation))