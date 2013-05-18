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


glm::vec3 OctNode::centerForQuadrant(Quadrant q)
{
	glm::vec3 qCenter = center;
	float offset = halfWidth / 2.0f;

	switch(q)
	{
		case FNE : qCenter.z += offset; qCenter.y += offset; qCenter.x += offset; break;
		case FNW : qCenter.z += offset; qCenter.y += offset; qCenter.x -= offset; break;
		case FSE : qCenter.z += offset; qCenter.y -= offset; qCenter.x += offset; break;
		case FSW : qCenter.z += offset; qCenter.y -= offset; qCenter.x -= offset; break;
		case BNE : qCenter.z -= offset; qCenter.y += offset; qCenter.x += offset; break;
		case BNW : qCenter.z -= offset; qCenter.y += offset; qCenter.x -= offset; break;
		case BSE : qCenter.z -= offset; qCenter.y -= offset; qCenter.x += offset; break;
		case BSW : qCenter.z -= offset; qCenter.y -= offset; qCenter.x -= offset; break;
	}

	return center;
}

bool OctNode::isLeaf()
{
	return child[0] != NULL;
}

void OctNode::allocateChildren()
{
	float childHalfWidth = halfWidth / 2.0f;

	child[0] = new OctNode(centerForQuadrant(Quadrant::FNE), childHalfWidth);
	child[1] = new OctNode(centerForQuadrant(FNW), childHalfWidth);
	child[2] = new OctNode(centerForQuadrant(FSE), childHalfWidth);
	child[3] = new OctNode(centerForQuadrant(FSW), childHalfWidth);
	child[4] = new OctNode(centerForQuadrant(BNE), childHalfWidth);
	child[5] = new OctNode(centerForQuadrant(BNW), childHalfWidth);
	child[6] = new OctNode(centerForQuadrant(BSE), childHalfWidth);
	child[7] = new OctNode(centerForQuadrant(BSW), childHalfWidth);
}

AABB OctNode::aabb()
{
	AABB bounds;

	for (Primitive *p : primitives)
	{
		AABBContainPrimitive(bounds, *p);
	}

	return bounds;
}

OctreeSceneGraphImp::OctreeSceneGraphImp(int treeDepth)
{
	maxDepth = treeDepth;
	numNodes = 0;
	root     = new OctNode();
}

OctreeSceneGraphImp::~OctreeSceneGraphImp()
{
	delete root;
}

void OctreeSceneGraphImp::build()
{
	root->center = glm::vec3(0);
	root->halfWidth = AABBMaxHalfWidth(root->aabb());

	std::cout << " root bounded by " << root->aabb() << std::endl;
	std::cout << " root halfWidth " << root->halfWidth << std::endl;

	buildOctree_r(root, maxDepth);

}

void OctreeSceneGraphImp::buildOctree_r(OctNode *n, int stopDepth)
{
	if (stopDepth == 0)
		return;

	std::cout << "stopdepth is " << stopDepth <<  std::endl;

	n->allocateChildren();
	numNodes += 8;
	std::cout << "num nodes at  " << numNodes <<  std::endl;

	for(int i = 0; i < 8; i++)
	{
		buildOctree_r(n->child[i], stopDepth - 1);
	}
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
