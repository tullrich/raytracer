#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <memory>
#include <map>

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

	/**
	 * accept a visitor that will call visitor.visit() on every entity in this resource
	 * @param visitor the visiting entity
	 */
	virtual void accept(const MeshDataVisitor &visitor) = 0;

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
	void accept(const MeshDataVisitor &visitor);


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

	/**
	 * recursive assimp aiNode visitor. Allocates and adds any resources at node and 
	 * calls visitNode_r on all child nodes.
	 * @param node    current assimp aiNode to post
	 * @param visitor visiting interface
	 */
	void visitNode_r(const aiNode *node, const MeshDataVisitor &visitor);
;
protected:
	Assimp::Importer importer;
	const aiScene* scene;
	std::string open_file;


};

class MeshDataSceneAdder : public MeshDataVisitor
{
public:
	MeshDataSceneAdder(SceneGraph *s) : scene(s) { };
	virtual void visit(mesh_data &mesh);

private:
	SceneGraph *scene;
};


template <typename T>
class ResourceManager : public Singleton<ResourceManager<T>>
{
private:
	typedef std::map<int, std::shared_ptr<T>> entity_map;

public:

	/**
	 * Adds a resource of type T into this manager.
	 * @param r resource instance to be added
	 */
	void addResource(std::string &key, T &r)
	{
		int hash = 0;

		hash = calcHash(key);
		if (resources.find(hash) == entity_map::end)
		{
			std::cout << "Warning: trying to add duplicate mesh " << r << std::endl;
			return;
		}

		std::cout << " hash('" << key << "') = " << hash << std::endl;
		resources[hash] = r;
	}

	/**
	 * fnv1 hash (http://isthe.com/chongo/tech/comp/fnv/) using recommended 
	 * seed and prime
	 * @param  str [description]
	 * @return     [description]
	 */
	int calcHash(const std::string &str)
	{
		int prime = 0x01000193; //   16777619
		int hash = 0x811C9DC5; // the seed - 2166136261

		for (char c : str)
		{
			hash = (c ^ hash) * prime;
		}

		return hash;
	}

private:
	entity_map resources;

};

class  MeshManager : ResourceManager<mesh_data> {};

} /* namespace raytracer */

#endif /* _RESOURCES_H_*/
