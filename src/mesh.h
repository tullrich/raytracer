#ifndef _MESH_H_
#define _MESH_H_

#include <iostream>
#include <memory>
#include <string>
#include "common.h"
#include "aabb.h"
#include "primitive.h"
#include "ray.h"

namespace raytracer {

using namespace cgutils;

class Material; //forward declaration

typedef union {
	unsigned int indices[3];
	struct {
		unsigned int x;
		unsigned int y;
		unsigned int z;
	};
} prim_tri;

/**
 * Holds reference to and owns all data needed for rendering a mesh
 */
class mesh_data
{

public:
	typedef std::shared_ptr<mesh_data> mesh_ptr;
	//typedef std::shared_ptr<const mesh_data> mesh_ptr;

	mesh_data() : numVertices(0), verts(NULL), numFaces(0), faces(NULL), mat(NULL) {};
	~mesh_data();


	void setName(const string &name) { this->name = name; };
	void setVertices(int count, const glm::vec3 *ptr);
	void setFaces(int count, const prim_tri *ptr);
	void setMaterial(const Material *mat) { this->mat = mat; };

	/**
	 * Updates the given aabb so that it fully contains this mesh 
	 * @param aabb the bounding box to update
	 */
	void AABBContainMesh(AABB &aabb) const;

	/**
	 * Computes the closest triangle in this mesh intersectioning {@Ray r}.
	 * @param  r            ray to compute intersection
	 * @param  intersection returned barycentric coordinates with respect to tri
	 * @param  tri          returned closest intersecting triangle
	 * @return              true if some prim_tri intersects this mesh
	 */
	bool closestIntersection(const Ray &r, TraceResult &result) const;

	/**
	 * Allocate a fresh buffer of primitives from the contents of this mesh
	 * Memory is owned by the caller
	 * @return       array of {@link Primitive)s of size this->numFaces
	 */
	virtual Primitive** allocatePrimitives() const;


	std::string name;
	
	int TangentsAndBitangents;

	int numVertices;
	const glm::vec3 *verts;

	int numFaces;
	const prim_tri *faces;

	const Material *mat;

private:
	/**
	 * access to private members
	 */
	friend std::ostream& operator<<(std::ostream& o, const mesh_data& b);
};

class textured_mesh_data : public mesh_data
{
public:
	textured_mesh_data() :  uvs(NULL) {};

	void setUVs(const TexCoord *ptr);

	virtual Primitive** allocatePrimitives() const;

	/**
	 * Array of UV coordinates for each vertex. The size of this array is
	 * numVertices
	 */
	const TexCoord *uvs;
};

/**
 * Insert stream operator for mesh_data
 */
std::ostream& operator<<(std::ostream& o, const mesh_data& b);

class  MeshManager : public ResourceManager<std::string, mesh_data::mesh_ptr> {};

}; /* namespace raytracer */

#endif /* _MESH_H_ */
