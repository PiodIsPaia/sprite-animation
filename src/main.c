#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define FRAME_WIDTH 32
#define FRAME_HEIGHT 32
#define SPRITE_SCALE 3
#define FRAME_DELAY 100

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Erro ao inicializar SDL: %s", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
      "Sprite Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (!window) {
    SDL_Log("Erro ao criar janela: %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    SDL_Log("Erro ao criar renderer: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  SDL_Surface *spriteSurface = IMG_Load("./assets/knight.png");
  if (!spriteSurface) {
    SDL_Log("Erro ao carregar imagem: %s", IMG_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }
  SDL_Texture *spriteTexture =
      SDL_CreateTextureFromSurface(renderer, spriteSurface);
  SDL_FreeSurface(spriteSurface);

  bool running = true;
  SDL_Event event;

  int frame = 0;
  Uint32 lastFrameTime = 0;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastFrameTime + FRAME_DELAY) {
      frame = (frame + 1) % 4;
      lastFrameTime = currentTime;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect srcRect = {frame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT};
    SDL_Rect dstRect = {WINDOW_WIDTH / 2 - (FRAME_WIDTH * SPRITE_SCALE) / 2,
                        WINDOW_HEIGHT / 2 - (FRAME_HEIGHT * SPRITE_SCALE) / 2,
                        FRAME_WIDTH * SPRITE_SCALE,
                        FRAME_HEIGHT * SPRITE_SCALE};

    SDL_RenderCopy(renderer, spriteTexture, &srcRect, &dstRect);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(spriteTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
