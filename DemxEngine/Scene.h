#pragma once

#include "Engine.h"

namespace Demx
{
	class Scene
	{
	public:
		Demx::Engine* engine;

		std::vector<std::string> scripts;
		std::vector<std::string> luaScripts;

		Scene();
		virtual ~Scene( GLvoid );

		/** 
		 *	@brief Initialization method for the scene. Must be implemented by derived classes.
		 * 
		 *	@author Demian Floreani
		 *
		 *
		 *	@return GL_HRESULT
		 */
		virtual GL_HRESULT Init( GLvoid ) = 0;

		/** 
		 *	@brief Loop method for the scene. Must be implemented by derived classes.
		 * 
		 *	@author Demian Floreani
		 *
		 *  @param[in] timeDelta time variable for time based movements
		 *
		 *	@return GL_HRESULT
		 */
		virtual GL_HRESULT Loop( GLfloat timeDelta ) = 0;

		/** 
		 *	@brief Input method for the scene. This method allows to manage user input. Must be implemented by derived classes.
		 * 
		 *	@author Demian Floreani
		 *
		 *  @param[in] msg Contains the type of user input
		 *  @param[in] param Contains the value of the user input
		 *
		 *	@return GL_HRESULT
		 */
		virtual GLvoid Input( MESSAGE msg, PARAM param ) = 0;

		void addScript( const std::string& path );
		void addLuaScript( const std::string& path );

		void setCamera( Demx::Camera* c );
	};
}