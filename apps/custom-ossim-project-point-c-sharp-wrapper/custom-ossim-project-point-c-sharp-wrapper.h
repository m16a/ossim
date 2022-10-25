#ifndef CUSTOM_OSSIM_PROJECT_POINT_C_SHARP_WRAPPER_HEADER
#define CUSTOM_OSSIM_PROJECT_POINT_C_SHARP_WRAPPER_HEADER 1

#ifdef __cplusplus
extern "C" {
#endif
	extern __declspec(dllexport) const char* Run(
		const char* settings_file_name,
		double latitude,
		double longitude,
		double elevation,
		double roll,
		double pitch,
		double heading,
		double px,
		double py,
		double focal_length,
		double pix_size_x,
		double pix_size_y,
		int img_width,
		int img_height,
		double u,
		double v);
#ifdef __cplusplus
}
#endif

#endif //CUSTOM_OSSIM_PROJECT_POINT_C_SHARP_WRAPPER_HEADER 