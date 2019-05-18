#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <sstream>

#include "glew\glew.h"
#include <gl\gl.h>
#include <gl\glu.h>

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"

// flow control

#define GL_OK			GL_TRUE
#define GL_FAIL			GL_FALSE
typedef GLboolean		GL_HRESULT;

// standard messages

#define MESSAGE			UINT
#define PARAM			WPARAM
#define KEYDOWN			WM_KEYDOWN
#define KEYUP			WM_KEYUP


typedef glm::vec3 Color3;
// vectors and matrices

typedef glm::vec3 vec3_t;

#define VEC2			glm::vec2
#define	VEC3        	glm::vec3
#define VEC4			glm::vec4
#define MAT4			glm::mat4
#define IDENTITY_MAT4	MAT4(1)
#define COLOR3			VEC3
#define COLOR4			VEC4

// standard vectors

#define VEC3_ZERO		glm::vec3( 0.0f, 0.0f, 0.0f )
#define VEC4_ZERO		glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f )

#define VEC3_X			glm::vec3( 1.0f, 0.0f, 0.0f )
#define VEC3_Y			glm::vec3( 0.0f, 1.0f, 0.0f )
#define VEC3_Z			glm::vec3( 0.0f, 0.0f, 1.0f )

// useful functions

#define NORMALIZE(i)	glm::normalize(i)

// standard colors

#define COLOR3_WHITE	COLOR3( 1.0f, 1.0f, 1.0f )
#define COLOR3_BLACK	COLOR3( 0.0f, 0.0f, 0.0f )
#define COLOR3_RED		COLOR3( 1.0f, 0.0f, 0.0f )
#define COLOR3_GREEN	COLOR3( 0.0f, 1.0f, 0.0f )
#define COLOR3_BLUE		COLOR3( 0.0f, 0.0f, 1.0f )
#define COLOR3_YELLOW	COLOR3( 1.0f, 1.0f, 0.0f )
#define COLOR3_CYAN 	COLOR3( 0.0f, 1.0f, 1.0f )
#define COLOR3_PURPLE 	COLOR3( 1.0f, 0.0f, 1.0f )

