#!/usr/bin/env python

'''
Author Eugene Mamin(mailto: thedzhon@gmail.com)

Auxiliary script for distance estimation

Script prerequisites:
- python 3.7+
- pyproj 3.0+

'''

import math

from pyproj import Geod

latB=47.176928
lonB=39.425619
altB=120.000000

latC1=47.179694
lonC1=39.421134
altC1=5.000000

latC2=47.180747
lonC2=39.424546
altC2=7.000000

g = Geod(ellps='WGS84')

azC1,_,projdC1 = g.inv(lonC1,latC1,lonB,latB)
azC2,_,projdC2 = g.inv(lonC2,latC2,lonB,latB)

if(azC1<0):
    azC1 = azC1 + 360.
if(azC2<0):
    azC2 = azC2 + 360.

hC1 = (altB-altC1)
hC2 = (altB-altC2)

dC1 = (projdC1**2 + hC1**2) ** 0.5
dC2 = (projdC2**2 + hC2**2) ** 0.5

elC1 = math.atan(hC1 / projdC1) * 180. / math.pi  
elC2 = math.atan(hC2 / projdC2) * 180. / math.pi

info_str  = "Camera #{c}: distance is {d:.2f} meters, " \
	"azimuth {az:.2f} degrees, " \
        "elevation {el:.2f} degrees"

print (info_str.format(c=1,d=dC1,az=azC1,el=elC1))
print (info_str.format(c=2,d=dC2,az=azC2,el=elC2))