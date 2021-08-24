#include <iostream>
#include "ECEF.h"
#include "matrices.h"
#include "triangulation.h"

using namespace std;

int main()
{
    //47.179694;39.421134;5.000000; (3355010.822458939161, 2757910.081720423885, 4655369.837082945742) 130.97;14.10;1920;1080;1440;540;23;9;5.00;4.00
    //47.180747; 39.424546; 7.000000; (3354781.301657155622, 2758056.201765720267, 4655450.873677616008) 169.19; 15.65; 1920; 1080; 960; 810; 24; 10; 5.00; 4.00

    CoordGeo coordFirstCam, coordSecondCam;
    coordFirstCam.lat = 47.179694; coordFirstCam.lon = 39.421134; coordFirstCam.h = 5.000000;
    coordSecondCam.lat = 47.180747; coordSecondCam.lon = 39.424546; coordSecondCam.h = 7.000000;
    CoordECEF ecefFirstCam = geotoecef(coordFirstCam), ecefSecondCam = geotoecef(coordSecondCam);

    vector<double> firstCam ={ ecefFirstCam.x, ecefFirstCam.y, ecefFirstCam.z, 1440, 540,  130.97, 14.10 },
                   secondCam = { ecefSecondCam.x, ecefSecondCam.y, ecefSecondCam.z, 960, 810, 169.19, 15.65 };
    
    Triangulation example(firstCam, secondCam);
    vector<double> exRes = example.triangulation();
    
    CoordECEF ecefResult;
    ecefResult.x = exRes[0]; ecefResult.y = exRes[1]; ecefResult.z = exRes[2];
    CoordGeo coordResult = eceftogeo(ecefResult);
    //cout << exRes[0] << "  " << exRes[1] << "  " << exRes[2] << endl;
    cout << coordResult.lat << "  " << coordResult.lon << "  " << coordResult.h << endl;

    return 0;
}