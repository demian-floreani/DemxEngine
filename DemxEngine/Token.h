#pragma once

#include <string>
#include <vector>

namespace Demx
{
	enum TokenType
	{
		T_Entity =				0,
		T_Texture =				1,
		T_Material =			2,
		T_Model =				3,
		T_Translate =			4,
		T_Scale =				5,
		T_Rotate =				6,
		T_Ambient_Light =		7,
		T_Directional_Light =	8,
		T_Spot_Light =			9,
		T_Point_Light =			10,
		T_Terrain =				11,
		T_Wave =				12,
		T_Particle_System =		13,

		T_Name =				50,
		T_Texturing =			51,
		T_Vertex =				52,
		T_Variable =			53,
		T_Path =				54,
		T_Specular =			55,
		T_SpecularColor =		56,
		T_Transparency =		57,
		T_Ent_Mat =				58,
		T_Vector =				59,
		T_Mapping =				60,
		T_Ent_Model =			61,
		T_Modifier =			62,
		T_Color =				63,
		T_Direction =			64,
		T_Position =			65,
		T_Shadow =				66,
		T_Size =				67,
		T_Transition =          68,
		T_Ent_Cast_Shadow =		69,
		T_Speed =				70,
		T_Fade_In =				71,

		T_Element_Start =		100,
		T_Element_End =			101,
		T_Array_Start =			102,
		T_Array_End =			103,

		T_Error =				-1
	};

	/** 
	*	@brief The token class is used to store lexical tokens by the Parser when performing a lexical analysis of the scripts.
	*/
	class Token
	{
	public:
		TokenType type;
		std::string value;

		Token(TokenType type, std::string value = "");
		bool operator==(const Token& token);
	};
}