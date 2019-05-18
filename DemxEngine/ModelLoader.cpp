#include "stdafx.h"

#include "Material.h"
#include "ModelManager.h"

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#include "ModelLoader.h"

#include "Shlwapi.h"

Demx::ModelLoader::ModelLoader(void)
{
	hasTransparency = GL_FALSE;
}

Demx::ModelLoader::~ModelLoader(void)
{
}

GL_HRESULT Demx::ModelLoader::processModel( const std::string& path, std::string name, ModelManager* manager, std::unordered_map<std::string, Demx::Texture>& texturesMap )
{
	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile( path,  
		aiProcess_CalcTangentSpace       |  
		aiProcess_Triangulate            | 
		aiProcess_GenSmoothNormals       |
		aiProcess_JoinIdenticalVertices  | 
		aiProcess_SortByPType); 
  
	if( !scene)
	{
		return GL_FAIL;
	}

	meshes.resize(scene->mNumMeshes);

	//std::vector<GLuint> indices;

	GLuint totalIndices = 0;
	GLuint totalVertices = 0;
	
	for(std::size_t i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh* mesh = scene->mMeshes[i];

		meshes[i].materialIndex = mesh->mMaterialIndex;
		meshes[i].startIndices = totalIndices;

		GLuint numIndices = 0;

		for(std::size_t j = 0; j < mesh->mNumFaces; ++j)
		{
			aiFace face = mesh->mFaces[j];

			for(std::size_t k = 0; k < face.mNumIndices; ++k)
			{
				// add total vertices as offset
				indices.push_back( face.mIndices[k] + totalVertices );
				++numIndices;
				++totalIndices;
			}
		}

		meshes[i].numIndices = numIndices;
		totalVertices += mesh->mNumVertices;

		for(std::size_t j = 0; j < mesh->mNumVertices; ++j)
		{
			vertices.push_back( VEC3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z) );
			mappings.push_back( glm::vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y) );
			normals.push_back( glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z) );
		}
	}

	//load materials
	LPCSTR pathStr = path.c_str();
	PathRemoveFileSpecA(const_cast<char*>(pathStr));

	for( std::size_t i = 0; i < scene->mNumMaterials; i++ )
	{ 
		const aiMaterial* material = scene->mMaterials[i]; 
		aiString path;  // filename

		Demx::Material newMaterial;

		aiColor3D specularColor;
		if( material->Get( AI_MATKEY_COLOR_SPECULAR, specularColor ) == AI_SUCCESS )
		{
			newMaterial.setSpecularColor( VEC3( specularColor.r, specularColor.g, specularColor.b ) );
		}

		float phongExp;
		if( material->Get( AI_MATKEY_SHININESS, phongExp ) == AI_SUCCESS )
		{
			newMaterial.setPhongExp( phongExp );
		}

		float opacity;
		if( material->Get( AI_MATKEY_OPACITY, opacity ) == AI_SUCCESS )
		{
			if( opacity < 1.0f )
				hasTransparency = GL_TRUE;

			newMaterial.setTransparency( opacity );
		}

		if( material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS ) 
		{ 
			std::string texture = path.data; 
			std::string texturePath = std::string(pathStr) + "\\" + texture; 

			texturePath.erase(std::remove(texturePath.begin(), texturePath.end(), ':'), texturePath.end());

			Demx::Texture newTexture; 
			newTexture.path = texturePath;
			newTexture.name = texture;
			//newTexture.loadTexture(newTexture.path);

			// add texture to texture map
			std::pair<std::string, Demx::Texture> pair ( newTexture.name, newTexture );
			texturesMap.insert( pair );

			newMaterial.setTexture( newTexture.name );
		} 

		manager->addMaterial(name, newMaterial);
	} 

	return GL_OK;
}
