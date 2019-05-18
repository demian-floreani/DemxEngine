#include "stdafx.h"

#include "Material.h"
#include "ModelManager.h"

Demx::ModelManager::ModelManager(void)
{
}

Demx::ModelManager::~ModelManager(void)
{
	cleanup();
}

void Demx::ModelManager::addMaterial(std::string name, Demx::Material material)
{
	//check if model exists
	std::unordered_map<std::string, std::vector<Demx::Material>*>::iterator it;

	if ( ( it = materials.find( name ) ) == materials.end() ) 
	{
		std::vector<Demx::Material>* vec = new std::vector<Demx::Material>();
		vec->push_back( material );
		std::pair<std::string, std::vector<Demx::Material>*> pair( name, vec );
		materials.insert( pair );
	} 
	else 
	{
		it->second->push_back( material );
	}
}

std::vector<Demx::Material>* Demx::ModelManager::getModelMaterials(const std::string& name)
{
	std::unordered_map<std::string, std::vector<Demx::Material>*>::const_iterator it = materials.find( name );

	if(it != materials.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

GLvoid Demx::ModelManager::cleanup()
{
	for(auto &ent : this->materials) 
	{
		delete ent.second;
	}
}
