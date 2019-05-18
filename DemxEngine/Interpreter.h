#pragma once

namespace Demx
{
	class Engine;
	class Parser;

	/** 
	*	@brief The interpreter processes the output of the parser.
	*/
	class Interpreter
	{
		Engine* engine;
		Parser* parser;

	private:
		int state;
		bool inArray;

	public:
		Interpreter(Engine* engine, Parser* parser);
		~Interpreter(void);

		void interpret();

	private:

		GLint extractInt( std::string text );
		GLfloat extractFloat( std::string text );
		GLboolean extractBoolean( std::string text );
		VEC2 extractVector2( std::string text );
		VEC3 extractVector3( std::string text );
	};
}