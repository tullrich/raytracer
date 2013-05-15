#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <memory>
#include <map>

#include "cgutils/cgutils.hpp"
#include "entity.h"
#include "mesh.h"
#include "light.h"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

#include "glm/glm.hpp"

namespace raytracer {

using namespace cgutils;
using std::unique_ptr;

class SceneGraph; //forward declaration
class Material; //forward declaration


class AssetReader
{
public:
	virtual bool open(const std::string& path) = 0;

	/**
	 * accept a visitor that will call visitor.visit() on every entity in this resource
	 * @param visitor the visiting entity
	 */
	virtual void accept(const EntityVisitor &visitor) = 0;

	/**
	 * accept a visitor that will call visitor.visit() on every light in this resource
	 * @param visitor the visiting entity
	 */
	virtual void accept(const LightVisitor &visitor) = 0;

	/**
	 * Get the last error from the underlying Assimp importer
	 * @return String containing the error message direct from Assimp
	 */
	virtual const char* error() const = 0;
};

class ResourceLoaderFactory
{
public:
	static unique_ptr<AssetReader> getReaderForFiletype(const string &filename);
};

class AssimpAssetReader : public AssetReader
{
public:
	AssimpAssetReader();
	~AssimpAssetReader();

	/**
	 * Read a file on disk into memory. Memory will be cleaned
	 * up when AssimpAssetImporter is destructed.
	 * 
	 * @param  path Path to the file ond disk
	 * @return      True if file was found and read correctly. 
	 */
	virtual bool open(const std::string& path);

	/**
	 * Get the last error from the underlying Assimp importer
	 * @return String containing the error message direct from Assimp
	 */
	virtual const char* error() const;

	/**
	 * concrete implementation of AssetReader::accept
	 * @param visitor the visiting interface
	 */
	void accept(const EntityVisitor &visitor);

	/**
	 * concrete implementation of AssetReader::accept
	 * @param visitor the visiting interface
	 */
	void accept(const LightVisitor &visitor);


private:
	/*
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


	Material* getMaterial(const int mMaterialIndex);


	mesh_data::mesh_ptr buildMesh(const aiMesh &node);

	/**
	 * allocate an entity for the object contained at this node. Connecting any meshes and materials
	 * that it references
	 * @param  node the Assimp node containing the object data
	 * @return      fully constructed {@link Entity}
	 */
	std::shared_ptr<Entity> buildEntity(const aiNode &node, aiMatrix4x4 parentToWorldSpace);

	/**
	 * allocate a light for the aiLight contained in this file
	 * @param  ai_light the ai_light to gather the parameters from	
	 * @return      fully constructed {@link Light}
	 */
	Light::light_ptr buildLight(aiLight &ai_light);

	/**
	 * recursive assimp aiNode visitor. Allocates and adds any resources at node and 
	 * calls visitNode_r on all child nodes.
	 * @param node    current assimp aiNode to post
	 * @param visitor visiting interface
	 */
	void visitNode_r(const aiNode *node, aiMatrix4x4 worldSpace, const EntityVisitor &visitor);

protected:
	Assimp::Importer importer;
	const aiScene* scene;
	std::string open_file;


};



} /* namespace raytracer */

#endif /* _RESOURCES_H_*/
