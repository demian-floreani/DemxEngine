#pragma once

#include "AABB.h"
#include "Material.h"
#include "Mesh.h"
#include "GLModel.h"

namespace Demx
{
	class Shader;

	/** 
	*	@brief Represent a scene object. This can be a user defined or loaded model.
	*/
	class Entity
	{
	public:
		// Entity
		std::string name;

		GLuint VAO;
		GLuint* VBO;
		GLuint IB;

		MAT4 modelMatrix;

		// AABB
		Demx::AABB box;

		// distance to camera
		GLfloat distance;

		GLboolean hasTransparency;

		// draw type
		GLuint						drawType;

		// vertex info
		std::vector<glm::vec3>		vertices;
		std::vector<glm::vec2>		mappings;
		std::vector<glm::vec3>		normals;
		std::vector<GLint>			indices;

		// shader program info
		std::string					shader;

		// material info
		std::string					material;

		// does entity cast a shadow
		GLboolean					castShadow;

		// model info
		std::string					model;
		GLModel*					glModel;

		// direction
		VEC3						direction;

	public:
		Entity( GLuint drawType = GL_TRIANGLES );
		~Entity();

		void setName( const std::string& name ); 

		void translate( const VEC3& vector );
		void rotate( GLfloat angle, const VEC3& axis );
		void scale( const VEC3& vector );
		void scaleAll( GLfloat value );
		void resetModelMatrix();

		MAT4 getModelMatrix();
		MAT4 getNormalMatrix();

		GLvoid setPosition( const VEC3& pos );
		VEC3 getPosition();

		GLvoid setupAABB( const std::vector<VEC3>& vertices );
		GLvoid setupAABB( const std::vector<VEC4>& vertices );

		GLboolean isModel();
		GLboolean isIndexed();

		GLvoid setCastShadow( GLboolean castShadow );
		GLboolean isCastingShadow();

		virtual GLvoid onStartDraw( Demx::Shader* program );
		virtual GLvoid onEndDraw();

		GLvoid position( VEC3 position );
		GLvoid map( VEC2 mapping );
		GLvoid normal( VEC3 normal );
		GLvoid triangle( VEC3 v1, VEC3 v2, VEC3 v3 );
		GLvoid setFaceNormal( VEC3 normal );
		GLvoid setMaterial( std::string material );

		GLvoid move( const VEC3& destination, GLfloat speed, GLfloat timeDelta );
	
		static Entity* createCube( std::string material, GLint width_mapping = 1.0f, GLint height_mapping = 1.0f );
		static Entity* createPlane( std::string material, GLfloat size, GLint w, GLint h );
		static Entity* createPyramid( std::string material, GLint w, GLint h );
		static Entity* createCircle( std::string material, GLfloat radius, GLint w, GLint h );
		static Entity* createCylinder( std::string material, GLfloat radius, GLfloat height, GLint w, GLint h );
		static Entity* createCone( std::string material, GLfloat radius, GLfloat height, GLint w, GLint h );
		static Entity* createSphere( std::string material, GLfloat radius, GLfloat height, GLint w, GLint h );
	};
}
