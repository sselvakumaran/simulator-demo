#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <Cocoa/Cocoa.h>

bgfx::PlatformData get_pd(SDL_Window* window) {
  bgfx::PlatformData pd{};
  pd.ndt = (__bridge NSWindow *) SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL);
  pd.nwh = NULL;
  return pd;
}