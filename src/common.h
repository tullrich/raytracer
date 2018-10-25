#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#ifndef NDEBUG
  #include <cassert>
  #define RAYTRACER_ASSERT(condition) \
  { \
      if(!(condition)) \
      { \
          std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__; \
          std::cerr << " inside " << __FUNCTION__ << std::endl; \
          std::cerr << "Condition: " << " "; \
          abort(); \
      } \
  }
#else
  #define RAYTRACER_ASSERT(condition) (condition)
#endif

namespace raytracer {

/* Default input values */
#define DEFAULT_IMG_WIDTH 100	// default output image height value
#define DEFAULT_IMG_HEIGHT 100	// default output image height value
#define DEFAULT_THREAD_COUNT 1
#define DEFAULT_OUT_PATH "traceimage.png"	// default output image filepath


#define CONFIG_FILEPATH "raytracer.cfg" // input configuration filepath

#define OPTION_WIDTH "w"
#define OPTION_HEIGHT "h"
#define OPTION_OUTPATH "o"
#define OPTION_THREADS "threads"
#define OPTION_ASSETPATH "asset-filepath"

#define FLOAT_ERROR 0.0001f

typedef glm::vec3 RGB;
typedef glm::vec2 TexCoord;

class Triangle
{
public:
	Triangle() : A(0), B(0), C(0) {};
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) : A(a), B(b), C(c) {};

	glm::vec3 A;
	glm::vec3 B;
	glm::vec3 C;

	/**
	 * converts a vec3 containing barycentric coordinates relative to this triangle
	 * into a point
	 * @param  intersection barycentric coordinates, |intersection| = 1.0
	 * @return              intersection point on the surface of this triangle
	 */
	glm::vec3 intersectionToPoint(const glm::vec4 &intersection) const;

	glm::vec3 normal()  const;
};

/**
 * Holds UVS for a triangle
 */
class UVTriangle
{
public:
	UVTriangle() : A(0), B(0), C(0) {};
	UVTriangle(TexCoord a, TexCoord b, TexCoord c) : A(a), B(b), C(c) {};

	TexCoord A;
	TexCoord B;
	TexCoord C;

	/**
	 * converts a vec2 containing barycentric coordinates relative to this triangle
	 * into the UVs at that point
	 * @param  intersection barycentric coordinates, |intersection| = 1.0
	 * @return              intersection point on the surface of this triangle
	 */
	TexCoord interpolateUV(const glm::vec4 &intersection) const;
};


/**
 * A plane representing all points X s.t. dot(X, n) = d
 */
struct Plane
{
	Plane() : n(0), d(0) {};

	float d;
	glm::vec3 n;
};

glm::vec3 adjustFloatingPointToward(const glm::vec3 point, const glm::vec3 &towards);

inline float clamp(float x, float a, float b)
{
    return x < a ? a : (x > b ? b : x);
}

/**
 * random float between 0 and 1 inclusive
 * @return random float
 */
inline float randf()
{
	return (float)rand()/(float)RAND_MAX;
}

/**
 * generate a uniformally random direction vector on a hemisphere around
 * the input normal
 * @param  normal normal vector around which the direction vector will be
 * @return        random direction vector
 */
glm::vec3 uniformDirectionOnHemisphere(const glm::vec3 normal);

std::string parentPath(const std::string path);
std::string appendFilename(const std::string path, const std::string filename);

std::ostream& operator<<(std::ostream& o, const glm::vec3& b);
std::ostream& operator<<(std::ostream& o, const glm::mat4& b);
std::ostream& operator<<(std::ostream& o, const Triangle& b);


template<typename T>
class Visitor
{
public:
	virtual void visit(T &object) const  = 0;
};

/**
 * Templated singleton pattern
 * Derived classes must friend Singleton<> so that its private
 * constructor is available in this scope
 */
template<typename T>
class Singleton
{
protected:
	Singleton() {};
	virtual ~Singleton() {};

public:

	static T& getInstance()
	{
		if (!Singleton<T>::instance)
		{
			// allocate singleton, run only once
			Singleton<T>::instance = new T();
		}

		return *(Singleton<T>::instance);
	}

private:
	static T *instance;

 	/**
 	 * Private copy constructor to prevent copy construction
 	 */
	Singleton( Singleton const & ) {};

	/**
	 * Private operator to prevent assignment
	 */
	Singleton &operator=( Singleton const & ) {};
};

template< typename T >
T *Singleton<T>::instance        = NULL;



template <typename K, typename T>
class ResourceManager : public Singleton<ResourceManager<K, T> >
{
	/**
	 * Must be a friend class of the singleton base
	 */
	friend class Singleton<ResourceManager<K, T> >;

protected:
	typedef std::map<K, T*> entity_map;
	ResourceManager() {};

public:

	/**
	 * Adds a resource of type T into this manager.
	 * @param key key for inserted element
	 * @param r resource instance to be added
	 */
	void add(const K &key, T &r)
	{
		if (resources.find(key) != resources.end())
		{
			//std::cout << "Warning: trying to add duplicate key " << key << std::endl;
			return;
		}

		//std::cout << " inserting('" << key << "') " << r << std::endl;
		resources[key] = &r;
	}

	/**
	 * get a resource from this manager
	 * @param  key key for the resource
	 * @return     ref to the resource
	 */
	T* get(const K& key)
	{
		if(resources.find(key) != resources.end())
		{
			return resources[key];
		}

		return NULL;
	}

	/**
	 * fnv1 hash (http://isthe.com/chongo/tech/comp/fnv/) using recommended
	 * seed and prime
	 * @param  str string to be hashed
	 * @return     	hashed int
	 */
	int calcHash(const std::string &str)
	{
		int prime = 0x01000193; //   16777619
		int hash = 0x811C9DC5; // the seed - 2166136261

		for (char c : str)
		{
			hash = (c ^ hash) * prime;
		}

		return hash;
	}

	int size() const
	{
		return resources.size();
	}

private:
	entity_map resources;

};

} /* raytracer */

#endif /* _COMMON_H_ */
