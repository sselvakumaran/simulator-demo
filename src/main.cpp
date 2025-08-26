#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

int main(int argc, char* argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "failed to initialize SDL3: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow(
    "simulator", 800, 600,
    SDL_WINDOW_RESIZABLE
  );

  if (!window) {
    std::cerr << "failed to create window: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) running = false;
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}