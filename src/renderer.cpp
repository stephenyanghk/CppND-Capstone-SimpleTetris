#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::string title, const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : window_title(title), screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

void Renderer::UpdateWindowTitle(int level, int score) {
    std::string title{window_title + " Level: " + std::to_string(level+1) + " Score: " + std::to_string(score)};
    SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::UpdateWindowTitle(std::string &&s) {
    std::string title{s};
    SDL_SetWindowTitle(sdl_window, title.c_str());
}