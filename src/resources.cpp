#include <iostream>
#include <cstring>
#include <regex>

#include "common.h"
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
        aiProcess_SortByPType 			 );

	if(scene == NULL)
	{
		// Assimp couldnt read the file
		return false;
	}

	open_file = path;
	parent_path = parentPath(open_file);
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

	//std::cout << "num cams " << scene->mNumCameras << std::endl;
	//std::cout << "num lights " << scene->mNumLights << std::endl;
	
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

Light::light_ptr AssimpAssetReader::buildLight(aiLight &ai_light)
{
	Light::light_ptr light;//(new Light());
	std::cout << "light type " << ai_light.mType << std::endl;

	std::string name = ai_light.mName.C_Str();


	switch(ai_light.mType)
	{
		case aiLightSource_DIRECTIONAL : 
		{
			light =  Light::light_ptr(new DirectionalLight(name)); 
			break;
		}
		case aiLightSource_POINT : 
		{
			light =  Light::light_ptr(new PointLight(name)); 
			break;
		}
		case aiLightSource_SPOT : 
		{
			light =  Light::light_ptr(new SpotLight(name, ai_light.mAngleInnerCone, ai_light.mAngleOuterCone)); 
			break;
		}
		default: light =  Light::light_ptr(NULL);
	}

	if (light)
	{

		// get colors as RGB
		RGB diffuse(ai_light.mColorDiffuse.r, ai_light.mColorDiffuse.g, ai_light.mColorDiffuse.b);
		RGB ambient(ai_light.mColorAmbient.r, ai_light.mColorAmbient.g, ai_light.mColorAmbient.b);
		RGB specular(ai_light.mColorSpecular.r, ai_light.mColorSpecular.g, ai_light.mColorSpecular.b);

		aiNode *n = scene->mRootNode->FindNode(ai_light.mName);
		aiMatrix4x4 childToWorld; // identity

		if (n != NULL)
		{
			computeTransformForNode(n, childToWorld);
		}

		aiVector3D worldPosition = childToWorld * ai_light.mPosition;
		aiVector3D worldDirection = childToWorld * ai_light.mDirection;

		// get location as vec3
		glm::vec3 position(worldPosition.x, worldPosition.y, worldPosition.z);
		glm::vec3 direction(worldDirection.x, worldDirection.y, worldDirection.z);

		std::cout << " light position " << position << std::endl;

		light->setColor(ambient, diffuse, specular);
		light->setLocation(position, direction);
		light->setAttenuation(ai_light.mAttenuationConstant, ai_light.mAttenuationLinear, ai_light.mAttenuationQuadratic);
	}

	return light;
}

void AssimpAssetReader::accept(const LightVisitor &visitor)
{
	ASSERT_ASSET_OPEN()

	for (int i = 0; i < scene->mNumLights; ++i)
	{
		aiLight *ai_light = scene->mLights[i];
		Light::light_ptr light = buildLight(*ai_light);

		if (light != NULL)
		{
			visitor.visit(light);
		}

	}
}

glm::vec3* vertBufferForAiVector3D(int mNumVertices, aiVector3D *verts, aiMatrix4x4 &worldSpace)
{
	glm::vec3 *buf = new glm::vec3[mNumVertices];
	glm::mat4 modelToWorld(0.0f);
	modelToWorld[0][0] = worldSpace.a1;
	modelToWorld[0][1] = worldSpace.b1;
	modelToWorld[0][2] = worldSpace.c1;
	modelToWorld[0][3] = worldSpace.d1;
	modelToWorld[1][0] = worldSpace.a2;
	modelToWorld[1][1] = worldSpace.b2;
	modelToWorld[1][2] = worldSpace.c2;
	modelToWorld[1][3] = worldSpace.d2;
	modelToWorld[2][0] = worldSpace.a3;
	modelToWorld[2][1] = worldSpace.b3;
	modelToWorld[2][2] = worldSpace.c3;
	modelToWorld[2][3] = worldSpace.d3;
	modelToWorld[3][0] = worldSpace.a4;
	modelToWorld[3][1] = worldSpace.b4;
	modelToWorld[3][2] = worldSpace.c4;
	modelToWorld[3][3] = worldSpace.d4;

	//std::cout << "MESH MATRIX " << modelToWorld << std::endl;

	for (int i = 0; i < mNumVertices; i++)
	{
		buf[i] = glm::vec3(modelToWorld * glm::vec4(verts[i].x, verts[i].y, verts[i].z, 1));
		//std::cout << "vert (" << buf[i].x << "," << buf[i].y << "," << buf[i].z << ") was " << verts[i].x << " " <<  verts[i].y << " " <<  verts[i].z << std::endl;
	}

	return buf;
}

glm::vec2* uvBufferForAiVector3D(int mNumVertices, aiVector3D *verts)
{
	glm::vec2 *buf = new glm::vec2[mNumVertices];

	for (int i = 0; i < mNumVertices; i++)
	{
		buf[i] = glm::vec2(verts[i].x, verts[i].y);
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

Texture* AssimpAssetReader::getTexture(aiTextureType type, const aiMaterial &ai_mat)
{
	Texture *t = NULL;

	if (ai_mat.GetTextureCount(type) > 0)
	{
		aiString tex_name;
		ai_mat.GetTexture(type, 0, &tex_name, NULL, NULL, NULL, NULL, NULL);

		if (tex_name.length)
		{
			std::string filename = appendFilename(parent_path, tex_name.C_Str());

			t = TextureManager::getInstance().get(filename);
			if(!t)
			{
				t = new Texture(filename);
				if(t->load())
				{
					std::cout << "adding t " << filename << std::endl; 
					TextureManager::getInstance().add(filename, *t);
				}
			}
			else
			{
				std::cout << "found t " << filename << std::endl; 
			}
		}
	}

	return t;
}

bool AssimpAssetReader::usesTextures(const aiMaterial &ai_mat)
{
	return ai_mat.GetTextureCount(aiTextureType_DIFFUSE) + 
		ai_mat.GetTextureCount(aiTextureType_SPECULAR) + 
		ai_mat.GetTextureCount(aiTextureType_EMISSIVE) + 
		ai_mat.GetTextureCount(aiTextureType_NORMALS);
}

Material* AssimpAssetReader::getMaterial(const int mMaterialIndex)
{
	Material *m = MaterialManager::getInstance().get(mMaterialIndex);

	if (m == NULL)
	{
		const aiMaterial &ai_mat = *(scene->mMaterials[mMaterialIndex]);

		aiString name;
		ai_mat.Get(AI_MATKEY_NAME, name);
		
		if (usesTextures(ai_mat))
		{
			// this material has some textures, set them up
			TextureMaterial *tex_m = new TextureMaterial(name.C_Str());

			tex_m->setDiffuseTexture(getTexture(aiTextureType_DIFFUSE, ai_mat));
			tex_m->setSpecularTexture(getTexture(aiTextureType_SPECULAR, ai_mat));
			tex_m->setEmissiveTexture(getTexture(aiTextureType_EMISSIVE, ai_mat));
			tex_m->setNormalTexture(getTexture(aiTextureType_NORMALS, ai_mat));
			
			m = tex_m;
		}
		else
		{
			m = new Material(name.C_Str());
		}

		getAiMatColor(ai_mat, AI_MATKEY_COLOR_DIFFUSE, m->diffuse);
		getAiMatColor(ai_mat, AI_MATKEY_COLOR_AMBIENT, m->ambient);
		getAiMatColor(ai_mat, AI_MATKEY_COLOR_SPECULAR, m->specular);
		getAiMatColor(ai_mat, AI_MATKEY_COLOR_EMISSIVE, m->emissive);
		getAiMatFloat(ai_mat, AI_MATKEY_SHININESS, m->shineness);

		MaterialManager::getInstance().add(mMaterialIndex, *m);
	}

	return m;
}

mesh_data::mesh_ptr AssimpAssetReader::buildMesh(const aiNode &node, int mMeshes_index, aiMatrix4x4 &worldSpace)
{
	glm::vec2 *uvs   = NULL;
	glm::vec3 *verts = NULL;
	prim_tri *faces  = NULL;
	Material *mat    = NULL;

	unsigned int mesh_index = node.mMeshes[mMeshes_index];
	aiMesh *ai_mesh = scene->mMeshes[mesh_index];

	// copy the verts to memory managed by us
	verts = vertBufferForAiVector3D(ai_mesh->mNumVertices, ai_mesh->mVertices, worldSpace);
	// copy the face indices to memory managed by us
	faces = faceBufferForAiFace(ai_mesh->mNumFaces, ai_mesh->mFaces);
	// create the material
	mat = getMaterial(ai_mesh->mMaterialIndex);

	// create the mesh
	mesh_data::mesh_ptr pMesh;
	if(ai_mesh->GetNumUVChannels() > 0 && ai_mesh->mNumUVComponents[0] == 2) 
	{
		std::cout << "got a mesh with uvs" << std::endl; 
		// we only support 2 component texture lookups on texture 0
		textured_mesh_data *texture_mesh = new textured_mesh_data();
		// copy the verts to memory managed by us
		uvs = uvBufferForAiVector3D(ai_mesh->mNumVertices, ai_mesh->mTextureCoords[0]);
		texture_mesh->setUVs(uvs);
		pMesh = mesh_data::mesh_ptr(texture_mesh);
	}
	else
	{	
		std::cout << "got a mesh without uvs" << std::endl; 
		// allocate and set name
		pMesh = mesh_data::mesh_ptr(new mesh_data());
	}
	pMesh->setName("" + mesh_index); // TODO: FIXME
	pMesh->setVertices(ai_mesh->mNumVertices, verts);
	pMesh->setFaces(ai_mesh->mNumFaces, faces);
	pMesh->setMaterial(mat);

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
		
		mesh = buildMesh(node, i, worldSpace);
		entity->addMeshComponent(mesh);
	}

	return entity;
}


void AssimpAssetReader::visitNode_r(const aiNode *node, aiMatrix4x4 parentToWorldSpace, const EntityVisitor &visitor)
{
	aiNode *child = NULL;
	Entity::entity_ptr ent = NULL;
	aiMatrix4x4 worldSpace;
	
	worldSpace = node->mTransformation * parentToWorldSpace;
	
	//compute the world space transformation matrix for this model
	if(node->mParent != NULL)
	{
		//std::cout << "visiting node " << node->mName.C_Str() << " parent "  << std::endl;
	}

	/*std::cout << worldSpace.a1 << " " << worldSpace.a2 << " " << worldSpace.a3 << std::endl;
	std::cout << worldSpace.b1 << " " << worldSpace.b2 << " " << worldSpace.b3 << std::endl;
	std::cout << worldSpace.c1 << " " << worldSpace.c2 << " " << worldSpace.c3 << std::endl;
	std::cout << worldSpace.d1 << " " << worldSpace.d2 << " " << worldSpace.d3 << std::endl;*/

	if(node->mNumMeshes > 0)
	{
		ent = buildEntity(*node, worldSpace);
		visitor.visit(ent);
	}

	// visit each child node of cur_node
	for (int i = 0; i < node->mNumChildren; i++)
	{
		child = node->mChildren[i];
		visitNode_r(child, worldSpace, visitor);
	}
}

void AssimpAssetReader::computeTransformForNode(aiNode *node, aiMatrix4x4 &childToWorld) const
{
	childToWorld = childToWorld * node->mTransformation;

	if (node != scene->mRootNode)
	{
		computeTransformForNode(node->mParent, childToWorld);
	}
}

Camera* AssimpAssetReader::getCamera() const
{
	if(scene->mNumCameras > 0)
	{
		aiCamera *ai_cam = scene->mCameras[0];

		aiNode *n = scene->mRootNode->FindNode(ai_cam->mName);
		aiMatrix4x4 childToWorld; // identity

		if (n != NULL)
		{
			computeTransformForNode(n, childToWorld);
		}

		aiVector3D worldPosition = childToWorld * ai_cam->mPosition;
		aiVector3D worldDirection = childToWorld * ai_cam->mLookAt;
		aiVector3D worldUp = childToWorld * ai_cam->mUp;

		glm::vec3 position(worldPosition.x, worldPosition.y, worldPosition.z);
		glm::vec3 direction(worldDirection.x, worldDirection.y, worldDirection.z);
		glm::vec3 up(worldUp.x, worldUp.y, worldUp.z);

		return new Camera(position, direction, up, ai_cam->mClipPlaneNear, ai_cam->mClipPlaneFar,  ai_cam->mHorizontalFOV, ai_cam->mAspect);
	}

	return NULL;
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
	else if (std::regex_match (filename, std::regex(".*\\.dae") ))
	{
		return unique_ptr<AssimpAssetReader>(new AssimpAssetReader());
	}
	else if (std::regex_match (filename, std::regex(".*\\.blend") ))
	{
		return unique_ptr<AssimpAssetReader>(new AssimpAssetReader());
	}
	else
	{
		return NULL;
	}
}


} /* namespace raytracer */
