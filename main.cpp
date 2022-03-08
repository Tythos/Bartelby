/**
 * "Bartelby" variation of beginner game engine architectures.
 *
 * @author <code@tythos.net>
 */

#include "App.h"

/**
 * Global state
 */
bool IS_POLYGON_RENDERED = true;

/**
 * Invoked when SDL text input produces a keypress event.
 *
 * @param {unsigned char} key - Indicates what key was pressed
 * @returns {bool} - Defaults to true, but will return false if a keypress results in a program interrupt
 */
bool onTextInput(unsigned char key) {
	bool isRunning = true;
	if (key == ' ') {
		IS_POLYGON_RENDERED = !IS_POLYGON_RENDERED;
	}
	if (key == 'q') {
		isRunning = false;
	}
	return isRunning;
}

/**
 * Defines a single iteration of the main application/game loop.
 *
 * @returns {bool} Indicates if the loop should continue running.
 */
bool mainLoop(App* hApp) {
	bool isRunning = true;
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
			isRunning = false;
		}
		else if (event.type == SDL_TEXTINPUT) {
			isRunning = onTextInput(event.text.text[0]);
		}
	}
	hApp->render(IS_POLYGON_RENDERED);
	return isRunning;
}

/**
 * Primary entry point. After initialization, this largely consists of calling
 * the main loop so long as it returns true. Once it returns false, resources
 * are cleaned up and the entry point returns.
 *
 * @param {int} nArgs - Not used
 * @param {char**} vArgs - Not used
 */
int main(int nArgs, char** vArgs) {
	App* hApp = new App();
	bool isRunning = true;
	while (isRunning) {
		isRunning = mainLoop(hApp);
	}
	delete hApp;
	return 0;
}
