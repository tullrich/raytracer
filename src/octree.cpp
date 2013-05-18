#include "octree.h"

namespace raytracer {

OctNode::~OctNode()
{
	for(int i = 0; i < 8; i++)
	{
		if (child[i])
		{
			delete child[i];
		}
	}
}

void OctNode::append(Primitive *prim)
{
	primitives.push_back(prim);
}

OctreeSceneGraphImp::OctreeSceneGraphImp(int treeDepth)
{
	maxDepth = treeDepth;
	root     = new OctNode(); 
}

OctreeSceneGraphImp::~OctreeSceneGraphImp()
{
	delete root;
}

void OctreeSceneGraphImp::build()
{
	std::cout << "building " << std::endl;
	int i = 0;
	for (Primitive *p : root->primitives)
	{
		i++;
	}


	std::cout << i << " tris attached to root " << std::endl;
}

OctNode* OctreeSceneGraphImp::buildOctree_r(glm::vec3 center,  float halfWidth, int stopDepth)
{

}

bool OctreeSceneGraphImp::traceRay(const Ray &r, TraceResult &result) const
{

}

void OctreeSceneGraphImp::addEntity(Entity::entity_ptr entity)
{
	allEntities.push_back(entity);

	for(mesh_data::mesh_ptr mesh : entity->meshes)
	{
		for(int i = 0; i < mesh->numFaces; i++)
		{
			const prim_tri *indices = &mesh->faces[i];

			Triangle t(mesh->verts[indices->x], mesh->verts[indices->y], mesh->verts[indices->z]);
			Primitive *fresh = new TrianglePrimitive(t, mesh->mat);

			root->append(fresh);
		}
	}

}

} /* namespace raytracer */
