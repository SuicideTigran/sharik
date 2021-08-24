#include "ECEF.h"

double n(double x) {
    double res;
    res = kA / sqrt(1.0 - kE2 * pow(sin(x * kPi180), 2));
    return res;
}

CoordECEF geotoecef(CoordGeo point) {
    CoordECEF new_point;
    new_point.x = (n(point.lat) + point.h) * cos(point.lat * kPi180) * cos(point.lon * kPi180);
    new_point.y = (n(point.lat) + point.h) * cos(point.lat * kPi180) * sin(point.lon * kPi180);
    new_point.z = (n(point.lat) * (1.0 - kE2) + point.h) * sin(point.lat * kPi180);
    return new_point;
}

CoordGeo eceftogeo(CoordECEF point)
{
    CoordGeo new_point;
    double a2 = a * a, b2 = b * b, x = point.x, y = point.y, z = point.z;
    double xy = sqrt(x * x + y * y);
    double thet = atan(z * a / (xy * b));
    double esq = 1.0 - b2 / a2;
    double epsq = a2 / b2 - 1.0;
    new_point.lat = (atan((z + epsq * b * (sin(thet) * sin(thet) * sin(thet))) / (xy - esq * a * (cos(thet) * cos(thet) * cos(thet)))));
    new_point.lon = (atan2(y, x));
    if (new_point.lon < 0) new_point.lon = 2 * M_PI + new_point.lon;
    double r = a2 / sqrt(a2 * cos(new_point.lat) * cos(new_point.lat) + b2 * sin(new_point.lat) * sin(new_point.lat));
    new_point.h = xy / cos(new_point.lat) - r;
    new_point.lat *= pog;
    new_point.lon *= pog;
    return new_point;
}