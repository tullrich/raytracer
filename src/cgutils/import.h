#ifndef _IMPORT_H
#define _IMPORT_H

#include "cgutils.hpp"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

namespace cgutils
{


class AssetFile
{
public:
	AssetFile();
	~AssetFile();

	/* data */
};

typedef struct 
{
	string meshName;
	
	int numVertices;
	int numFaces;
	int numBones;
	bool hasNormals;
	bool hasTangentsAndBitangents;
	bool hasVertexColors;

} MeshData;

class AssimpAssetImporter 
{

public:
	AssimpAssetImporter();

	/**
	 * Read a file on disk into memory. Memory will be cleaned
	 * up when AssimpAssetImporter is destructed.
	 * 
	 * @param  path Path to the file ond disk
	 * @return      True if file was found and read correctly. 
	 */
	bool Open(const std::string& path);

	/**
	 * Get the last error from the underlying Assimp importer
	 * @return String containing the error message direct from Assimp
	 */
	const char* Error() const;

	/**
	 * Get the number of meshes found in the file.
	 * An asset must be open.
	 * @return total number of meshes
	 */
	int NumMeshes();

	/**
	 * Get the number of materials found in the file.
	 * An asset must be open.
	 * @return total number of materials
	 */
	int NumMaterials();

	/**
	 * Get the Mesh in the open file at the given index. 
	 * @param  index index between 0 and NumMeshes()-1
	 * @return       pointer to a Assimp_Mesh that references data owned by Assimp
	 */
	int GetMeshAtIndex(int index);

protected:
	Assimp::Importer importer;
	const aiScene* scene;


};


} /* namespace cgutils*/
#endif /* _IMPORT_H */