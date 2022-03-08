/**
 * @author <code@tythos.net>
 */

#include "App.h"

/**
 * Construct a new App object, including initialization of SDL, GL, and GLEW
 * subsystems.
 */
App::App() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw std::runtime_error("SDL initialization failed");
	}
	this->window = SDL_CreateWindow("Artemis - Alphonse", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->screenWidth, this->screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (this->window == NULL) {
		throw std::runtime_error("Window initialization failed");
	}
	this->context = SDL_GL_CreateContext(this->window);
	if (this->context == NULL) {
		throw std::runtime_error("Context initialization failed");
	}
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		throw std::runtime_error("GLEW initialization failed");
	}
	SDL_StartTextInput();
	this->graphics = new Graphics();
}

/**
 * Destroy the App object. This frees graphics resources and window handles, as
 * well as other SDL-specific cleanup.
 */
App::~App() {
	delete this->graphics;
	SDL_StopTextInput();
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

/**
 * Clears color buffer and (if input is true) invokes the graphic system's
 * "render()" routine. Once finished, swaps graphics buffers for this window's
 * context.
 * 
 * @param {bool} isMeshVisible - If true, forwards a render() call to the graphics system
 */
void App::render(bool isMeshVisible) {
	glClear(GL_COLOR_BUFFER_BIT);
	if (isMeshVisible) {
		this->graphics->render();
	}
	SDL_GL_SwapWindow(this->window);
}
