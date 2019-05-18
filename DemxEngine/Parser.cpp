#include "stdafx.h"

#include <iostream>
#include <fstream>

#include "Parser.h"

Demx::Parser::Parser()
{
	state = State::S_None;
	inElement = GL_FALSE;
}

Demx::Parser::~Parser()
{
	reset();
}

/** 
*	@brief Resets the parser.
* 
*	@author Demian Floreani
*
*
*	@return None
*/
void Demx::Parser::reset()
{
	for(std::size_t i = 0; i < elements.size(); i++)
	{
		delete elements[i];
	}

	elements.clear();
}

/** 
*	@brief Loads a script file.
* 
*	@author Demian Floreani
*
*	@param[in] path Location of script file
*
*	@return None
*/
void Demx::Parser::loadSource(std::string path)
{
	std::string line;
	std::ifstream myfile (path);

	if (myfile.is_open())
	{
		while (getline (myfile, line))
		{
			if(line[0] == '#')
				continue;

			source += line;
		}

		myfile.close();
	}
}

/** 
*	@brief Gets the next token from the character stream.
* 
*	@author Demian Floreani
*
*	@param[in] input The character stream of the script source
*	@param[in] pos The position in the character stream that the parser has reached
*
*	@return Token The next lexical token
*/
Demx::Token Demx::Parser::getNextToken(const std::string& input, std::size_t& pos)
{
	std::string text;


	for(std::size_t i = pos; i < input.size(); i++)
	{
		pos = i + 1;

		if(input.at(i) == ' ' || input.at(i) == '\n' || input.at(i) == '\t' || input.at(i) == '#')
			continue;

		if(state == State::S_None && input.at(i) == ',')
			continue;

		text += input[i];

		switch(state)
		{
			case State::S_Seeking_Semicolumn:
			{
				if(input.at(i) == ':')
				{
					state = State::S_Seeking_Var;
					text = "";
				}
			}break;

			case State::S_Seeking_Var:
			{
				if(text == "(")
				{
					state = State::S_Seeking_Vec;
					text = "";
					trimmed_vector = "";
				}

				if(input.at(i) == ',' || input.at(i) == '}')
				{
					pos = i;
					state = State::S_None;
					text.pop_back();
					return Token(TokenType::T_Variable, text);
				}
			}break;

			case State::S_Seeking_Var_Arr:
			{
				if(input.at(i) == ',')
				{
					pos = i + 1;
					text.pop_back();
					return Token(TokenType::T_Variable, text);
				}

				if(input.at(i) == ']')
				{
					state = State::S_None;

					pos = i;
					text.pop_back();
					return Token(TokenType::T_Variable, text);
				}
			}break;

			case State::S_Seeking_Var_Vec:
			{
				if(input.at(i) == ',')
				{
					trimmed_vector += text; 
					text = "";
				}

				if(input.at(i) == ')')
				{
					state = State::S_Seeing_Vec_Arr;
					text.pop_back();
					trimmed_vector += text; 
					return Token(TokenType::T_Variable, trimmed_vector);
				}
			}break;

			case State::S_Seeing_Vec_Arr:
			{
				if(input.at(i) == ',')
				{
					text = "";
					continue;
				}
			}break;

			case State::S_Seeking_Vec:
			{
				if(input.at(i) == ',')
				{
					trimmed_vector += text; 
					text = "";
				}

				if(input.at(i) == ')')
				{
					state = State::S_None;
					text.pop_back();
					trimmed_vector += text; 
					return Token(TokenType::T_Variable, trimmed_vector);
				}
			}break;
		}

		if( inElement == GL_FALSE )
		{
			if(text == "entity")
			{
				//check next char
				if(input.at(i + 1) == ' ' || input.at(i + 1) == '{')
				{
					pos = i + 1;
					inElement = GL_TRUE;
					return Token(TokenType::T_Entity);
				}
			}
			else if(text == "texture")
			{
				//check next char
				if(input.at(i + 1) == ' ' || input.at(i + 1) == '{')
				{
					pos = i + 1;
					inElement = GL_TRUE;
					return Token(TokenType::T_Texture);
				}
			}
			else if(text == "material")
			{
				//check next char
				if(input.at(i + 1) == ' ' || input.at(i + 1) == '{')
				{
					pos = i + 1;
					inElement = GL_TRUE;
					return Token(TokenType::T_Material);
				}
			}
			else if(text == "model")
			{
				//check next char
				if(input.at(i + 1) == ' ' || input.at(i + 1) == '{')
				{
					pos = i + 1;
					inElement = GL_TRUE;
					return Token(TokenType::T_Model);
				}
			}
			else if(text == "translate")
			{
				//check next char
				if(input.at(i + 1) == ' ' || input.at(i + 1) == '{')
				{
					pos = i + 1;
					inElement = GL_TRUE;
					return Token(TokenType::T_Translate);
				}
			}
			else if(text == "scale")
			{
				//check next char
				if(input.at(i + 1) == ' ' || input.at(i + 1) == '{')
				{
					pos = i + 1;
					inElement = GL_TRUE;
					return Token(TokenType::T_Scale);
				}
			}
			else if(text == "rotate")
			{
				//check next char
				if(input.at(i + 1) == ' ' || input.at(i + 1) == '{')
				{
					pos = i + 1;
					inElement = GL_TRUE;
					return Token(TokenType::T_Rotate);
				}
			}
			else if(text == "ambient_light")
			{
				//check next char
				if(input.at(i + 1) == ' ' || input.at(i + 1) == '{')
				{
					pos = i + 1;
					inElement = GL_TRUE;
					return Token(TokenType::T_Ambient_Light);
				}
			}
			else if(text == "directional_light")
			{
				//check next char
				if(input.at(i + 1) == ' ' || input.at(i + 1) == '{')
				{
					pos = i + 1;
					inElement = GL_TRUE;
					return Token(TokenType::T_Directional_Light);
				}
			}
			else if(text == "spot_light")
			{
				//check next char
				if(input.at(i + 1) == ' ' || input.at(i + 1) == '{')
				{
					pos = i + 1;
					inElement = GL_TRUE;
					return Token(TokenType::T_Spot_Light);
				}
			}
			else if(text == "point_light")
			{
				//check next char
				if(input.at(i + 1) == ' ' || input.at(i + 1) == '{')
				{
					pos = i + 1;
					inElement = GL_TRUE;
					return Token(TokenType::T_Point_Light);
				}
			}
			else if(text == "terrain")
			{
				//check next char
				if(input.at(i + 1) == ' ' || input.at(i + 1) == '{')
				{
					pos = i + 1;
					inElement = GL_TRUE;
					return Token(TokenType::T_Terrain);
				}
			}
			else if(text == "wave")
			{
				//check next char
				if(input.at(i + 1) == ' ' || input.at(i + 1) == '{')
				{
					pos = i + 1;
					inElement = GL_TRUE;
					return Token(TokenType::T_Wave);
				}
			}
			else if(text == "particle_system")
			{
				//check next char
				if(input.at(i + 1) == ' ' || input.at(i + 1) == '{')
				{
					pos = i + 1;
					inElement = GL_TRUE;
					return Token(TokenType::T_Particle_System);
				}
			}
		}
		else
		{
			if(text == "name")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Name);
			}
			else if(text == "texturing")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Texturing);
			}
			else if(text == "vertices")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Vertex);
			}
			else if(text == "path")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Path);
			}
			else if(text == "phong_exp")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Specular);
			}
			else if(text == "specular_color")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_SpecularColor);
			}
			else if(text == "transparency")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Transparency);
			}
			else if(text == "material")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Ent_Mat);
			}
			else if(text == "vector")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Vector);
			}
			else if(text == "mapping")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Mapping);
			}
			else if(text == "model")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Ent_Model);
			}
			else if(text == "modifier")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Modifier);
			}
			else if(text == "color")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Color);
			}
			else if(text == "direction")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Direction);
			}
			else if(text == "position")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Position);
			}
			else if(text == "shadow")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Shadow);
			}
			else if(text == "size")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Size);
			}
			else if(text == "transition")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Transition);
			}
			else if(text == "cast_shadow")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Ent_Cast_Shadow);
			}
			else if(text == "speed")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Speed);
			}
			else if(text == "fade_in")
			{
				state = State::S_Seeking_Semicolumn;
				return Token(TokenType::T_Fade_In);
			}
		}

		if(text == "{")
		{
			stk.push(Token(TokenType::T_Element_Start));
			return Token(TokenType::T_Element_Start);
		}
		else if(text == "}")
		{
			if(!stk.empty())
			{
				Token t = stk.top();
				stk.pop();

				if((t.type == TokenType::T_Element_Start))
				{
					inElement = GL_FALSE;
					return Token(TokenType::T_Element_End);
				}
				else
				{
					return Token(TokenType::T_Error);
				}
			}
		}
		else if(text == "[")
		{
			state = State::S_Seeking_Var_Arr;
			stk.push(Token(TokenType::T_Array_Start));
			return Token(TokenType::T_Array_Start);
		}
		else if(text == "]")
		{
			state = State::S_None;

			if(!stk.empty())
			{
				Token t = stk.top();
				stk.pop();

				if(t == TokenType::T_Array_Start)
					return Token(TokenType::T_Array_End);
				else
					return Token(TokenType::T_Error);
			}
		}
		else if(text == "(")
		{
			state = State::S_Seeking_Var_Vec;
			text = "";
			trimmed_vector = "";
		}
		else if(text == ")")
		{
			state = State::S_None;
		}
	}

	return Token(TokenType::T_Error, "");
}

/** 
*	@brief Runs a lexical analysis of a script file. The parser will save an vector of tokens to be used by the interpreter.
* 
*	@author Demian Floreani
*
*	@param[in] path Location of script file 
*
*	@return GL_HRESULT GL_OK if the analysis was successful, GL_FAIL otherwise
*/
GL_HRESULT Demx::Parser::lexicalAnalysis( std::string path )
{
	reset();

	loadSource( path );

	std::size_t pos = 0;
	std::vector<Token>* vec = nullptr;

	while(pos < source.size())
	{
		Token tok = getNextToken(source, pos);
			
		if(tok.type == TokenType::T_Error)
		{
			return GL_FAIL;
		}

		if(tok == TokenType::T_Entity)
		{
			vec = new std::vector<Token>();
			vec->push_back(tok);
			elements.push_back(vec);
		}
		else if(tok == TokenType::T_Texture)
		{
			vec = new std::vector<Token>();
			vec->push_back(tok);
			elements.push_back(vec);
		}
		else if(tok == TokenType::T_Material)
		{
			vec = new std::vector<Token>();
			vec->push_back(tok);
			elements.push_back(vec);
		}
		else if(tok == TokenType::T_Model)
		{
			vec = new std::vector<Token>();
			vec->push_back(tok);
			elements.push_back(vec);
		}
		else if(tok == TokenType::T_Translate)
		{
			vec = new std::vector<Token>();
			vec->push_back(tok);
			elements.push_back(vec);
		}
		else if(tok == TokenType::T_Scale)
		{
			vec = new std::vector<Token>();
			vec->push_back(tok);
			elements.push_back(vec);
		}
		else if(tok == TokenType::T_Rotate)
		{
			vec = new std::vector<Token>();
			vec->push_back(tok);
			elements.push_back(vec);
		}
		else if(tok == TokenType::T_Ambient_Light)
		{
			vec = new std::vector<Token>();
			vec->push_back(tok);
			elements.push_back(vec);
		}
		else if(tok == TokenType::T_Directional_Light)
		{
			vec = new std::vector<Token>();
			vec->push_back(tok);
			elements.push_back(vec);
		}
		else if(tok == TokenType::T_Spot_Light)
		{
			vec = new std::vector<Token>();
			vec->push_back(tok);
			elements.push_back(vec);
		}
		else if(tok == TokenType::T_Point_Light)
		{
			vec = new std::vector<Token>();
			vec->push_back(tok);
			elements.push_back(vec);
		}
		else if(tok == TokenType::T_Terrain)
		{
			vec = new std::vector<Token>();
			vec->push_back(tok);
			elements.push_back(vec);
		}
		else if(tok == TokenType::T_Wave)
		{
			vec = new std::vector<Token>();
			vec->push_back(tok);
			elements.push_back(vec);
		}
		else if(tok == TokenType::T_Particle_System)
		{
			vec = new std::vector<Token>();
			vec->push_back(tok);
			elements.push_back(vec);
		}
		else
		{
			vec->push_back(tok);
		}
	}

	if(!stk.empty()) 
	{
		return GL_FAIL;
	}

	return GL_OK;
}

void Demx::Parser::debugParsing(std::ostream& out)
{
	for(std::size_t i = 0; i < elements.size(); i++)
	{
		for(std::size_t j = 0; j < elements[i]->size(); j++)
		{
			out << elements[i]->at(j).type << " " << elements[i]->at(j).value.c_str() << std::endl;
		}

		out << "======" << std::endl;
	}
}