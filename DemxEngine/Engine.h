#pragma once

#include "Entity.h"
#include "Lights.h"
#include "ParticleSystem.h"
#include "WaveSimultation.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Camera.h"

#include "Window.h"

struct lua_State;

namespace Demx
{
	class Renderer;
	class Scene;

	class Engine
	{
		friend Scene;
		friend Renderer;

	private:
		PLIGHTS lights;
		std::vector<Demx::Entity*> entities;
		std::vector<Demx::ParticleSystem*> particleSystems;
		std::vector<Demx::WaveSimultation*> fluidSimultations;
		std::vector<Demx::Terrain*> terrains;

		Demx::SkyBox* skybox;

		GLboolean	winmsg;
		GLuint		msg;
		WPARAM		wParam;
		LPARAM		lParam;

		Renderer*	renderer;
		Scene*		scene;

		GL_HRESULT cleanUp();

	public:
		/* the Lua interpreter */
		lua_State* L;

	public:
		Engine( GLvoid );
		~Engine( GLvoid );

		GL_HRESULT createWindow( Demx::Window* window, LPCWSTR title, long width, long height, WNDPROC lpfnWndProc );
		GL_HRESULT createOpenGLContext();
		GL_HRESULT closeWindow( Demx::Window* window );

		GL_HRESULT init( Scene* scene );
		GL_HRESULT update();
		GL_HRESULT render();
		GL_HRESULT close();
		GL_HRESULT processInput( GLuint msg, WPARAM wParam, LPARAM lParam );
	
		GLvoid setCamera( Demx::Camera* c );
		Demx::Camera* getCamera();
	
		Demx::AmbientLight* createAmbientLight( const glm::vec3& color );

		Demx::DirectionalLight* createDirectionalLight( glm::vec3 color, glm::vec3 direction );
		Demx::DirectionalLight* createDirectionalLight( Demx::DirectionalLight* light );

		Demx::PointLight* createPointLight( glm::vec3 color, glm::vec3 position );
		Demx::PointLight* createPointLight( Demx::PointLight* light );

		Demx::SpotLight* createSpotLight( glm::vec3 color, glm::vec3 position = VEC3_ZERO, glm::vec3 direction = VEC3_ZERO, GLfloat angle = 15.0f );
		Demx::SpotLight* createSpotLight( Demx::SpotLight* light );

		Demx::Entity* createEntity( std::string name, std::string model );
		Demx::Entity* createEntity( std::string name, Demx::Entity* entity );

		Demx::ParticleSystem* createParticleSystem( GLuint particle_number );
	
		GL_HRESULT addTexture( std::string name, std::string path );

		GL_HRESULT addMaterial( std::string name, const Demx::Material& material );
		GL_HRESULT addMaterial( std::string name, std::vector<std::string> textures, VEC3 specularColor = VEC3_ZERO, GLfloat phongExp = 1.0f, GLfloat transparency = 1.0f );

		GL_HRESULT addModel( std::string name, std::string path );

		Demx::WaveSimultation* addWaveSimultation( GLuint size );
		Terrain* addTerrain( std::string path, std::string material );
		Demx::SkyBox* addSkyBox(std::string directory, std::string format, GLfloat size);

		Demx::Entity* getEntity( std::string name );
		Demx::ParticleSystem* getParticleSystem( std::string name );
		Demx::WaveSimultation* getWaveSimultation( std::string name );
		Demx::Terrain* getTerrain( std::string name );

		Demx::AmbientLight* getAmbientLight( std::string name ); 
		Demx::DirectionalLight* getDirectionalLight( std::string name ); 
		Demx::PointLight* getPointLight( std::string name ); 
		Demx::SpotLight* getSpotLight( std::string name ); 

		GLvoid setDebug( GLboolean debug );
		GLvoid resize( GLuint w, GLuint h );

		GLvoid luaManager();
	};
}