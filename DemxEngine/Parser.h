#pragma once

#include "Token.h"
#include <stack>

class Interpreter;

#define START_ELEMENT	"{"
#define END_ELEMENT     "}"
#define START_ARRAY     "["
#define END_ARRAY       "]"
#define START_VECTOR    "("
#define END_VECTOR      ")"

namespace Demx
{
	/** 
	*	@brief The parser takes as an input a script and creates an array of tokens to pass to the interpreter.
	*/
	class Parser
	{
		friend Interpreter;

	private:
		enum State
		{
			S_None =				0,
			S_Seeking_Semicolumn =	1,
			S_Seeking_Var =			2,
			S_Seeking_Var_Arr =		3,
			S_Seeking_Var_Vec =		4,
			S_Seeing_Vec_Arr =		5,
			S_Seeking_Vec =			6,
		};

		State state;

		GLboolean inElement;

		std::vector<std::vector<Demx::Token>*> elements;
		std::stack<Demx::Token> stk;
		std::string trimmed_vector;
		std::string source;

		Demx::Token getNextToken(const std::string& input, std::size_t& pos);

	public:
		Parser();
		~Parser();

		void reset();
		void loadSource(std::string path);
		GL_HRESULT lexicalAnalysis( std::string path );

		void debugParsing(std::ostream& out);
	};
}