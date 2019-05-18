#include "stdafx.h"
#include "Engine.h"
#include "Renderer.h"
#include "Scene.h"
#include "GLModel.h"
#include "Parser.h"
#include "Interpreter.h"

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include "LuaManager.h"

Demx::Engine::Engine(void)
{
	this->renderer = new Renderer( this );
	this->lights = new Lights();
	this->lights->ambientLight = nullptr;

	skybox = nullptr;
	winmsg = GL_FALSE;
}

Demx::Engine::~Engine(void)
{
}

GL_HRESULT Demx::Engine::init( Scene* scene )
{
	if( atof( (const char*) glGetString( GL_VERSION ) ) < 3.3f )
	{
		MessageBox( NULL, L"This application requires OpenGL 3.3+.", L"ERROR", MB_OK | MB_ICONQUESTION );
	}

	/* initialize Lua */
	L = lua_open();
	luaL_openlibs(L);

	luaManager();

	if( scene != nullptr )
	{
		this->scene = scene;
		scene->engine = this;

		if( !scene->scripts.empty() )
		{
			Demx::Parser parser;
			Demx::Interpreter interpreter(this, &parser);

			for(	std::vector<std::string>::const_iterator script = scene->scripts.begin(); 
					script != scene->scripts.end(); 
					++script)
			{
				if(parser.lexicalAnalysis( *script ) != GL_OK)
				{
					std::cout << "error parsing" << *script;
					continue;
				}

				parser.debugParsing(std::cout);
				interpreter.interpret();
			}
		}

		if( scene->Init() != GL_OK )
		{
			return GL_FAIL;
		}

		if( !scene->luaScripts.empty() )
		{
			for(	std::vector<std::string>::const_iterator script = scene->luaScripts.begin(); 
					script != scene->luaScripts.end(); 
					++script)
			{
				/* run the script */
				luaL_dofile( L, (*script).c_str() );
			}
		}

		//this->renderer->resize( width, height );

		if( renderer->init() != GL_OK )
		{
			return GL_FAIL;
		}

		return GL_OK;
	}
	else
	{
		return GL_FAIL;
	}
}

GL_HRESULT Demx::Engine::update()
{
	if( winmsg == GL_TRUE )
	{
		this->scene->Input( this->msg, this->wParam );
		winmsg = GL_FALSE;
	}

	if( scene->Loop( renderer->timePassed ) != GL_OK )
		return GL_FAIL;

	renderer->update();

	return GL_OK;
}

GL_HRESULT Demx::Engine::render()
{
	return renderer->draw();
}

GL_HRESULT Demx::Engine::close()
{
	return cleanUp();	
}

GL_HRESULT Demx::Engine::cleanUp()
{
	for( std::size_t i = 0; i < this->entities.size(); i++ )
	{
		if( !this->entities[i]->isModel() )
		{
			glDeleteBuffers(3, entities[i]->VBO );
			glDeleteVertexArrays( 1, &entities[i]->VAO );
		}
		delete entities[i];
	}

	for( std::size_t i = 0; i < this->particleSystems.size(); i++ )
		delete particleSystems[i];
	for( std::size_t i = 0; i < this->lights->directionalLights.size(); i++ )
		delete this->lights->directionalLights[i];
	for( std::size_t i = 0; i < this->lights->pointLights.size(); i++ )
		delete this->lights->pointLights[i];
	for( std::size_t i = 0; i < this->lights->spotLights.size(); i++ )
		delete this->lights->spotLights[i];
	for( std::size_t i = 0; i < this->fluidSimultations.size(); i++ )
		delete this->fluidSimultations[i];
	for( std::size_t i = 0; i < this->terrains.size(); i++ )
		delete this->terrains[i];

	delete this->skybox;

	delete this->scene;
	delete this->renderer;

	/* cleanup Lua */
	lua_close(L);

	return GL_OK;
}

GL_HRESULT Demx::Engine::processInput( GLuint msg, WPARAM wParam, LPARAM lParam )
{
	if( msg == KEYDOWN )
	{
		winmsg = GL_TRUE;
		this->msg = msg;
		this->wParam = wParam;
		this->lParam = lParam;
	}

	return GL_OK;
}

Demx::AmbientLight* Demx::Engine::createAmbientLight( const glm::vec3& color )
{
	Demx::AmbientLight* light = new Demx::AmbientLight( color );
	this->lights->ambientLight = light;
	return light;
}

Demx::DirectionalLight* Demx::Engine::createDirectionalLight( glm::vec3 color, glm::vec3 direction )
{
	Demx::DirectionalLight* light = new Demx::DirectionalLight( color, direction );
	lights->directionalLights.push_back( light );

	return light;
}

Demx::DirectionalLight* Demx::Engine::createDirectionalLight( Demx::DirectionalLight* light )
{
	lights->directionalLights.push_back( light );

	return light;
}

Demx::PointLight* Demx::Engine::createPointLight( glm::vec3 color, glm::vec3 position )
{
	Demx::PointLight* light = new Demx::PointLight( color, position );
	lights->pointLights.push_back( light );

	return light;
}

Demx::PointLight* Demx::Engine::createPointLight( Demx::PointLight* light )
{
	lights->pointLights.push_back( light );
	return light;
}

Demx::SpotLight* Demx::Engine::createSpotLight( glm::vec3 color, glm::vec3 position, glm::vec3 direction, GLfloat angle )
{
	Demx::SpotLight* light = new Demx::SpotLight( color, position, direction, angle );
	lights->spotLights.push_back( light );

	return light;
}

Demx::SpotLight* Demx::Engine::createSpotLight( Demx::SpotLight* light )
{
	lights->spotLights.push_back( light );

	return light;
}

Demx::Entity* Demx::Engine::createEntity( std::string name, std::string model )
{
	Demx::Entity* entity = new Demx::Entity( GL_TRIANGLES );
	entity->name = name;
	entity->model = model;
	entities.push_back( entity );
	return entity;
}

Demx::Entity* Demx::Engine::createEntity( std::string name, Demx::Entity* entity )
{
	entity->name = name;
	entities.push_back( entity );
	return entity;
}

Demx::ParticleSystem* Demx::Engine::createParticleSystem( GLuint particle_number )
{
	Demx::ParticleSystem* particleSystem = new Demx::ParticleSystem( particle_number );
	particleSystems.push_back( particleSystem );
	return particleSystem;
}

Demx::Camera* Demx::Engine::getCamera()
{
	return this->renderer->cam;
}

GLvoid Demx::Engine::setCamera( Demx::Camera* c )
{
	this->renderer->cam = c;
}

GL_HRESULT Demx::Engine::addTexture( std::string name, std::string path )
{
	Demx::Texture newTexture;
	newTexture.name = name;
	newTexture.path = path;
	
	std::pair<std::string, Demx::Texture> texture ( name, newTexture );
	this->renderer->texturesMap.insert( texture );

	return GL_OK;
}

GL_HRESULT Demx::Engine::addMaterial( std::string name, const Demx::Material& material )
{
	std::pair<std::string, Demx::Material> pair ( name, material );
	this->renderer->materialsMap.insert( pair );

	return GL_OK;
}

GL_HRESULT Demx::Engine::addMaterial( std::string name, std::vector<std::string> textures, VEC3 specularColor, GLfloat phoneExp, GLfloat transparency )
{
	Demx::Material newMaterial;

	for(std::vector<std::string>::iterator it = textures.begin(); it != textures.end(); it++)
	{
		newMaterial.setTexture(*it, 1.0f);
	}

	newMaterial.setSpecularColor(specularColor);
	newMaterial.setPhongExp(phoneExp);
	newMaterial.setTransparency(transparency);

	std::pair<std::string, Demx::Material> material ( name, newMaterial );
	this->renderer->materialsMap.insert( material );

	return GL_OK;
}

GL_HRESULT Demx::Engine::addModel( std::string name, std::string path )
{
	GLModel* model = new GLModel( name, path );

	this->renderer->modelsUser.push_back( model );

	return GL_OK;
}

Demx::WaveSimultation* Demx::Engine::addWaveSimultation( GLuint size )
{
	Demx::WaveSimultation* ws = new Demx::WaveSimultation( size );
	this->fluidSimultations.push_back( ws );

	return ws;
}

Demx::Terrain* Demx::Engine::addTerrain( std::string path, std::string material )
{
	Terrain* terrain = new Terrain( path );
	terrain->setMaterial( material );
	this->terrains.push_back( terrain );

	return terrain;
}

Demx::SkyBox* Demx::Engine::addSkyBox(std::string directory, std::string format, GLfloat size)
{
	Demx::SkyBox* sky = new Demx::SkyBox( directory, format, size );

	this->skybox = sky;

	return sky;
}

Demx::Entity* Demx::Engine::getEntity( std::string name )
{
	for( std::vector<Demx::Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		if((*it)->name == name)
		{
			return (*it);
		}
	}

	for( std::vector<Demx::WaveSimultation*>::iterator it = fluidSimultations.begin(); it != fluidSimultations.end(); it++)
	{
		if((*it)->name == name)
		{
			return (*it);
		}
	}

	for( std::vector<Demx::Terrain*>::iterator it = terrains.begin(); it != terrains.end(); it++)
	{
		if((*it)->name == name)
		{
			return (*it);
		}
	}

	return nullptr;
}

Demx::ParticleSystem* Demx::Engine::getParticleSystem( std::string name )
{
	for( std::vector<Demx::ParticleSystem*>::iterator it = particleSystems.begin(); it != particleSystems.end(); it++)
	{
		if((*it)->name == name)
		{
			return (*it);
		}
	}

	return nullptr;
}

Demx::WaveSimultation* Demx::Engine::getWaveSimultation( std::string name )
{
	for( std::vector<Demx::WaveSimultation*>::iterator it = fluidSimultations.begin(); it != fluidSimultations.end(); it++)
	{
		if((*it)->name == name)
		{
			return (*it);
		}
	}

	return nullptr;
}

Demx::Terrain* Demx::Engine::getTerrain( std::string name )
{
	for( std::vector<Demx::Terrain*>::iterator it = terrains.begin(); it != terrains.end(); it++)
	{
		if((*it)->name == name)
		{
			return (*it);
		}
	}

	return nullptr;
}

Demx::AmbientLight* Demx::Engine::getAmbientLight( std::string name )
{

}

Demx::DirectionalLight* Demx::Engine::getDirectionalLight( std::string name )
{
	for( std::vector<Demx::DirectionalLight*>::iterator it = lights->directionalLights.begin(); it != lights->directionalLights.end(); it++)
	{
		if( (*it)->name == name )
			return (*it);
	}

	return nullptr;
}

Demx::PointLight* Demx::Engine::getPointLight( std::string name )
{
	for( std::vector<Demx::PointLight*>::iterator it = lights->pointLights.begin(); it != lights->pointLights.end(); it++)
	{
		if( (*it)->name == name )
			return (*it);
	}

	return nullptr;
}

Demx::SpotLight* Demx::Engine::getSpotLight( std::string name )
{

}

GLvoid Demx::Engine::setDebug( GLboolean debug )
{
	this->renderer->debug = debug;
}

GLvoid Demx::Engine::resize( GLuint w, GLuint h )
{
	renderer->resize( w, h );
}

GLvoid Demx::Engine::luaManager()
{
	lua_pushlightuserdata( L, this );
	lua_pushcclosure( L, luaCreateCube, 1 );
	lua_setglobal( L, "createCube" );

	lua_pushlightuserdata( L, this );
	lua_pushcclosure( L, luaCreateCylinder, 1 );
	lua_setglobal( L, "createCylinder" );

	lua_pushlightuserdata( L, this );
	lua_pushcclosure( L, luaCreateModelEntity, 1 );
	lua_setglobal( L, "createModelEntity" );

	lua_pushlightuserdata( L, this );
	lua_pushcclosure( L, luaTranslate, 1 );
	lua_setglobal( L, "translate" );

	lua_pushlightuserdata( L, this );
	lua_pushcclosure( L, luaRotate, 1 );
	lua_setglobal( L, "rotate" );

	lua_pushlightuserdata( L, this );
	lua_pushcclosure( L, luaScale, 1 );
	lua_setglobal( L, "scale" );

	lua_pushlightuserdata( L, this );
	lua_pushcclosure( L, luaScaleAll, 1 );
	lua_setglobal( L, "scaleAll" );
}