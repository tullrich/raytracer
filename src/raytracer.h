#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include <string>
#include "rtimage.h"

namespace raytracer {

using std::string;
using namespace raytracer::image;

/* Default input values */
#define DEFAULT_IMG_WIDTH 100	// default output image height value
#define DEFAULT_IMG_HEIGHT 100	// default output image height value
#define DEFAULT_OUT_PATH "traceimage.png"	// default output image filepath

#define CONFIG_FILEPATH "raytracer.cfg" // input configuration filepath

#define OPTION_WIDTH "w"
#define OPTION_HEIGHT "h"
#define OPTION_OUTPATH "output-filepath"


/**
 * raytracer root
 */
class Raytracer 
{
public:
	Raytracer(const string &outputfile, int height, int width);
	~Raytracer() {};

	string& getOutpath() { return outpath; };
	void setOutpath(const string &newpath) {  outpath = newpath; };

	/**
	 * run the raytrace algorithm
	 */
	void trace();



private:
    rtimage *img;
 	string outpath;
};


// forward declaration so we dont need to include boost/program_options.hpp
class boost::program_options::variables_map;
typedef boost::program_options::variables_map options_map;

class RaytraceFactory
{
public:
	/**
	 * allocates a new {@ Raytracer} on the heap configured with specified options in om
	 * @param om a map of options to be set
	 * @return ref to the allocated {@ Raytracer}
	 */
	static Raytracer* getRaytracerWithOptions(const options_map &om);
};

} /* namespace raytracer */

#endif /* _RAYTRACER_H_ */