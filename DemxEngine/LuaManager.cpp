#include "stdafx.h"
#include "Engine.h"

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include "LuaManager.h"

int luaCreateCube( lua_State* L )
{
	// check number parameters
	if( !( lua_gettop( L ) == 4 ) )
	{
		lua_error( L );
	}

	// check parameters
	if( lua_isstring( L, 1 ) == 0 ||
		lua_isstring( L, 2 ) == 0 ||
		lua_isnumber( L, 3 ) == 0 ||
		lua_isnumber( L, 4 ) == 0 )
	{
		lua_error( L );
	}

	Demx::Engine* engine = (Demx::Engine*) lua_touserdata( L, lua_upvalueindex( 1 ) );

	Demx::Entity* entity = Demx::Entity::createCube(	std::string( lua_tostring( L, 2 ) ), 
															(GLint)lua_tonumber( L, 3 ), 
															(GLint)lua_tonumber( L, 4 ) );

	engine->createEntity( std::string( lua_tostring( L, 1 ) ), entity );

	return 0;
}

int luaCreateCylinder( lua_State* L )
{
	// check number parameters
	if( !( lua_gettop( L ) == 6 ) )
	{
		lua_error( L );
	}

	// check parameters
	if( lua_isstring( L, 1 ) == 0 ||
		lua_isstring( L, 2 ) == 0 ||
		lua_isnumber( L, 3 ) == 0 ||
		lua_isnumber( L, 4 ) == 0 ||
		lua_isnumber( L, 5 ) == 0 ||
		lua_isnumber( L, 6 ) == 0 )
	{
		lua_error( L );
	}

	Demx::Engine* engine = (Demx::Engine*) lua_touserdata( L, lua_upvalueindex( 1 ) );

	Demx::Entity* entity = Demx::Entity::createCylinder(	std::string( lua_tostring( L, 2 ) ), 
																(GLfloat)lua_tonumber( L, 3 ), 
																(GLfloat)lua_tonumber( L, 4 ), 
																(GLint)lua_tonumber( L, 5 ), 
																(GLint)lua_tonumber( L, 6 ) );
	
	engine->createEntity( std::string( lua_tostring( L, 1 ) ), entity );

	return 0;
}

int luaCreateModelEntity( lua_State* L )
{
	// check number parameters
	if( !( lua_gettop( L ) == 2 ) )
	{
		lua_error( L );
	}

	// check parameters
	if( lua_isstring( L, 1 ) == 0 ||
		lua_isstring( L, 2 ) == 0 )
	{
		lua_error( L );
	}

	Demx::Engine* engine = (Demx::Engine*) lua_touserdata( L, lua_upvalueindex( 1 ) );
	
	Demx::Entity* entity = engine->createEntity( std::string( lua_tostring( L, 1 ) ), std::string( lua_tostring( L, 2 ) ) );

	return 0;
}

int luaTranslate( lua_State* L )
{
	// check number parameters
	if( !( lua_gettop( L ) == 4 ) )
	{
		lua_error( L );
	}

	// check parameters
	if( lua_isstring( L, 1 ) == 0 ||
		lua_isnumber( L, 2 ) == 0 ||
		lua_isnumber( L, 3 ) == 0 ||
		lua_isnumber( L, 4 ) == 0 )
	{
		lua_error( L );
	}

	Demx::Engine* engine = (Demx::Engine*) lua_touserdata( L, lua_upvalueindex( 1 ) );

	engine->getEntity( std::string( lua_tostring( L, 1 ) ) )->translate( VEC3(	lua_tonumber( L, 2 ), 
																				lua_tonumber( L, 3 ), 
																				lua_tonumber( L, 4 ) ) );

	return 0;
}

int luaRotate( lua_State* L )
{
	// check number parameters
	if( !( lua_gettop( L ) == 5 ) )
	{
		lua_error( L );
	}

	// check parameters
	if( lua_isstring( L, 1 ) == 0 ||
		lua_isnumber( L, 2 ) == 0 ||
		lua_isnumber( L, 3 ) == 0 ||
		lua_isnumber( L, 4 ) == 0 ||
		lua_isnumber( L, 5 ) == 0 )
	{
		lua_error( L );
	}

	Demx::Engine* engine = (Demx::Engine*) lua_touserdata( L, lua_upvalueindex( 1 ) );

	engine->getEntity( std::string( lua_tostring( L, 1 ) ) )->rotate(	(GLfloat) lua_tonumber( L, 2 ),
																		VEC3(		
																			lua_tonumber( L, 3 ), 
																			lua_tonumber( L, 4 ), 
																			lua_tonumber( L, 5 ) 
																		) );

	return 0;
}

int luaScale( lua_State* L )
{
	// check number parameters
	if( !( lua_gettop( L ) == 4 ) )
	{
		lua_error( L );
	}

	// check parameters
	if( lua_isstring( L, 1 ) == 0 ||
		lua_isnumber( L, 2 ) == 0 ||
		lua_isnumber( L, 3 ) == 0 ||
		lua_isnumber( L, 4 ) == 0 )
	{
		lua_error( L );
	}

	Demx::Engine* engine = (Demx::Engine*) lua_touserdata( L, lua_upvalueindex( 1 ) );

	engine->getEntity( std::string( lua_tostring( L, 1 ) ) )->scale( VEC3(	lua_tonumber( L, 2 ), 
																			lua_tonumber( L, 3 ), 
																			lua_tonumber( L, 4 ) ) );

	return 0;
}

int luaScaleAll( lua_State* L )
{
	// check number parameters
	if( !( lua_gettop( L ) == 2 ) )
	{
		lua_error( L );
	}

	// check parameters
	if( lua_isstring( L, 1 ) == 0 ||
		lua_isnumber( L, 2 ) == 0 )
	{
		lua_error( L );
	}

	Demx::Engine* engine = (Demx::Engine*) lua_touserdata( L, lua_upvalueindex( 1 ) );

	engine->getEntity( std::string( lua_tostring( L, 1 ) ) )->scaleAll( (GLfloat) lua_tonumber( L, 2 ) );

	return 0;
}

int luaDebug( lua_State* L )
{
	std::cout << lua_tostring( L, 1 ) << std::endl;

	return 0;
}