#include "stdafx.h"

#include "Shader.h"
#include "Entity.h"

/** 
*	@brief Constructor for entity
* 
*	@author Demian Floreani
*
*	@param[in] drawType How to draw the entity. GL_TRIANGLES is the default method.
*
*	@return None
*/
Demx::Entity::Entity( GLuint drawType )
{
	VAO = 0;
	IB = 0;
	distance = 0.0f;
	hasTransparency = GL_FALSE;
	resetModelMatrix();

	this->drawType = drawType;
	model = "";
	name = "";
	shader = "main";
	glModel = nullptr;
	castShadow = GL_TRUE;
}

Demx::Entity::~Entity()
{
}

/** 
*	@brief Query if entity is a model
* 
*	@author Demian Floreani
*
*	@return GLboolean If true the entity is a loaded model
*/
GLboolean Demx::Entity::isModel()
{
	return glModel != nullptr;
}

/** 
*	@brief Query if entity uses indexed drawing
* 
*	@author Demian Floreani
*
*	@return GLboolean If true indexed drawing is used
*/
GLboolean Demx::Entity::isIndexed()
{
	return indices.size() != 0;
}

/** 
*	@brief Set if the entity casts a shadow
* 
*	@author Demian Floreani
*
*	@param[in] castShadow If set to true the entity casts a shadow
*
*	@return None
*/
GLvoid Demx::Entity::setCastShadow( GLboolean castShadow )
{
	this->castShadow = castShadow;
}

/** 
*	@brief Query if entity is casting a shadow
* 
*	@author Demian Floreani
*
*	@return GLboolean If true entity is casting a shadow
*/
GLboolean Demx::Entity::isCastingShadow()
{
	return castShadow;
}

/** 
*	@brief Specify what happens before drawing the entity. This can be overloaded by derived classes.
* 
*	@author Demian Floreani
*
*	@param[in] Shader* A pointer to a shader program
*
*	@return None
*/
GLvoid Demx::Entity::onStartDraw( Demx::Shader* program )
{
}

/** 
*	@brief Specify what happens after drawing the entity. This can be overloaded by derived classes.
* 
*	@author Demian Floreani
*
*	@return None
*/
GLvoid Demx::Entity::onEndDraw()
{
}

/** 
*	@brief Add a position vertex to the user defined mesh.
* 
*	@author Demian Floreani
*
*	@param[in] position VEC3 representing a 3D vertex
*
*	@return None
*/
GLvoid Demx::Entity::position( VEC3 position )
{
	this->vertices.push_back( position );
}

/** 
*	@brief Add a texture mapping to the user defined mesh.
* 
*	@author Demian Floreani
*
*	@param[in] mapping VEC2 representing a texture coordinate
*
*	@return None
*/
GLvoid Demx::Entity::map( VEC2 mapping )
{
	this->mappings.push_back( mapping );
}

/** 
*	@brief Add a normal to the user defined mesh.
* 
*	@author Demian Floreani
*
*	@param[in] normal VEC3 representing a normal vector
*
*	@return None
*/
GLvoid Demx::Entity::normal( VEC3 normal )
{
	this->normals.push_back( normal );
}

/** 
*	@brief Add a triangle to the user defined mesh. The Normals of the triangle will be also calculated.
* 
*	@author Demian Floreani
*
*	@param[in] v1 First triangle
*	@param[in] v2 Second triangle
*	@param[in] v3 Third triangle
*
*	@return None
*/
GLvoid Demx::Entity::triangle( VEC3 v1, VEC3 v2, VEC3 v3 )
{
	this->position( v1 );
	this->position( v2 );
	this->position( v3 );

	// calculate the normal of the triangle
	VEC3 triangleNormal = NORMALIZE( glm::cross( v2 - v1, v3 - v1 ) );

	for( int i = 0; i < 3; ++i )
		this->normal( triangleNormal );
}

/** 
*	@brief Set the normal for a triangle face. Only works when drawing in triangle mode.
* 
*	@author Demian Floreani
*
*	@param[in] normal VEC3 representing a nomal vector.
*
*	@return None
*/
GLvoid Demx::Entity::setFaceNormal( VEC3 normal )
{
	switch( this->drawType )
	{
		case GL_TRIANGLES:
			for( GLuint i = 0; i < 3; ++i )
				this->normal( normal );
		return;

		case GL_TRIANGLE_STRIP:
			for( GLuint i = 0; i < 4; ++i )
				this->normal( normal );
		return;
	}
}

/** 
*	@brief Set a material.
* 
*	@author Demian Floreani
*
*	@param[in] material Name of the material.
*
*	@return None
*/
GLvoid Demx::Entity::setMaterial( std::string material )
{
	this->material = material;
}

/** 
*	@brief Move the entity
* 
*	@author Demian Floreani
*
*	@param[in] destination A VEC3 specifying the destination position
*   @param[in] speed The speed of movement
*   @param[in] timeDelta The time delta to allow time based movement
*   
*	@return None
*/
GLvoid Demx::Entity::move( const VEC3& destination, GLfloat speed, GLfloat timeDelta )
{
	VEC3 direction = NORMALIZE( destination - this->getPosition() );

	VEC3 pos = this->getPosition();

	if( glm::distance( destination, this->getPosition() ) >= 0.1f )
	{
		this->translate( direction * speed * timeDelta ); 
	}
}

/** 
*	@brief Set the entity name
* 
*	@author Demian Floreani
*
*	@param[in] name String
*   
*	@return None
*/
GLvoid Demx::Entity::setName( const std::string& name )
{
	this->name = name;
}

/** 
*	@brief Reset the model matrix to an identity matrix
* 
*	@author Demian Floreani
*
*	@return None
*/
GLvoid Demx::Entity::resetModelMatrix()
{
	modelMatrix = MAT4( 1.0f );
}

/** 
*	@brief Translate the entity by an amount specified by a 3D vector
* 
*	@author Demian Floreani
*
*	@param[in] vector The amount to translate
*   
*	@return None
*/
GLvoid Demx::Entity::translate( const VEC3& vector )
{
	modelMatrix = glm::translate( modelMatrix, vector );
}

/** 
*	@brief Rotate the entity by an amount specified by a 3D vector
* 
*	@author Demian Floreani
*
*	@param[in] angle The amount in degrees to rotate
*   @param[in] axis The axis around which to rotate
*
*	@return None
*/
GLvoid Demx::Entity::rotate( GLfloat angle, const VEC3& axis )
{
	modelMatrix = glm::rotate( modelMatrix, angle, axis );
}

/** 
*	@brief Scale the entity by an amount specified by a 3D vector
* 
*	@author Demian Floreani
*
*	@param[in] vector The amount to scale
*   
*	@return None
*/
GLvoid Demx::Entity::scale( const VEC3& vector )
{
	modelMatrix = glm::scale( modelMatrix, vector );
}

/** 
*	@brief Scale the entity by an amount specified by a float
* 
*	@author Demian Floreani
*
*	@param[in] value The amount to scale
*   
*	@return None
*/
GLvoid Demx::Entity::scaleAll( GLfloat value )
{
	modelMatrix = glm::scale( modelMatrix, VEC3( value, value, value ) );
}

/** 
*	@brief Set the position of the entity in world coordinates
* 
*	@author Demian Floreani
*
*	@param[in] pos VEC3 representing a position
*   
*	@return None
*/
GLvoid Demx::Entity::setPosition( const VEC3& pos )
{
	modelMatrix[3][0] = pos.x;
	modelMatrix[3][1] = pos.y;
	modelMatrix[3][2] = pos.z;
}

/** 
*	@brief Get the position of the entity in world coordinates
* 
*	@author Demian Floreani
*
*	@return VEC3 The position of the entity
*/
VEC3 Demx::Entity::getPosition()
{
	return VEC3( modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2] );
}

/** 
*	@brief Get the entity's current model matrix
* 
*	@author Demian Floreani
*
*	@return MAT4 The model matrix
*/
MAT4 Demx::Entity::getModelMatrix()
{
	return this->modelMatrix;
}

/** 
*	@brief Get the entity's current normal matrix. This is computed as the transpose inverse of the model matrix.
* 
*	@author Demian Floreani
*
*	@return MAT4 The normal matrix
*/
MAT4 Demx::Entity::getNormalMatrix()
{
	return glm::transpose( glm::inverse( this->modelMatrix ) );
}

/** 
*	@brief Setup the entity's Axis Aligned Bounding Box
* 
*	@author Demian Floreani
*
*	@param[in] vertices The vertices in 3D to use to setup the AABB
*   
*	@return None
*/
GLvoid Demx::Entity::setupAABB( const std::vector<VEC3>& vertices )
{
	box.setup( vertices );
}

/** 
*	@brief Setup the entity's Axis Aligned Bounding Box
* 
*	@author Demian Floreani
*
*	@param[in] vertices The vertices in 4D to use to setup the AABB
*   
*	@return None
*/
GLvoid Demx::Entity::setupAABB( const std::vector<VEC4>& vertices )
{
	box.setup( vertices );
}