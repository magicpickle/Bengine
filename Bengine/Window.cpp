#include "Window.h"
#include "ErrorHelper.h"

namespace Bengine
{
	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{

		Uint32 flags = SDL_WINDOW_OPENGL;
		if (currentFlags & Bengine::INVISIBLE)
			flags |= SDL_WINDOW_HIDDEN;
		if (currentFlags & Bengine::FULLSCREEN)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		if (currentFlags & Bengine::BORDERLESS)
			flags |= SDL_WINDOW_BORDERLESS;


		//create the window
		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

		//Error checking
		if (_sdlWindow == nullptr)
		{
			fatalError("SDL Window could not be created!");
		}
		//CONTEXT
		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr)
		{
			fatalError("SDL_GL context could not be created!");
		}
		//GLEW
		GLenum error = glewInit();
		if (error != GLEW_OK)
		{
			fatalError("Could not initialize glew!");
		}

		//print out the GL version
		std::printf("***     OpenGL Version: %s   ****\n", glGetString(GL_VERSION));


		//SET BACKGROUD COLOR
		glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

		//set vsync 
		SDL_GL_SetSwapInterval(0);

		//Enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}
	void Window::swapBuffer()
	{
		//Swap our buffer and draw everything
		SDL_GL_SwapWindow(_sdlWindow);
	}
}