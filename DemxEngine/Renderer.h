#pragma once

#include "stdafx.h"
#include "Camera.h"
#include "Entity.h"
#include "GLModel.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "Frustum.h"
#include "RenderQueue.h"

class Terrain;

namespace Demx
{
	class Engine;
	class ModelManager;

	class Renderer
	{
		friend Engine;
		friend Demx::Camera;

	private:
		GLboolean debug;

		Engine* pEngine;

		ModelManager* modelManager;
		Demx::ShaderManager* shaderManager;
		Demx::Frustum* camFrustum;
		Demx::RenderQueue* queue;

		//std::vector<Shadow*> shadows;

		// shader program
		Demx::Shader* program;

		// saved VAO
		GLuint savedVAO;

		// time based movement
		DWORD lastTime;
		GLfloat timePassed; 
		GLfloat timeElapsed;
		GLuint frames;

		// screen size
		GLuint width;
		GLuint height;

		// camera
		Demx::Camera* cam;

		// bias matrix
		MAT4 biasMatrix;

		// projection matrix
		MAT4 projViewMatrix;
		MAT4 projectionMatrix;
		MAT4 viewMatrix;

		// fullscreen
		GLboolean fullscreen;

		// transparent mode
		GLboolean transparent;

		std::vector<GLModel*> modelsUser;

		std::unordered_map<std::string, Demx::Texture> texturesMap;
		std::unordered_map<std::string, Demx::Material> materialsMap;

		std::vector<std::string> predefinedDepthBiasMVP;
		std::vector<std::string> predefinedTextureContributions;

	public:
		Renderer( Engine* );
		~Renderer( GLvoid );

		GLvoid update();
		GL_HRESULT resize(GLuint width, GLuint height);
		GL_HRESULT init();
		GL_HRESULT draw();
		GL_HRESULT cleanup();

	private:

		GLvoid setPredefinedShaderAttributes();

		GL_HRESULT createDepthTexture( GLuint& buffer, GLuint& texture, GLsizei resolution );
		GL_HRESULT createCubeDepthTexture( GLuint& buffer, GLuint& texture, GLuint& cubeMap, GLsizei resolution );
	
		GL_HRESULT drawShadowMapTexture( const Shadow* shadow );
		GL_HRESULT drawShadowCubeMapTexture( const Shadow* shadow, GLenum textarget );

		GLvoid setShader( const std::string& shader );

		GLvoid setRenderTexture( Demx::Shader* shader, const std::unordered_map<std::string, GLfloat>& textures );
		GL_HRESULT setRenderMaterial( Demx::Shader* shader, const Demx::Material& material );
	
		GLvoid setTransparentMode( GLboolean isTransparent );
		GLboolean transparentMode();
		GLvoid enableTransparentMode();
		GLvoid disableTransparentMode();

		GLvoid bindVAO( GLuint VAO );

		GLvoid addObjectsRenderQueue( Demx::RenderQueue* queue );
		GLvoid addObjectsShadowRenderQueue( const Demx::Frustum* frustum, const VEC3& pos, Demx::RenderQueue* queue );

		GLvoid drawRenderQueue( const std::deque<Demx::Entity*>& entities );
		GLvoid drawShadowRenderQueue( const MAT4* depthMatrix, const std::deque<Demx::Entity*>& entities );

		GL_HRESULT bindShadowTextures();
	};
}