#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include <string>
#include <boost/thread.hpp>
#include "rtimage.h"
#include "scenegraph.h"
#include "common.h"
#include "material.h"
#include "camera.h"
#include "ray.h"

namespace raytracer {

using std::string;
using namespace raytracer::image;

/**
 * raytracer root
 */
class Raytracer 
{
public:
	Raytracer(const string &outputfile, int height, int width);
	~Raytracer();

	string& getOutpath() { return outpath; };
	void setOutpath(const string &newpath) {  outpath = newpath; };

	/**
	 * run the raytrace algorithm
	 */
	void trace(int u_min, int u_max);
	void run();

	/* setters */
	void setScene(SceneGraph *scene) { this->scene = scene; };
	void setCamera(Camera *camera) { this->camera = camera; };

private:
	void rayForPixel(int i, int j, Ray &r) const;

	RGB emittedRadiance(const Material &mat, const Triangle &tri, const glm::vec4 intersection);
	RGB directRadiance(const Material &mat, const Triangle &tri, const glm::vec4 intersection);
	RGB indirectRadiance(const Material &mat, const Triangle &tri, const glm::vec4 intersection, int depth);

	bool traceRay(const Ray &r, RGB &color, int depth);
	bool russianRoulette(const RGB &reflectance, float &survivor);
	void lightPixel(int u, int v);

    Image img;

    /**
     * image output pathname
     */
 	string outpath;

 	/**
 	 * Scene info for the raytracer
 	 */
 	SceneGraph *scene;

 	/**
 	 * Camera for the renderer
 	 */
 	Camera *camera;
};

/**
 * {@ Raytrace} builder class with config functions and default values
 */
class RaytraceBuilder
{
public:
	/**
	 * setup default input values
	 */
	RaytraceBuilder() : outputfile(DEFAULT_OUT_PATH), height(DEFAULT_IMG_HEIGHT), width(DEFAULT_IMG_WIDTH) {}

	/**
	 * allocates a new {@ Raytracer} on the heap configured with specified builder options 
	 * @param om a map of options to be set
	 */
	Raytracer* buildRaytracer();

	/* setters */
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
