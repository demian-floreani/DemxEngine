#include "stdafx.h"
#include "Token.h"

Demx::Token::Token(TokenType type, std::string value)
{
	this->type = type;
	this->value = value;
}

bool Demx::Token::operator==(const Demx::Token& token)
{
	return this->type == token.type;
}