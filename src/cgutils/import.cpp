#include <stdexcept>

#include "import.h"

namespace cgutils
{

#define CHECK_ASSET_OPEN() \
	CGUTILS_ASSERT(scene != NULL)


AssimpAssetImporter::AssimpAssetImporter() : scene(NULL)
{

}

bool AssimpAssetImporter::open(const std::string& path)
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


	return true;
}

const char* AssimpAssetImporter::error() const
{
	return importer.GetErrorString();
}


int AssimpAssetImporter::numMeshes()
{
	CHECK_ASSET_OPEN()

	return scene->mNumMeshes;
}

int AssimpAssetImporter::numMaterials()
{
	CHECK_ASSET_OPEN()

	return scene->mNumMaterials;
}

int AssimpAssetImporter::getMeshAtIndex(int index)
{
	CHECK_ASSET_OPEN()
	
	aiMesh* mesh = scene->mMeshes[index];
}

void AssimpAssetImporter::accept(const AssetVisitor &visitor)
{
	
}

} /* namespace cgutils */