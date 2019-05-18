#include "stdafx.h"
#include "Terrain.h"

#include <fstream>
#include <iostream>

/** 
*	@brief Constructor for Terrain class
*
*	@author Demian Floreani
*
*   @return None
*/
Demx::Terrain::Terrain( std::string path ) : Entity( GL_TRIANGLE_STRIP )
{
	setHeightMap( path );
}

Demx::Terrain::~Terrain(void)
{
}

/** 
*	@brief Initialize the terrain.
*
*	@author Demian Floreani
*
*   @return GL_HRESULT GL_OK if initialization is successful
*/
GL_HRESULT Demx::Terrain::init()
{
	std::ifstream hmFile;
	hmFile.open( heightMap );
	
	std::vector<GLfloat> fileInput;
	
	char output[10];
	if ( hmFile.is_open() ) 
	{
		while ( !hmFile.eof() ) 
		{
			hmFile >> output;
			fileInput.push_back( (GLfloat) atof( output ) );
		}
	}

	hmFile.close();

	// set the terrain size based on heightmap
	gridX = gridZ = (GLuint) glm::sqrt( (GLfloat)fileInput.size() );
	sizeX = sizeZ = (GLfloat) gridX;

	float* heights = new float[ gridX * gridZ ];
	GLuint i = 0, j = 0;
	for(	std::vector<GLfloat>::const_iterator input = fileInput.begin();
			input != fileInput.end();
			++input )
	{
		heights[ i * gridZ + j ] = (*input) / 2.0f;

		j++;
		if( j > gridX - 1 )
		{
			i++;
			j = 0;
		}
	}

	// set the terrain vertex positions
	for( GLuint i = 0; i < gridZ; i++ ) 
	{ 
		for( GLuint j = 0; j < gridX; j++ ) 
		{
			vertices.push_back(
				VEC3(	-sizeX / 2 + j * sizeX / ( gridX - 1 ),
						heights[ i * gridZ + j ],
						-sizeZ / 2 + i * sizeZ / ( gridZ - 1 ) )
			);
		}
	} 

	// set the terrain texture coordinates
	for( GLuint i = 0; i < gridZ; i++ )
	{
		for( GLuint j = 0; j < gridX; j++ )
		{
			mappings.push_back( VEC2(	GLfloat(gridX) * GLfloat(j) / GLfloat( gridX - 1 ), 
										GLfloat(gridZ) * GLfloat(i) / GLfloat( gridZ - 1 ) ) );
		}
	}

	glm::vec3*** tmpNormals;
	tmpNormals = new glm::vec3**[2];
	tmpNormals[0] = new glm::vec3*[ gridZ - 1 ];
	tmpNormals[1] = new glm::vec3*[ gridZ - 1 ];

	for( GLuint i = 0; i < gridZ; i++ )
	{
		tmpNormals[0][i] = new glm::vec3 [ gridX ];
	}
	for( GLuint i = 0; i < gridZ; i++ )
	{
		tmpNormals[1][i] = new glm::vec3 [ gridX ];
	}

	/*for( GLuint i = 0; i < gridZ - 1; i++ )
	{
		for( GLuint j = 0; j < gridX - 1; j++ )
		{
			glm::vec3 triangle0[3];
			
			triangle0[0] = vertices[ i * gridZ + j ];
			triangle0[1] = vertices[ ( i + 1 ) * gridZ + j ];
			triangle0[2] = vertices[ ( i + 1 ) * gridZ + ( j + 1 ) ];

			glm::vec3 triangle1[3];

			triangle1[0] = vertices[ ( i + 1 ) * gridZ + ( j + 1 ) ];
			triangle1[1] = vertices[ i * gridZ + ( j + 1 ) ];
			triangle1[2] = vertices[ i * gridZ + j ];

			tmpNormals[0][i][j] = glm::normalize( glm::cross( triangle0[0] - triangle0[1], triangle0[1] - triangle0[2] ) );
			tmpNormals[1][i][j] = glm::normalize( glm::cross( triangle1[0] - triangle1[1], triangle1[1] - triangle1[2] ) );
		}
	}*/

	for( GLuint i = 0; i < gridZ - 1; i++ )
	{
		for( GLuint j = 0; j < gridX - 1; j++ )
		{
			glm::vec3 triangle0[3];
			
			triangle0[0] = vertices[ i * gridZ + j ];
			triangle0[1] = vertices[ ( i + 1 ) * gridZ + j ];
			triangle0[2] = vertices[ i * gridZ + ( j + 1 ) ];

			glm::vec3 triangle1[3];

			triangle1[0] = vertices[ i * gridZ + ( j + 1 ) ];
			triangle1[1] = vertices[ ( i + 1 ) * gridZ + j ];
			triangle1[2] = vertices[ ( i + 1 ) * gridZ + ( j + 1 ) ];

			tmpNormals[0][i][j] = glm::normalize( glm::cross( triangle0[1] - triangle0[0], triangle0[2] - triangle0[0] ) );
			tmpNormals[1][i][j] = glm::normalize( glm::cross( triangle1[1] - triangle1[0], triangle1[2] - triangle1[0] ) );
		}
	}

	// set the vertex normals by summing the normals of adjacent triangles
	for( GLuint i = 0; i < gridZ; i++ )
	{
		for( GLuint j = 0; j < gridX; j++ )
		{
			glm::vec3 normal = glm::vec3( 0.0f, 0.0f, 0.0f );

			if( j != 0 && i != 0 )
			{
				for( int k = 0; k < 2; k++ )
				{
					normal += tmpNormals[ k ][ i - 1][ j - 1 ];
				}
			}

			if( i != 0 && j != ( gridX - 1 ) )
			{
				normal += tmpNormals[ 0 ][ i - 1 ][ j ];
			}

			if( i != ( gridZ - 1 ) && j != ( gridX - 1 ) )
			{
				for( int k = 0; k < 2; k++ )
				{
					normal += tmpNormals[ k ][ i ][ j ];
				}
			}

			if( i != ( gridZ - 1 ) && j != 0 )
			{
				normal += tmpNormals[ 1 ][ i ][ j - 1 ];
			}

			normals.push_back( glm::normalize( normal ) );
		}
	}

	// set the terrain indices
	for( GLuint i = 0; i < gridZ - 1; i++ )
	{
		int count = gridZ * i;
		
		for( GLuint j = 0; j < gridX * 2; j++ )
		{
			indices.push_back( count );

			if( j % 2 == 0 )
			{
				count += gridX;
			}
			else
			{
				count -= gridX;
				count += 1;
			}
		}

		// primitive restart number
		indices.push_back( gridX * gridX );
	}	

	/*for( GLuint i = 0; i < gridZ - 1; i++ ) 
	{
		int count = gridX + gridZ * i;

		for( GLuint j = 0; j < gridX * 2; j++ ) 
		{
			indices.push_back( count );

			if( j % 2 == 0 )
				count -= gridX;
			else
				count += (gridX + 1);
		}

		indices.push_back( gridX * gridX );
	}*/


	delete[] heights;

	for( GLuint i = 0; i < gridZ; i++ )
	{
		delete[] tmpNormals[0][i];
	}

	for( GLuint i = 0; i < gridZ; i++ )
	{
		delete[] tmpNormals[1][i];
	}

	delete[] tmpNormals;

	return GL_OK;
}

/** 
*	@brief Set the heightmap to use
*
*	@author Demian Floreani
*
*	@param[in] path The path to heightmap
*
*   @return None
*/
void Demx::Terrain::setHeightMap( std::string path )
{
	this->heightMap = path;
}

/** 
*	@brief Set the operations to perform before rendering. Overloads derived class method.
*
*	@author Demian Floreani
*
*	@param[in] shader Pointer to shader program
*
*   @return None
*/
GLvoid Demx::Terrain::onStartDraw( Demx::Shader* program )
{
	glEnable( GL_PRIMITIVE_RESTART );
	//glDisable( GL_CULL_FACE );
}

/** 
*	@brief Set the operations to perform after rendering Overloads derived class method.
*
*	@author Demian Floreani
*
*	@param[in] shader Pointer to shader program
*
*   @return None
*/
GLvoid Demx::Terrain::onEndDraw()
{
	glDisable( GL_PRIMITIVE_RESTART );
	//glEnable( GL_CULL_FACE ); 
}