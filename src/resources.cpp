#include <iostream>
#include <cstring>
#include <regex>

#include "resources.h"
#include "material.h"
#include "entity.h"

namespace raytracer {

#define ASSERT_ASSET_OPEN() \
	CGUTILS_ASSERT(scene != NULL)


AssimpAssetReader::AssimpAssetReader() : scene(NULL), open_file("")
{

}

AssimpAssetReader::~AssimpAssetReader()
{
	if (scene)
	{
		importer.FreeScene();
	}
}

bool AssimpAssetReader::open(const std::string& path)
{

	scene = importer.ReadFile( path, 
        aiProcess_CalcTangentSpace       | 
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);

	if(scene == NULL)
	{
		// Assimp couldnt read the file
		return false;
	}

	open_file = path;
	return true;
}

const char* AssimpAssetReader::error() const
{
	return importer.GetErrorString();
}


int AssimpAssetReader::numMeshes()
{
	ASSERT_ASSET_OPEN()

	return scene->mNumMeshes;
}

int AssimpAssetReader::numMaterials()
{
	ASSERT_ASSET_OPEN()

	return scene->mNumMaterials;
}

int AssimpAssetReader::getMeshAtIndex(int index)
{
	ASSERT_ASSET_OPEN()
	
	aiMesh* mesh = scene->mMeshes[index];
}

void AssimpAssetReader::accept(const EntityVisitor &visitor)
{
	ASSERT_ASSET_OPEN()

	aiNode *root = NULL;
	aiMatrix4x4 identity;

	std::cout << "num cams " << scene->mNumCameras << std::endl;
	
	root = scene->mRootNode;
	if(root)
	{
		visitNode_r(root, identity, visitor);
	}
	else
	{
		std::cout << "Warning: file " << open_file << " has no root node? (internal Assimp problem)" << std::endl;
	}

}


glm::vec3* vertBufferForAiVector3D(int mNumVertices, aiVector3D *verts)
{
	glm::vec3 *buf = new glm::vec3[mNumVertices];

	for (int i = 0; i < mNumVertices; i++)
	{
		buf[i] = glm::vec3(verts[i].x, verts[i].y, verts[i].z);
		//std::cout << "vert (" << buf[i].x << "," << buf[i].y << "," << buf[i].z << ")" << std::endl;
	}

	return buf;
}

prim_tri* faceBufferForAiFace(int mNumFaces, aiFace *faces)
{
	prim_tri *buf = new prim_tri[mNumFaces];

	for (int i = 0; i < mNumFaces; i++)
	{
		aiFace *face = &faces[i];
		if (face->mNumIndices == 3)
		{
			memcpy(&buf[i], face->mIndices, sizeof(prim_tri)); 
			//std::cout << " face (" << buf[i].x << "," << buf[i].y << "," << buf[i].z << ")" << std::endl;
		}
		else
		{
			std::cout << "Warning: mesh includes non-tri faces!" << std::endl;
		}
	}

	return buf;
}

void getAiMatColor(const aiMaterial &ai_mat, const char* pKey, unsigned int type, unsigned int idx, glm::vec3 &copy)
{
	aiColor3D color (0.f,0.f,0.f); 
	ai_mat.Get(pKey, type, idx, color);
	copy = vec3(color.r, color.g, color.b);
}

void getAiMatFloat(const aiMaterial &ai_mat, const char* pKey, unsigned int type, unsigned int idx, float &copy)
{
	copy = 0;
	ai_mat.Get(pKey, type, idx, copy);
}


Material* AssimpAssetReader::getMaterial(const int mMaterialIndex)
{
	Material *m = MaterialManager::getInstance().get(mMaterialIndex);

	if (m == NULL)
	{
		aiMaterial *ai_mat = scene->mMaterials[mMaterialIndex];

		aiString name;
		ai_mat->Get(AI_MATKEY_NAME, name);
		m = new Material(name.C_Str());

		getAiMatColor(*ai_mat, AI_MATKEY_COLOR_DIFFUSE, m->diffuse);
		getAiMatColor(*ai_mat, AI_MATKEY_COLOR_AMBIENT, m->ambient);
		getAiMatColor(*ai_mat, AI_MATKEY_COLOR_SPECULAR, m->specular);
		getAiMatColor(*ai_mat, AI_MATKEY_COLOR_EMISSIVE, m->emissive);
		getAiMatFloat(*ai_mat, AI_MATKEY_SHININESS, m->shineness);

		MaterialManager::getInstance().add(mMaterialIndex, *m);
	}

	return m;
}

mesh_data::mesh_ptr AssimpAssetReader::buildMesh(const aiMesh &node)
{
	glm::vec3 *verts = NULL;
	prim_tri *faces  = NULL;
	Material *mat    = NULL;

	// allocate and set name
	mesh_data::mesh_ptr pMesh(new mesh_data());
	pMesh->setName(node.mName.C_Str());

	// copy the verts to memory managed by us
	verts = vertBufferForAiVector3D(node.mNumVertices, node.mVertices);
	pMesh->setVertices(node.mNumVertices, verts);

	// copy the face indices to memory managed by us
	faces = faceBufferForAiFace(node.mNumFaces, node.mFaces);
	pMesh->setFaces(node.mNumFaces, faces);

	// setup the material pointer
	mat = getMaterial(node.mMaterialIndex);
	if(mat)
	{
		pMesh->setMaterial(mat);
	}

	// add it to the meshmanager
	// TODO: broken
	MeshManager::getInstance().add(node.mName.C_Str(), pMesh);

	return pMesh;
}

Entity::entity_ptr AssimpAssetReader::buildEntity(const aiNode &node, aiMatrix4x4 worldSpace)
{
	int mesh_index           = 0;
	mesh_data::mesh_ptr mesh = NULL;
	aiVector3D pos, scale;
 	aiQuaternion rot;

	Entity::entity_ptr entity(new Entity());
	entity->setName(node.mName.C_Str());

	worldSpace.Decompose(scale, rot, pos);
	entity->position = glm::vec3(pos.x, pos.y, pos.z);
	entity->rotation = glm::quat(rot.w, rot.x, rot.y, rot.z);
	entity->scale = glm::vec3(scale.x, scale.y, scale.z);

	for (int i = 0; i < node.mNumMeshes; i++)
	{
		mesh_index = node.mMeshes[i];
		mesh = buildMesh(*(scene->mMeshes[mesh_index]));
		entity->addMeshComponent(mesh);
	}

	return entity;
}


void AssimpAssetReader::visitNode_r(const aiNode *node, aiMatrix4x4 parentToWorldSpace, const EntityVisitor &visitor)
{
	aiNode *child = NULL;
	Entity::entity_ptr ent = NULL;
	aiMatrix4x4 worldSpace;

	//compute the world space transformation matrix for this model
	worldSpace = node->mTransformation * parentToWorldSpace;

	/*std::cout << worldSpace.a1 << " " << worldSpace.a2 << " " << worldSpace.a3 << std::endl;
	std::cout << worldSpace.b1 << " " << worldSpace.b2 << " " << worldSpace.b3 << std::endl;
	std::cout << worldSpace.c1 << " " << worldSpace.c2 << " " << worldSpace.c3 << std::endl;
	std::cout << worldSpace.d1 << " " << worldSpace.d2 << " " << worldSpace.d3 << std::endl;*/

	ent = buildEntity(*node, worldSpace);
	visitor.visit(ent);

	// visit each child node of cur_node
	for (int i = 0; i < node->mNumChildren; i++)
	{
		child = node->mChildren[i];
		visitNode_r(child, worldSpace, visitor);
	}
}

unique_ptr<AssetReader> ResourceLoaderFactory::getReaderForFiletype(const string &filename)
{

	if (std::regex_match (filename, std::regex(".*\\.obj") ))
	{
		return unique_ptr<AssimpAssetReader>(new AssimpAssetReader());
	}
	else if (std::regex_match (filename, std::regex(".*\\.3ds") ))
	{
		return unique_ptr<AssimpAssetReader>(new AssimpAssetReader());
	}
	else
	{
		return NULL;
	}
}


} /* namespace raytracer */
