#ifndef _MESH_H_
#define _MESH_H_

#include <iostream>
#include <memory>
#include <string>
#include "common.h"
#include "aabb.h"
#include "ray.h"

namespace raytracer {

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

	mesh_data() : numVertices(0), verts(NULL), numFaces(0), TangentsAndBitangents(0), faces(NULL), mat(NULL), uvs(NULL){};
	~mesh_data();


	void setName(const std::string &name) { this->name = name; };
	void setVertices(int count, const glm::vec3 *ptr);
	void setFaces(int count, const prim_tri *ptr);
	void setMaterial(const Material *mat) { this->mat = mat; };
	void setUVs(const TexCoord *ptr);

	/**
	 * Updates the given aabb so that it fully contains this mesh
	 * @param aabb the bounding box to update
	 */
	void AABBContainMesh(AABB &aabb) const;


	int getUVCount() const;

	std::string name;

	int TangentsAndBitangents;

	int numVertices;
	const glm::vec3 *verts;

	int numFaces;
	const prim_tri *faces;

	const Material *mat;

	/**
	 * Array of UV coordinates for each vertex. The size of this array is
	 * numVertices
	 */
	const TexCoord *uvs;

private:
	/**
	 * access to private members
	 */
	friend std::ostream& operator<<(std::ostream& o, const mesh_data& b);
};


/**
 * Insert stream operator for mesh_data
 */
std::ostream& operator<<(std::ostream& o, const mesh_data& b);

}; /* namespace raytracer */

#endif /* _MESH_H_ */
