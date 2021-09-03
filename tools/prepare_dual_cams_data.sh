#!/bin/bash

#
# Author Eugene Mamin(mailto: thedzhon@gmail.com)
#
# Test data writer for the 'balloon' task.
#
# Provides two files: cam1.csv and cam2.csv by calling gen_single_camera_data.py script
#
# Script prerequisites:
# - GNU bash
# - python 3.7+
# - pyproj 3.0+
#

### BALLON OUTPUT PARAMETERS

# Balloon 3D coordinates (degrees x degrees x meters)
B_LAT=47.177
B_LON=39.426
B_ALT=120.000000

# Real balloon size, meters
RB_W=1.0
RB_H=0.8



### Camera common parameters

# Camera field of view, degrees
CFOV_W=1.125
CFOV_H=1.0

# Camera sensor resolution, pixels
CRES_W=1280
CRES_H=720



### First and second camera geometry

# First camera params (azimuth, elevation, distance from balloon, x and y pixel position on screen, outpuf csv filename)
C1_AZ=132
C1_ELEV=10
C1_D=480
C1_X=440
C1_Y=540
F1="cam1.csv"

# Second camera params (azimuth, elevation, distance from balloon, x and y pixel position on screen, outpuf csv filename)
C2_AZ=150
C2_ELEV=15
C2_D=450
C2_X=820
C2_Y=510
F2="cam2.csv"

chmod +x ./gen_single_camera_data.py

rungen="./gen_single_camera_data.py --rb_w=$RB_W --rb_h=$RB_H \
	--b_alt=$B_ALT --b_lon=$B_LON --b_lat=$B_LAT \
	--cfov_h=$CFOV_H --cfov_w=$CFOV_W --cres_h=$CRES_H --cres_w=$CRES_W"

echo "Writing first camera data > $F1"
$rungen --caz=$C1_AZ --celev=$C1_ELEV --cdist=$C1_D --b_x=$C1_X --b_y=$C1_Y --outfile=$F1

echo "Writing second camera data > $F2"
$rungen --caz=$C2_AZ --celev=$C2_ELEV --cdist=$C2_D --b_x=$C2_X --b_y=$C2_Y --outfile=$F2

echo "Done"	
