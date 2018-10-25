#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include <string>
#include <atomic>
#include "rtimage.h"
#include "scenegraph.h"
#include "common.h"
#include "primitive.h"
#include "material.h"
#include "camera.h"
#include "ray.h"

namespace raytracer {


/**
* performance stats for a Raytracer instance.
*/
class RaytracerStats
{
public:
	RaytracerStats() 
	{
		reset();
	}

	void reset()
	{
		totalTime = 0;
		totalPrimitiveIntersections = 0;
		totalRaysCast = 0;
		raysHit = 0;
		raysMissed = 0;
		totalNodeTraversals = 0;
	}

	// in milliseconds
	long long totalTime;
	
	// total number of primitive intersections computed
	std::atomic<long long> totalPrimitiveIntersections;

	// Total number of rays cast
	std::atomic<long long> totalRaysCast, raysHit, raysMissed;

	// Total number of scenegraph node traversals
	std::atomic<long long> totalNodeTraversals;
};

/**
 * raytracer root
 */
class Raytracer
{
public:
	Raytracer(const std::string &outputfile, int height, int width, int numThreads);
	~Raytracer();

	std::string& getOutpath() { return outpath; };
	void setOutpath(const std::string &newpath) {  outpath = newpath; };

	/**
	 * run the raytrace algorithm
	 */
	void render(int u_min, int u_max);
	void run();

	/* setters */
	void setScene(SceneGraph *scene) { this->scene = scene; };
	void setCamera(Camera *camera) { this->camera = camera; };

	/* debug */
	const RaytracerStats& getStats() const { return stats; }
private:

	RGB emittedRadiance(const TraceResult &r);
	RGB directRadiance(const TraceResult &r);
	RGB indirectRadiance(const TraceResult &r, int depth);

	bool traceRay(const Ray &r, RGB &color, int depth);
	bool russianRoulette(const RGB &reflectance, float &survivor);
	void lightPixel(int u, int v);

	glm::vec3 uniformImportanceSampling(const glm::vec3 normal, float &inverse_pdf);
	glm::vec3 cosineImportanceSampling(const glm::vec3 normal, float &inverse_pdf);

    Image img;

    /**
     * image output pathname
     */
 	std::string outpath;

 	/**
 	 * Scene info for the raytracer
 	 */
 	SceneGraph *scene;

 	/**
 	 * Camera for the renderer
 	 */
 	Camera *camera;

	/**
	* Stats collected during a run.
	*/
	RaytracerStats stats;

	/**
	* The number of threads to use.
	*/
	int numThreads;
};

std::ostream& operator<<( std::ostream& os, const RaytracerStats& dt );

} /* namespace raytracer */

#endif /* _RAYTRACER_H_ */
