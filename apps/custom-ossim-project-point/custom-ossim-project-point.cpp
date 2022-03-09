#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/init/ossimInit.h>
#include <ossim/projection/ossimApplanixEcefModel.h>
#include <ossim/util/ossimInfo.h>

#include <fstream>
#include <iostream>

using namespace std;

struct Params {
   double latitude;
   double longitude;
   double elevation;
   double roll;
   double pitch;
   double heading;
   double px;
   double py;
   double focal_length;
   double pix_size_x;
   double pix_size_y;
   int img_width;
   int img_height;
   int u;
   int v;
};

Params parse_params() {
   std::ifstream inps{"params.txt", std::ifstream::in};
   Params result;
   inps >> result.latitude;
   inps >> result.longitude;
   inps >> result.elevation;
   inps >> result.roll;
   inps >> result.pitch;
   inps >> result.heading;
   inps >> result.px;
   inps >> result.py;
   inps >> result.focal_length;
   inps >> result.pix_size_x;
   inps >> result.pix_size_y;
   inps >> result.img_width;
   inps >> result.img_height;
   inps >> result.u;
   inps >> result.v;
   return result;
}

int main(int argc, char *argv[])
{
   //---
   // Get the arg count so we can tell if an arg was consumed by
   // ossimInit::instance()->initialize
   //---
   int originalArgCount = argc;

   ossimArgumentParser ap(&argc, argv);

   // Initialize ossim stuff, factories, plugin, etc.
   ossimInit::instance()->initialize(ap);

   try
   {
      Params params = parse_params();
      ossimApplanixEcefModel model{
         ossimDrect{0, 0, params.img_width, params.img_height},
	 ossimGpt{params.latitude, params.longitude, params.elevation},
	 params.roll,
	 params.pitch,
	 params.heading,
	 ossimDpt{params.px, params.py},
	 params.focal_length,
	 ossimDpt{params.pix_size_x, params.pix_size_y}
      };
      model.setPrincipalPoint(ossimDpt{params.px, params.py});
      ossimGpt res;
      model.lineSampleToWorld(ossimDpt{params.u, params.v}, res);
      std::cout << "Res lat, long, height: " << res.latd() << ", " << res.lond() << ", " <<
         res.height() << std::endl;
   }
   catch (const ossimException& e)
   {
      ossimNotify(ossimNotifyLevel_WARN) << e.what() << std::endl;
      return 1;
   }
      
   
   return 0;
   
} // End of main...
