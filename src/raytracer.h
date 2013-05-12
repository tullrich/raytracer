#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include <string>
#include "rtimage.h"
#include "scenegraph.h"

namespace raytracer {

using std::string;
using namespace raytracer::image;



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

    /**
     * image output pathname
     */
 	string outpath;

 	/**
 	 * Scene info for the raytracer
 	 */
 	SceneGraph *scene;
};


/* Default input values */
#define DEFAULT_IMG_WIDTH 100	// default output image height value
#define DEFAULT_IMG_HEIGHT 100	// default output image height value
#define DEFAULT_OUT_PATH "traceimage.png"	// default output image filepath

/**
 * {@ Raytrace} builder class with config functions and default values
 */
class RaytraceBuilder
{
public:
	RaytraceBuilder() : outputfile(DEFAULT_OUT_PATH), width(DEFAULT_IMG_WIDTH), height(DEFAULT_IMG_HEIGHT) {}

	/**
	 * allocates a new {@ Raytracer} on the heap configured with specified builder options 
	 * @param om a map of options to be set
	 */
	Raytracer* buildRaytracer();

	void setOutpath(const string &newpath) {  outputfile = newpath; };
	void setHeight(int height) { this->height = height; };
	void setWidth(int width) { this->width = width; };
private:
	string outputfile;
	int height;
	int width;
};

} /* namespace raytracer */

#endif /* _RAYTRACER_H_ */
