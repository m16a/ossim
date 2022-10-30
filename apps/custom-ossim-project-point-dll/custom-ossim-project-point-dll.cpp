#include "custom-ossim-project-point-dll.h"

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

int projectPoint(
	const char* settings_file_name,
	double latitude, double longitude, double elevation,
	double roll, double pitch, double heading,
	double px, double py, double focal_length,
	double pix_size_x, double pix_size_y,
	int img_width, int img_height, double u, double v,
	double& out_lattitude, double& out_longitude, double& out_altitude)
{
	out_lattitude = 0.0;
	out_longitude = 0.0;
	out_altitude = 0.0;
	try {
		std::string cmdLine = "nonused_app_name -P " + std::string(settings_file_name);
		ossimArgumentParser ap(cmdLine);
		// Initialize ossim stuff, factories, plugin, etc.
		ossimInit::instance()->initialize(ap);

		ossimApplanixEcefModel model{
		   ossimDrect{0, 0, (double)img_width, (double)img_height},
		   ossimGpt{latitude, longitude, elevation},
		   roll,
		   pitch,
		   heading,
		   ossimDpt{px, py},
		   focal_length,
		   ossimDpt{pix_size_x, pix_size_y}
		};
		model.setPrincipalPoint(ossimDpt{ px, py });
		ossimGpt res;
		model.lineSampleToWorld(ossimDpt{ u, v }, res);
		
		out_lattitude = res.latd();
		out_longitude = res.lond();
		out_altitude = res.height();
	}
	catch (const ossimException& e) {
		return 1;
	}
	return 0;
}
