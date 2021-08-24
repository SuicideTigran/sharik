#pragma once
#define _USE_MATH_DEFINES
#include <vector>
#include <math.h>

const double a = 6378137, b = 6356752.314, pog = 57.295754957;
const double kPi = M_PI;
const double kPi180 = kPi / 180;
const double kA = 6378137.0;
const double k1F = 298.257223563;
const double kB = kA * (1.0 - 1.0 / k1F);
const double kE2 = (1.0 / k1F) * (2.0 - (1.0 / k1F));
const double kEd2 = kE2 * kA * kA / (kB * kB);

struct CoordECEF {
    double x;
    double y;
    double z;
};

struct CoordGeo {
    double lat;
    double lon;
    double h;
};

double n(double x);

CoordECEF geotoecef(CoordGeo point);

CoordGeo eceftogeo(CoordECEF point);

