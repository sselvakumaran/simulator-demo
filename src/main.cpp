#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>
#include <iostream>
#include "platform/platform_window.h"

struct PosColorVertex {
    float x, y, z;
    uint32_t abgr;
    static void init() {
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
            .end();
    }
    static bgfx::VertexLayout layout;
};

bgfx::VertexLayout PosColorVertex::layout;

static PosColorVertex s_cubeVertices[] = {
    {-1.0f,  1.0f,  1.0f, 0xff0000ff }, // 0
    { 1.0f,  1.0f,  1.0f, 0xff00ff00 }, // 1
    {-1.0f, -1.0f,  1.0f, 0xffff0000 }, // 2
    { 1.0f, -1.0f,  1.0f, 0xffffffff }, // 3
    {-1.0f,  1.0f, -1.0f, 0xff0000ff }, // 4
    { 1.0f,  1.0f, -1.0f, 0xff00ff00 }, // 5
    {-1.0f, -1.0f, -1.0f, 0xffff0000 }, // 6
    { 1.0f, -1.0f, -1.0f, 0xffffffff }, // 7
};
static const uint16_t s_cubeIndices[] = {
    0, 1, 2, 1, 3, 2,
    4, 6, 5, 5, 6, 7,
    0, 2, 4, 4, 2, 6,
    1, 5, 3, 5, 7, 3,
    0, 4, 1, 4, 5, 1,
    2, 3, 6, 6, 3, 7,
};

bgfx::ShaderHandle loadShader(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        std::cerr << "Failed to open shader: " << filename << std::endl;
        return BGFX_INVALID_HANDLE;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    const bgfx::Memory* mem = bgfx::alloc(size+1);
    fread(mem->data, 1, size, file);
    fclose(file);
    mem->data[mem->size-1] = '\0';
    return bgfx::createShader(mem);
}

bgfx::ProgramHandle loadProgram(const char* vsName, const char* fsName) {
    bgfx::ShaderHandle vsh = loadShader(vsName);
    bgfx::ShaderHandle fsh = loadShader(fsName);
    return bgfx::createProgram(vsh, fsh, true);
}

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

  PosColorVertex::init();
  bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(
    bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
    PosColorVertex::layout
  );
  bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(
    bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices))
  );
  bgfx::ProgramHandle program = loadProgram("vs_cubes.bin", "fs_cubes.bin");

  std::cout << "launching" << std::endl;

  bool running = true;
  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) running = false;
    }
    bgfx::setViewRect(0, 0, 0, 800, 600);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xffffffff, 1.0f, 0);

    float view[16];
    float proj[16];
    const bx::Vec3 eye = {0.0f, 0.0f, -7.0f};
    const bx::Vec3 at  = {0.0f, 0.0f,  0.0f};
    const bx::Vec3 up  = {0.0f, 1.0f,  0.0f};
    bx::mtxLookAt(view, eye, at, up);
    bx::mtxProj(proj, 60.0f, float(800)/float(600), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(0, view, proj);

    static float time = 0.0f;
    time += 0.01f;
    float mtx[16];
    bx::mtxRotateXY(mtx, time, time*0.37f);

    bgfx::setTransform(mtx);
    bgfx::setVertexBuffer(0, vbh);
    bgfx::setIndexBuffer(ibh);
    bgfx::setState(BGFX_STATE_DEFAULT);
    bgfx::submit(0, program);


    // bgfx::touch(0);

    bgfx::frame();
  }

  bgfx::destroy(program);
  bgfx::destroy(vbh);
  bgfx::destroy(ibh);

  bgfx::shutdown();
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}