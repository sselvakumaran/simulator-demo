#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <SDL3/SDL_metal.h>
#include <Cocoa/Cocoa.h>

// TODO: NOT have to rely on metal here
bgfx::PlatformData get_pd(SDL_Window* window) {
  bgfx::PlatformData pd{};
  // NSWindow *ns_window = (__bridge NSWindow *) SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
  SDL_MetalView metalView = SDL_Metal_CreateView(window);
  pd.nwh = SDL_Metal_GetLayer(metalView);
  pd.ndt = nullptr;
  return pd;
}