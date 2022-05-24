#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/init/ossimInit.h>
#include <ossim/projection/ossimApplanixEcefModel.h>
#include <ossim/util/ossimInfo.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
   double u;
   double v;
};

Params parse_params_from_string(const std::string& params_string) {
   Params result;
   std::istringstream iss{params_string};
   iss >> result.latitude;
   iss >> result.longitude;
   iss >> result.elevation;
   iss >> result.roll;
   iss >> result.pitch;
   iss >> result.heading;
   iss >> result.px;
   iss >> result.py;
   iss >> result.focal_length;
   iss >> result.pix_size_x;
   iss >> result.pix_size_y;
   iss >> result.img_width;
   iss >> result.img_height;
   iss >> result.u;
   iss >> result.v;
   return result;
}

ossimGpt run_from_params(const Params& params, bool debug=false) {
   if (debug) {
      std::cout << "Parsed input params: " << std::endl;
      std::cout << "  latitude (deg): " << params.latitude << std::endl;
      std::cout << "  longitude (deg): " << params.longitude << std::endl;
      std::cout << "  height above ground (meters): " << params.elevation << std::endl;
      std::cout << "  roll (deg): " << params.roll << std::endl;
      std::cout << "  pitch (deg): " << params.pitch << std::endl;
      std::cout << "  yaw (deg): " << params.heading << std::endl;
      std::cout << "  principal point offset from center x (mm): " << params.px << std::endl;
      std::cout << "  principal point offset from center y (mm): " << params.py << std::endl;
      std::cout << "  focal length (mm): " << params.focal_length << std::endl;
      std::cout << "  pixel size x (mm): " << params.pix_size_x << std::endl;
      std::cout << "  pixel size y (mm): " << params.pix_size_y << std::endl;
      std::cout << "  image width (pix): " << params.img_width << std::endl;
      std::cout << "  image height (pix): " << params.img_height << std::endl;
      std::cout << "  Selected pixel x (pix): " << params.u << std::endl;
      std::cout << "  Selected pixel y (pix): " << params.v << std::endl;
   }
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
   return res;
}

void run_from_tasks_file_or_str_input(const std::string& tasks_fpath_or_str_inp) {
   std::ifstream inps{tasks_fpath_or_str_inp, std::ifstream::in};
   if (inps.good()) {
      // read from tasks file
      std::string line;
      while (std::getline(inps, line)) {
         if (line.rfind("//", 0) == 0) {
            continue;
         }
         const Params& params = parse_params_from_string(line);
         const ossimGpt& result_coord = run_from_params(params);
         std::cout << "Result coordinate: " << result_coord << std::endl;
      }
   } else {
      // read from str input
      const Params& params = parse_params_from_string(tasks_fpath_or_str_inp);
      const ossimGpt& result_coord = run_from_params(params);
      std::cout << "Result coordinate: " << result_coord << std::endl;
   }
}

int main(int argc, char *argv[])
{
   int originalArgCount = argc;

   ossimArgumentParser ap(&argc, argv);

   // Initialize ossim stuff, factories, plugin, etc.
   ossimInit::instance()->initialize(ap);

   try
   {
      run_from_tasks_file_or_str_input(argv[argc - 1]);
   }
   catch (const ossimException& e)
   {
      ossimNotify(ossimNotifyLevel_WARN) << e.what() << std::endl;
      return 1;
   }
      
   
   return 0;
   
} // End of main...
