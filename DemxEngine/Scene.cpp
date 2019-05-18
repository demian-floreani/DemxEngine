#include "stdafx.h"

#include "Engine.h"
#include "Scene.h"

Demx::Scene::Scene()
{
}

Demx::Scene::~Scene(void)
{
}

/** 
 *	@brief Add a script to the scene.
 * 
 *	@author Demian Floreani
 *
 *	@param[in] path path to the script

 *	@return None
 */
void Demx::Scene::addScript( const std::string& path )
{
	scripts.push_back( path );
}

/** 
 *	@brief Add a Lua script to the scene.
 * 
 *	@author Demian Floreani
 *
 *	@param[in] path path to the lua script

 *	@return None
 */
void Demx::Scene::addLuaScript( const std::string& path )
{
	luaScripts.push_back( path );
}

/** 
 *	@brief Set the scene camera
 * 
 *	@author Demian Floreani
 *
 *	@param[in] c Camera for the scene

 *	@return None
 */
void Demx::Scene::setCamera( Demx::Camera* c )
{
	engine->setCamera( c );
}