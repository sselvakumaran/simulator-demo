#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <iostream>
#include "platform/platform_window.h"

inline bool WindowInit(SDL_Window* window) {
  if (!window) {
    std::cerr << "failed to set window: " << SDL_GetError() << std::endl;
    return false;
  }
  std::cout << "Getting platform data..." << std::endl;
  bgfx::PlatformData pd = get_pd(window);
  std::cout << "Platform data obtained. nwh: " << pd.nwh << ", ndt: " << pd.ndt << std::endl;

  bgfx::Init init;
  init.type = bgfx::RendererType::Count;
  init.resolution.width = 800;
  init.resolution.height = 600;
  init.resolution.reset = BGFX_RESET_VSYNC;
  init.debug = BGFX_DEBUG_TEXT;
  init.platformData = pd;
  init.vendorId = BGFX_PCI_ID_NONE;
  std::cout << "Initializing bgfx..." << std::endl;
  if (!bgfx::init(init)) {
    std::cerr << "bgfx::init failed" << std::endl;
    return false;
  }
  std::cout << "bgfx initialized successfully" << std::endl;
  return true;
}

int main(int argc, char* argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "failed to initialize SDL3: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow(
    "simulator", 800, 600,
    SDL_WINDOW_RESIZABLE
  );

  if (!WindowInit(window)) {
    std::cerr << "failed to initialize bgfx" << std::endl;
    return 1;
  }

  std::cout << "launching" << std::endl;

  bool running = true;
  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) running = false;
    }
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xff0000ff, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, 800, 600);
    bgfx::touch(0);

    bgfx::frame();
  }

  bgfx::shutdown();
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}