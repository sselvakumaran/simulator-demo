#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#if defined(SDL_PLATFORM_LINUX)
#include <X11/Xlib.h>
#endif

bgfx::PlatformData get_pd(SDL_Window* window) {
  bgfx::PlatformData pd{};
#if defined(SDL_PLATFORM_WIN32)
  pd.nwh = (HWND) SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
  pd.ndt = NULL;
#elif defined(SDL_PLATFORM_LINUX)
  if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0) {
    pd.ndt = (Display *) SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
    pd.nwh = (void *) (uintptr_t) (Window) SDL_GetNumberProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
  } else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0) {
    pd.ndt = (struct wl_display *) SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
    pd.nwh = (struct wl_surface *) SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
  }
#endif
  return pd;
}