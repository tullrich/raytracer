#include <iostream>
#include <regex>

#include "resources.h"

namespace raytracer {

#define ASSERT_ASSET_OPEN() \
	CGUTILS_ASSERT(scene != NULL)


AssimpAssetReader::AssimpAssetReader() : scene(NULL), open_file("")
{

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

void AssimpAssetReader::accept(const MeshDataVisitor &visitor)
{
	ASSERT_ASSET_OPEN()

	aiNode *root = NULL;

	root = scene->mRootNode;
	if(root)
	{
		visitNode_r(root, visitor);
	}
	else
	{
		std::cout << "Warning: file " << open_file << " has no root node? (internal Assimp problem)" << std::endl;
	}

}


void AssimpAssetReader::visitNode_r(const aiNode *node, const MeshDataVisitor &visitor)
{
	aiNode *child = NULL;

	std::cout << "visiting node: " << node->mName.C_Str() << std::endl;

	// visit each child node of cur_node
	for (int i = 0; i < node->mNumChildren; i++)
	{
		child = node->mChildren[i];
		visitNode_r(child, visitor);
	}
}

unique_ptr<AssetReader> ResourceLoaderFactory::getReaderForFiletype(const string &filename)
{

	if (std::regex_match (filename, std::regex(".*\.obj") ))
	{
		return unique_ptr<AssimpAssetReader>(new AssimpAssetReader());
	}
	else
	{
		return NULL;
	}
}

void MeshDataSceneAdder::visit(mesh_data &mesh)
{
	std::cout << "visiting a mesh" << std::endl;
}

} /* namespace raytracer */
