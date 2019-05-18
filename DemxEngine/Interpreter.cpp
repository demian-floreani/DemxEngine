#include "stdafx.h"

#include "Interpreter.h"

#include "Engine.h"
#include "Parser.h"

/** 
*	@brief Constructor for the interpreter
* 
*	@author Demian Floreani
*
*	@param[in] engine Pointer to the engine
*	@param[in] parser Pointer to the parser
*
*	@return None
*/
Demx::Interpreter::Interpreter(Engine* engine, Parser* parser)
{
	this->engine = engine;
	this->parser = parser;

	state = 0;
	inArray = false;
}

Demx::Interpreter::~Interpreter(void)
{
}

/** 
*	@brief Runs the interpreter on the token vector that was produced by the parser.
* 
*	@author Demian Floreani
*
*	@return None
*/
void Demx::Interpreter::interpret()
{
	for(std::vector<std::vector<Token>*>::iterator it = parser->elements.begin(); it != parser->elements.end(); it++)
	{
		Token tok = (*it)->at(0);

		std::string name;
		std::string path;
		std::string material;
		std::string model;
		std::vector<std::string> texturing;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> mappings;
		std::vector<glm::vec3> transitions;
		VEC3 vector, specularColor = VEC3_ZERO, color = VEC3_ZERO, direction = VEC3_ZERO, position = VEC3_ZERO;
		GLfloat modifier = 1.0f, specular = 1.0f, transparency = 1.0f, speed = 1.0f, fadeIn = 0.0f;
		GLboolean shadow = GL_FALSE; 
		GLboolean castShadow = GL_TRUE;
		GLint size = 0;
		std::vector<std::string> listEntities;

		// loop through the tokens
		for( std::size_t i = 1; i < (*it)->size(); i++ )
		{
			int type = (*it)->at(i).type;
			std::string value = (*it)->at(i).value;

			switch( type )
			{
				// if the token type is a variable, process it's value based on the present state
				case TokenType::T_Variable:
				{
					switch( state )
					{
						case TokenType::T_Name:
						{
							if( inArray )
							{
								listEntities.push_back( value );
							}
							else
							{
								name = value;
							}
						}break;

						case TokenType::T_Path:
						{
							path = value;
						}break;

						case TokenType::T_Ent_Model:
						{
							model = value;
						}break;

						case TokenType::T_Texturing:
						{
							texturing.push_back( value );
						}break;

						case TokenType::T_Specular:
						{
							specular = extractFloat( value );
						}break;

						case TokenType::T_SpecularColor:
						{
							specularColor = extractVector3( value );
						}break;

						case TokenType::T_Transparency:
						{
							transparency = extractFloat( value );
						}break;

						case TokenType::T_Mapping:
						{
							mappings.push_back( extractVector2( value ) );
						}break;

						case TokenType::T_Vertex:
						{
							vertices.push_back( extractVector3( value ) );
						}break;

						case TokenType::T_Vector:
						{
							vector = extractVector3( value );
						}break;

						case TokenType::T_Ent_Mat:
						{
							material = value;
						}break;

						case TokenType::T_Modifier:
						{
							modifier = extractFloat( value );
						}break;

						case TokenType::T_Color:
						{
							color = extractVector3( value );
						}break;

						case TokenType::T_Direction:
						{
							direction = extractVector3( value );
						}break;

						case TokenType::T_Position:
						{
							position = extractVector3( value );
						}break;

						case TokenType::T_Shadow:
						{
							shadow = extractBoolean( value );
						}break;

						case TokenType::T_Size:
						{
							size = extractInt( value );
						}break;

						case TokenType::T_Transition:
						{
							transitions.push_back( extractVector3( value ) );
						}break;

						case TokenType::T_Ent_Cast_Shadow:
						{
							castShadow = extractBoolean( value );
						}break;

						case TokenType::T_Speed:
						{
							speed = extractFloat( value );
						}break;

						case TokenType::T_Fade_In:
						{
							fadeIn = extractFloat( value );
						}break;
					}

					continue;
				}break;

				case TokenType::T_Element_Start:
				{
					continue;
				}break;

				case TokenType::T_Element_End:
				{
					continue;
				}break;

				case TokenType::T_Array_Start:
				{
					inArray = true;
					continue;
				}break;

				case TokenType::T_Array_End:
				{
					inArray = false;
					continue;
				}break;
			}

			state = type;
		}

		switch( tok.type )
		{
			case TokenType::T_Texture: 
			{
				engine->addTexture(name, path);
			}break;

			case TokenType::T_Material:
			{
				engine->addMaterial(name, texturing, specularColor, specular, transparency);
			}break;

			case TokenType::T_Model:
			{
				engine->addModel(name, path);
			}break;

			case TokenType::T_Entity:
			{
				if( model.empty() )
				{
					Demx::Entity* entity = new Demx::Entity( GL_TRIANGLES );
					
					for( std::size_t i = 0; i < vertices.size(); i += 3)
					{
						entity->triangle( vertices[i + 0], vertices[i + 1], vertices[i + 2] );
					}

					entity->mappings = mappings;
					entity->setMaterial(material);
					engine->createEntity(name, entity);
				}
				else
				{
					engine->createEntity(name, model);
				}
			}break;

			case TokenType::T_Translate:
			{
				if( !listEntities.empty() )
				{
					for(std::vector<std::string>::iterator it = listEntities.begin(); it != listEntities.end(); it++)
					{
						engine->getEntity(*it)->translate(vector);
					}
				}
				else
				{
					engine->getEntity(name)->translate(vector);
				}
			}break;

			case TokenType::T_Scale:
			{
				if( !listEntities.empty() )
				{
					for(std::vector<std::string>::iterator it = listEntities.begin(); it != listEntities.end(); it++)
					{
						engine->getEntity(*it)->scaleAll(modifier);
					}
				}
				else
				{
					engine->getEntity(name)->scaleAll(modifier);
				}
			}break;

			case TokenType::T_Rotate:
			{
				if( !listEntities.empty() )
				{
					for(std::vector<std::string>::iterator it = listEntities.begin(); it != listEntities.end(); it++)
					{
						engine->getEntity(*it)->rotate(modifier, vector);
					}
				}
				else
				{
					engine->getEntity(name)->rotate(modifier, vector);
				}
			}break;

			case TokenType::T_Ambient_Light:
			{
				Demx::Light* light = engine->createAmbientLight(color);
				light->setName( name );
			}break;

			case TokenType::T_Directional_Light:
			{
				Demx::Light* light = engine->createDirectionalLight(color, direction);
				light->setName( name );
				light->castShadow( shadow );
			}break;

			case TokenType::T_Spot_Light:
			{
				Demx::Light* light = engine->createSpotLight(color, position, direction);
				light->setName( name );
				light->castShadow( shadow );
			}break;

			case TokenType::T_Point_Light:
			{
				Demx::Light* light = engine->createPointLight(color, position);
				light->setName( name );
				light->castShadow( shadow );
			}break;

			case TokenType::T_Terrain:
			{
				Demx::Terrain* terrain = engine->addTerrain( path, material );
				terrain->setName( name );
			}break;

			case TokenType::T_Wave:
			{
				Demx::WaveSimultation* waves = engine->addWaveSimultation( size );
				waves->setName( name );
				waves->setMaterial( material );
				waves->setCastShadow( castShadow );
			}break;

			case TokenType::T_Particle_System:
			{
				Demx::ParticleSystem* ps = engine->createParticleSystem( size );
				ps->setName( name );
				ps->setPosition( position );
				ps->setMaterial( material );
				ps->setGravity( vector );
				ps->setSize( modifier );
				ps->setSpeed( speed );
				ps->setLoop( GL_TRUE );

				if( transitions.size() != 0 )
				{
					ps->setColorTransition( transitions[0], transitions[1], 8.0f );
				}
				else
				{
					ps->setColor( VEC4( color.x, color.y, color.z, 1 ) );
				}

				if( fadeIn != 0 )
				{
					ps->setFadeIn( fadeIn );
				}
			}break;
		}
	}
}

/** 
*	@brief Extracts an int from a string
* 
*	@author Demian Floreani
*
*	@param[in] text A string
*
*	@return GLint
*/
GLint Demx::Interpreter::extractInt( std::string text )
{
	return stoi( text );
}

/** 
*	@brief Extracts a float from a string
* 
*	@author Demian Floreani
*
*	@param[in] text A string
*
*	@return GLfloat
*/
GLfloat Demx::Interpreter::extractFloat( std::string text )
{
	return stof( text );
}

/** 
*	@brief Extracts a boolean from a string
* 
*	@author Demian Floreani
*
*	@param[in] text A string
*
*	@return GLboolean
*/
GLboolean Demx::Interpreter::extractBoolean( std::string text )
{
	if( text == "true" || text == "1" )
	{
		return GL_TRUE;
	}
	else
	{
		return GL_FALSE;
	}
}

/** 
*	@brief Extracts a 2D vector from a string
* 
*	@author Demian Floreani
*
*	@param[in] text A string
*
*	@return VEC2
*/
VEC2 Demx::Interpreter::extractVector2( std::string text )
{
	std::stringstream ss( text );

	int j = 0;
	float el;
	float points[2];

	while (ss >> el)
	{
		points[j] = el;

		if (ss.peek() == ',')
			ss.ignore();

		j++;
	}

	return VEC2(points[0], points[1]);
}

/** 
*	@brief Extracts a 3D vector from a string
* 
*	@author Demian Floreani
*
*	@param[in] text A string
*
*	@return VEC3
*/
VEC3 Demx::Interpreter::extractVector3( std::string text )
{
	std::stringstream ss( text );

	int j = 0;
	float el;
	float points[3];

	while (ss >> el)
	{
		points[j] = el;

		if (ss.peek() == ',')
			ss.ignore();

		j++;
	}

	return VEC3(points[0], points[1], points[2]);
}