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


glm::vec3 OctNode::centerForQuadrant(unsigned int q)
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

	return qCenter;
}

bool OctNode::isLeaf() const
{
	return primitives.size() > 0;
}

void OctNode::pushDown()
{
	AABB childAABB;
	int *count = new int[primitives.size()]();

	for (int i = 0; i < 8; i++)
	{
		int j = 0;
		//bool  = false;
		childAabb(childAABB, i);
		for(Primitive *p : primitives)
		{
			if(p->intersects(childAABB))
			{
				if(child[i] == NULL)
				{
					allocateChild(i);
				}
				count[j] += 1;
				child[i]->append(p);
			}
			j++;
		}
		//if(child[i]->primitives.size() > 0)
		//std::cout << "gave child " << i << " " << child[i]->primitives.size() << " primitives " << std::endl; 
	}
	int n = 0;
	int i = 0;
	for (Primitive *p : primitives)
	{
		if(count[i] <= 0)
		{
			n++;
			//std::cout << "broken primitive " << reinterpret_cast<TrianglePrimitive*>(p)->face << std::endl; 
		}
		i++;
	}
	delete count;
	//CGUTILS_ASSERT(n == 0); // if we didnt overlap >=1 child, its a bug
	primitives.clear();
	
}

void OctNode::allocateChild(unsigned int zone)
{
	float childHalfWidth = halfWidth / 2.0f;

	child[zone] = new OctNode(centerForQuadrant(zone), childHalfWidth);
}

void OctNode::allocateChildren()
{
	float childHalfWidth = halfWidth / 2.0f;

	child[BSW] = new OctNode(centerForQuadrant(BSW), childHalfWidth);
	child[FSW] = new OctNode(centerForQuadrant(FSW), childHalfWidth);
	child[BNW] = new OctNode(centerForQuadrant(BNW), childHalfWidth);
	child[FNW] = new OctNode(centerForQuadrant(FNW), childHalfWidth);
	child[BSE] = new OctNode(centerForQuadrant(BSE), childHalfWidth);
	child[FSE] = new OctNode(centerForQuadrant(FSE), childHalfWidth);
	child[BNE] = new OctNode(centerForQuadrant(BNE), childHalfWidth);
	child[FNE] = new OctNode(centerForQuadrant(FNE), childHalfWidth);
}


void OctNode::aabb(AABB &aabb)
{
	for (int i = 0; i < 3; i++)
	{
		aabb.max[i] = center[i] + halfWidth;
		aabb.min[i] = center[i] - halfWidth;
	}
}


void OctNode::childAabb(AABB &aabb, unsigned int zone)
{
	float childHalfWidth = halfWidth / 2.0f;
	glm::vec3 childCenter = centerForQuadrant(zone);

	for (int i = 0; i < 3; i++)
	{
		aabb.max[i] = childCenter[i] + childHalfWidth;
		aabb.min[i] = childCenter[i] - childHalfWidth;
	}
}

bool OctNode::testPrimitives(const Ray &r, TraceResult &result)
{
	TraceResult temp_result;
	bool found_one = false;

	for(Primitive *p : primitives)
	{
		if(p->intersects(r, temp_result))
		{
			if (!found_one || temp_result.intersection.w < result.intersection.w)
			{
				found_one = true;
				result = temp_result;
			}
		}
	}

	OctreeSceneGraphImp::numCompares += primitives.size();
	//std::cout << "got here    found " << found_one << std::endl; 
	return found_one;
}

static unsigned char first_node(float tx0, float ty0, float tz0, float txm, float tym, float tzm)
{
	unsigned char ret_value = 0;
	float compare = fmaxf(tx0, fmaxf(ty0, tz0));

	if(tx0 == compare) // entry plane YZ
	{
		if(tym < tx0) ret_value |= 2;
		if(tzm < tx0) ret_value |= 1;
	}
	else if(ty0 == compare) // entry plane XZ
	{
		if(txm < ty0) ret_value |= 4;
		if(tzm < ty0) ret_value |= 1;
	} 
	else // entry plane XY
	{
		if(txm < tz0) ret_value |= 4;
		if(tym < tz0) ret_value |= 2;
	}

	return ret_value;
}

static unsigned char next_node(float txm, unsigned char next_in_x, float tym, unsigned char next_in_y, float tzm, unsigned char next_in_z)
{
	float compare = fminf(txm, fminf(tym, tzm));

	if(txm == compare) // exit plane YZ
	{
		return next_in_x;
	}
	else if(tym == compare) // exit plane XZ
	{
		return next_in_y;
	} 
	else // exit plane XY
	{
		return next_in_z;
	}
}

bool OctNode::traceRay(const Ray &r, TraceResult &result, float tx0,float ty0, float tz0, float tx1, float ty1, float tz1, unsigned char a)
{
	if (tx1 < 0.0f || ty1 < 0.0f || tz1 < 0.0f)
	{
		return false;
	}

	if(isLeaf())
	{
		//std::cout << " in a child " << std::endl; 
		return testPrimitives(r, result);
	}
	//std::cout << " in a parent " << std::endl; 

	float txm = 0.5f * (tx0 + tx1); // t value when the ray intersects the x-axis of this node
	float tym = 0.5f * (ty0 + ty1); // t value when the ray intersects the y-axis of this node
	float tzm = 0.5f * (tz0 + tz1); // t value when the ray intersects the z-axis of this node

	unsigned char currNode = first_node(tx0, ty0, tz0, txm, tym, tzm);
	do
	{
		switch(currNode)
		{
			case 0 :
			{
				if(child[BSW ^ a] && child[BSW ^ a]->traceRay(r, result, tx0, ty0, tz0, txm, tym, tzm, a)) // back - south - west
				{
					return true;
				}
				currNode = next_node(txm, 4, tym, 2, tzm, 1);
				//currNode = new_node()
				break;
			}
			case 1 :
			{
				if(child[FSW ^ a] && child[FSW ^ a]->traceRay(r, result, tx0, ty0, tzm, txm, tym, tz1, a)) // front - south - west
				{
					return true;
				}
				currNode = next_node(txm, 5, tym, 3, tz1, 8);
				//currNode = new_node()
				break;
			}
			case 2 :
			{
				if(child[BNW ^ a] && child[BNW ^ a]->traceRay(r, result, tx0, tym, tz0, txm, ty1, tzm, a)) // back - north - west
				{
					return true;
				}
				currNode = next_node(txm, 6, ty1, 8, tzm, 3);
				//currNode = new_node()
				break;
			}
			case 3 :
			{
				if(child[FNW ^ a] && child[FNW ^ a]->traceRay(r, result, tx0, tym, tzm, txm, ty1, tz1, a)) // front - north - west
				{
					return true;
				}
				currNode = next_node(txm, 7, ty1, 8, tz1, 8);
				//currNode = new_node()
				break;
			}
			case 4 :
			{
				if(child[BSE ^ a] && child[BSE ^ a]->traceRay(r, result, txm, ty0, tz0, tx1, tym, tzm, a)) // back - south - east
				{
					return true;
				}
				currNode = next_node(tx1, 8, tym, 6, tzm, 5);
				//currNode = new_node()
				break;
			}
			case 5 :
			{
				if(child[FSE ^ a] && child[FSE ^ a]->traceRay(r, result, txm, ty0, tzm, tx1, tym, tz1, a)) // front - south - east
				{
					return true;
				}
				currNode = next_node(tx1, 8, tym, 7, tz1, 8);
				//currNode = new_node()
				break;
			}
			case 6 :
			{
				if(child[BNE ^ a] && child[BNE ^ a]->traceRay(r, result, txm, tym, tz0, tx1, ty1, tzm, a)) // back - north - east
				{
					return true;
				}
				currNode = next_node(tx1, 8, ty1, 8, tzm, 7);
				//currNode = new_node()
				break;
			}
			case 7 :
			{
				if(child[FNE ^ a] && child[FNE ^ a]->traceRay(r, result, txm, tym, tzm, tx1, ty1, tz1, a)) // front - north - east
				{
					return true;
				}
				currNode = 8;
				//currNode = new_node()
				break;
			}
		}
	} while(currNode < 8);

	return false;
}

long OctreeSceneGraphImp::numCompares = 0;

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
	root->halfWidth = optimalRootWidth() + 1.0f;

	//buildOctree_r(root, maxDepth);
	pushDown_r(root, maxDepth);
}

float OctreeSceneGraphImp::optimalRootWidth()
{
	AABB bounds;

	int i = 0;
	for (Primitive *p : root->primitives)
	{
		p->updateAABB(bounds);
	}

	return AABBMaxHalfWidth(bounds);
}

void OctreeSceneGraphImp::buildOctree_r(OctNode *n, int stopDepth)
{
	if (stopDepth == 0)
		return;


	n->allocateChildren();

	for(int i = 0; i < 8; i++)
	{
		buildOctree_r(n->child[i], stopDepth - 1);
	}
}

void OctreeSceneGraphImp::pushDown_r(OctNode *n, int stopDepth)
{
	if (stopDepth == 0)
		return;

	n->pushDown();

	for(int i = 0; i < 8; i++)
	{
		if(n->child[i])
			pushDown_r(n->child[i], stopDepth - 1);
	}
}

bool OctreeSceneGraphImp::traceRay(const Ray &r, TraceResult &result) const
{
	numCompares = 0;
	unsigned char a = 0;
	Ray mod_r = r;

	if(r.n.x < 0)
	{
		mod_r.p.x = -mod_r.p.x;
		mod_r.n.x = -mod_r.n.x;
		a |= 4;
	}

	if(r.n.y < 0)
	{
		mod_r.p.y =  -mod_r.p.y;
		mod_r.n.y = -mod_r.n.y;
		a |= 2;
	}

	if(r.n.z < 0)
	{
		mod_r.p.z = -mod_r.p.z;
		mod_r.n.z = -mod_r.n.z;
		a |= 1;
	}


	AABB root_bounds;
	root->aabb(root_bounds);

	float tx0 = (root_bounds.min.x - mod_r.p.x) / mod_r.n.x; // entry t on x-axis
	float tx1 = (root_bounds.max.x - mod_r.p.x) / mod_r.n.x; // exit t on x-axis
	float ty0 = (root_bounds.min.y - mod_r.p.y) / mod_r.n.y; // entry t on y-axis
	float ty1 = (root_bounds.max.y - mod_r.p.y) / mod_r.n.y; // exit t on y-axis
	float tz0 = (root_bounds.min.z - mod_r.p.z) / mod_r.n.z; // entry t on z-axis
	float tz1 = (root_bounds.max.z - mod_r.p.z) / mod_r.n.z; // exit t on z-axis

	float max = fmaxf(tx0, fmaxf(ty0, tz0)); // earliest we are through all planes
	float min = fminf(tx1, fminf(ty1, tz1)); // latest we are inside all exit planes
	if(fmaxf(tx0, fmaxf(ty0, tz0)) < fminf(tx1, fminf(ty1, tz1)))
	{
		bool found = root->traceRay(r, result, tx0, ty0, tz0, tx1, ty1, tz1, a);

		return found;
	}

	return false;
}


void OctreeSceneGraphImp::addPrimitive(Primitive *p)
{
	root->append(p);
}

} /* namespace raytracer */
