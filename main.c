#include <SDL2/SDL.h>
#include <stdio.h>

void move_box(int **buffer, unsigned int width, unsigned int height, unsigned int bgcolor,
              unsigned int posx, unsigned int posy, unsigned int box_width, unsigned int box_height, unsigned int fgcolor)
{
  // memset can only be used with byte values and we use 4-byte color values
  for (unsigned int row = 0; row < height; row++)
  {
    for (unsigned int col = 0; col < width; col++)
    {
      (*buffer)[row * width + col] = bgcolor;
    }
  }

  for (int row = posy; row < box_height + posy; row++)
  {
    unsigned int start = row * width + posx;
    for (unsigned int pos = start; pos < start + box_width; pos++)
    {
      (*buffer)[pos] = fgcolor;
    }
  }
}

int main(int argc, char *argv[])
{
  unsigned int width = 2048;
  unsigned int height = 2048;
  unsigned int box_size = 1000;
  SDL_bool quit = SDL_FALSE;
  unsigned int boxX = width / 2 - box_size / 2;
  unsigned int boxY = height / 2 - box_size / 2;
  int *buffer = (int *)malloc(width * height * sizeof(int));

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow("Double Buffer Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                        width, height, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
  int pitch = 4 * width;

  Uint32 prev_frame_time = 0, curr_frame_time = 0;
  float deltaTime = 0.0;
  Uint32 frame_count = 0;

  while (!quit)
  {
    prev_frame_time = curr_frame_time;
    curr_frame_time = SDL_GetTicks();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {

      if (event.type == SDL_QUIT)
      {
        quit = SDL_TRUE;
      }
      else if (event.type == SDL_KEYDOWN)
      {
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
          quit = SDL_TRUE;
          break;
        case SDLK_UP:
          boxY -= 5;
          break;
        case SDLK_DOWN:
          boxY += 5;
          break;
        case SDLK_LEFT:
          boxX -= 5;
          break;
        case SDLK_RIGHT:
          boxX += 5;
          break;
        }
      }
    }
    SDL_RenderClear(renderer);
    move_box(&buffer, width, height, 0xFFFFFFFF, boxX, boxY, box_size, box_size, 0xFF000000);
    SDL_LockTexture(tex, NULL, (void **)(&buffer), &pitch);

    SDL_UnlockTexture(tex);

    SDL_RenderCopy(renderer, tex, NULL, NULL);
    SDL_RenderPresent(renderer);

    Uint32 frame_time = curr_frame_time - prev_frame_time;
    frame_count++;
    if (frame_count % 100 == 0)
    {
      printf("Frame %d, Frame Time: %u milliseconds\n", frame_count, frame_time);
    }
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}