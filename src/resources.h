#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <memory>

#include "scenegraph.h"
#include "cgutils/cgutils.hpp"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

namespace raytracer {

using namespace cgutils;
using std::unique_ptr;

typedef struct 
{
	string meshName;
	
	int numVertices;
	int numFaces;
	int numBones;
	bool hasNormals;
	bool hasTangentsAndBitangents;
	bool hasVertexColors;

} mesh_data;

typedef Visitor<mesh_data> MeshDataVisitor;

class AssetReader
{
public:
	virtual bool open(const std::string& path) = 0;

	virtual void accept(const MeshDataVisitor &visitor) = 0;
};

class ResourceLoaderFactory
{
public:
	static unique_ptr<AssetReader> getReaderForFiletype(const string &filename);
};

class AssimpAssetImporter : public AssetReader
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
	bool open(const std::string& path);

	/**
	 * Get the last error from the underlying Assimp importer
	 * @return String containing the error message direct from Assimp
	 */
	const char* error() const;

	/**
	 * Get the number of meshes found in the file.
	 * An asset must be open.
	 * @return total number of meshes
	 */
	int numMeshes();

	/**
	 * Get the number of materials found in the file.
	 * An asset must be open.
	 * @return total number of materials
	 */
	int numMaterials();

	/**
	 * Get the Mesh in the open file at the given index. 
	 * @param  index index between 0 and NumMeshes()-1
	 * @return       pointer to a Assimp_Mesh that references data owned by Assimp
	 */
	int getMeshAtIndex(int index);


	void accept(const MeshDataVisitor &visitor)
;
protected:
	Assimp::Importer importer;
	const aiScene* scene;


};

class MeshDataSceneAdder : public MeshDataVisitor
{
public:
	MeshDataSceneAdder(SceneGraph *s) : scene(s) { };
	virtual void visit(mesh_data &mesh);

private:
	SceneGraph *scene;
};

} /* namespace raytracer */

#endif /* _RESOURCES_H_*/
