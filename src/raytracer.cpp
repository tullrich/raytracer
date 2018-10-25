#include "raytracer.h"
#include <math.h>
#include <ctime>
#include <chrono>
#include <boost/thread.hpp>

namespace
{
	long long currentTime()
	{
		std::chrono::time_point< std::chrono::system_clock > now = std::chrono::system_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>( now.time_since_epoch() ).count();
	}
}

namespace raytracer {


Raytracer::Raytracer(const std::string &outputfile, int width, int height, int threads) : img(width, height), outpath(outputfile), numThreads( threads )
{
    scene = NULL;
    srand((unsigned)time(0));

	std::cout << "Initializing raytracer:" << std::endl
		<< "\tOutpath=" << outputfile << std::endl
		<< "\tResolution=<" << height << ", " << width << ">" << std::endl
		<< "\tThreads=" << threads << std::endl;
}

Raytracer::~Raytracer()
{
}

RGB Raytracer::emittedRadiance(const TraceResult &r)
{
    return 2.0f * r.p->Le(r.intersection);
}

RGB Raytracer::directRadiance(const TraceResult &r)
{
    RGB color(0);
    RGB per_light(0);
    TraceResult result;
    Ray to_light;

    glm::vec3 point = r.biasedIntersectionPoint();

    for (Light::light_ptr light : scene->lights)
    {
        // get some light ray for this light
        light->genShadowRay(point, to_light);

        if (scene->testVisibility(to_light, result))
        {
            light->getAttenuatedRadiance(to_light, per_light);
            color += per_light * (r.p->BRDF(r.intersection)) * r.p->geometricTerm(to_light.n);
			//color += per_light * (r.p->BRDF(r.intersection) / (float) M_PI) * r.p->geometricTerm(to_light.n
        }
    }

    return color;
}

RGB Raytracer::indirectRadiance(const TraceResult &r, int depth)
{
    RGB indirect_color(0);
    glm::vec3 point = r.biasedIntersectionPoint();

	int numIndirectRays = 1;
	if (depth > 0)
	{
		for (int i = 0; i < numIndirectRays; i++)
		{
			float survive = 1.0f;
			float inverse_pdf = 0;
			glm::vec3 rand_direction = uniformImportanceSampling(r.p->surfaceNormal(), inverse_pdf);
			Ray indirect_ray(point, point + rand_direction);
			RGB per_ray_color;
			traceRay(indirect_ray, per_ray_color, depth - 1);
			indirect_color +=  survive * per_ray_color * r.p->BRDF(r.intersection) * r.p->geometricTerm(rand_direction) * 2.0f;
		}
	}
    return indirect_color / (float) numIndirectRays;
}

bool Raytracer::traceRay(const Ray &r, RGB &color, int depth)
{
    TraceResult result;

	bool ret = false;
    if(scene->traceRay(r, result))
    {
        color = emittedRadiance(result);
        color += directRadiance(result);
        color += indirectRadiance(result, depth);
		ret = true;
		stats.raysHit++;
    }
	else
	{
		color = RGB(0.1f, 0.1f, 0.1f);
		stats.raysMissed++;
	}

	// Update stats
	stats.totalRaysCast++;
	stats.totalPrimitiveIntersections += result.primitiveIntersections;
	stats.totalNodeTraversals += result.nodesTraversed;
    return ret;
}

void Raytracer::lightPixel(int u, int v)
{
    RGB color;
    Ray r;

    int numViewRays = 1;
    for(int i = 0; i < numViewRays ; i++)
    {
        RGB view_ray(0);
        camera->genViewingRay(u, v, r);
        traceRay(r, view_ray, 1);
        color += view_ray;
    }

    color /= numViewRays;

    img.setPixelColor(u, v , color);
}

void Raytracer::render(int u_min, int u_max)
{
    for (int u = u_min; u < u_max; ++u)
    {
        for (int v = 0; v < img.height; ++v)
        {
			if (u==514 && v==629)
			{
				lightPixel( u, v );
				continue;
			}
			lightPixel( u, v );
        }
    }
}

void Raytracer::run()
{
    RAYTRACER_ASSERT(scene);
    RAYTRACER_ASSERT(camera);

	// clear stats
	stats.reset();

	// mark start time
	long long startms = currentTime();

	// spin up threads
	if (numThreads > 1)
	{
		boost::thread_group group;
		int columnsPerThread = img.width / numThreads;
		for ( int i = 1; i < numThreads; i++ )
		{
			group.add_thread( new boost::thread(&Raytracer::render, this, i * columnsPerThread, (i + 1) * columnsPerThread));
		}

		render(0, columnsPerThread);

		// wait for all
		group.join_all();
	}
	else
	{
		render(0, img.width);
	}

	// write image
    if (!img.writeOut(outpath))
    {
        std::cout << "error: FreeImage_Save()" << std::endl;
    }

	// record duration
	stats.totalTime = currentTime() - startms;
}

bool Raytracer::russianRoulette(const RGB &reflectance, float &survivor)
{
    float p = fmax(reflectance[0], fmax(reflectance[1], reflectance[2]));
    survivor = 1.0f/p;
    if (randf() > p)
    {
        //std::cout << "russianRoulette stop" << std::endl;
        return true;
    }
    //std::cout << "russianRoulette keep going" << std::endl;
    return false;
}

glm::vec3 Raytracer::uniformImportanceSampling(const glm::vec3 normal, float &inverse_pdf)
{
    inverse_pdf = 2.0f * (float)M_PI;
    return uniformDirectionOnHemisphere(normal);
}

std::ostream& operator<<( std::ostream& os, const RaytracerStats& stats )
{
	os << "\tDuration: " << stats.totalTime / 1000.0f << " seconds" << std::endl;
	os << "\tRays Cast:" << stats.totalRaysCast << " Hit:" << stats.raysHit << " Missed:" << stats.raysMissed << std::endl;
	os << "\tPrimitive Intersections Total:" << stats.totalPrimitiveIntersections << " Average:" << stats.totalPrimitiveIntersections / ( float )stats.totalRaysCast << std::endl;
	os << "\tNode Traversals Total:" << stats.totalNodeTraversals << " Average:" << stats.totalNodeTraversals / (float)stats.totalRaysCast << std::endl;
	return os;
}

} /* namespace raytracer */
