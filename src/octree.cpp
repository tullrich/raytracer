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

void OctNode::append(OctPrimitive *prim)
{
	if(head == NULL )
	{
		head = prim;
		return;
	}

	OctPrimitive *tail = head;
	while (tail->next != NULL)
	{
		tail = tail->next;
	}
	tail->next = prim;
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
	OctPrimitive *current = root->head;
	int i = 0;
	while (current != NULL)
	{
		current = current->next;
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

	OctPrimitive *first = NULL;
	OctPrimitive *last = NULL;

	for(mesh_data::mesh_ptr mesh : entity->meshes)
	{
		for(int i = 0; i < mesh->numFaces; i++)
		{
			const prim_tri *indices = &mesh->faces[i];

			OctPrimitive *fresh = new OctPrimitive();
			fresh->tri = Triangle(mesh->verts[indices->x], mesh->verts[indices->y], mesh->verts[indices->z]);
			fresh->mesh = mesh;
			fresh->entity = entity;

			if (first != NULL)
			{
				last->next = fresh;
				last = fresh;
			}
			else
			{
				first = last = fresh;
			}
		}
	}

	root->append(first);
}

} /* namespace raytracer */
