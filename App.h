/**
 * @author <code@tythos.net>
 */

#include <SDL.h>
#include <gl\glew.h>
#include "Graphics.h"
#include <glm/glm.hpp>

#ifndef APP_H
#define APP_H

/**
 * Defines top-level singleton of an application instance, including window
 * management and GL context.
 * 
 * @property {int} screenWidth - Width of window, in pixels (defaults to 640)
 * @property {int} screenHeight - Height of window, in pixels (defaults to 480)
 * @property {SDL_Window*} window - Handle to SDL window instance
 * @property {Graphics*} graphics - Subsystem for managing GL state and behavior
 * @property {SDL_GLContext} context - Specific GL graphics context for binding w/ window manager
 */
class App {
private:
protected:
	int screenWidth = 640;
	int screenHeight = 480;
	SDL_Window* window = NULL;
	Graphics* graphics = NULL;
	SDL_GLContext context;
	glm::mat4x4 camera;

public:
	App();
	~App();
	void render(bool isMeshVisible);
	void setCamPos(float x, float y, float z);
	void setCamLook(float x, float y, float z);
};

#endif
