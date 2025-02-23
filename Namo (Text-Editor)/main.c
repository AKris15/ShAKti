#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "./la.h"
#include <stdbool.h>
#include "./font.h"

#define STB_IMAGE_IMPLEMENTATION


#include "stb_image.h"
#define FONT_WIDTH 128
#define FONT_HEIGHT 64
#define FONT_COLS 18
#define FONT_ROWS 7
#define FONT_CHAR_WIDTH (FONT_WIDTH / FONT_COLS)
#define FONT_CHAR_HEIGHT (FONT_HEIGHT / FONT_ROWS)

void scc(int code)
{
    if (code < 0) {
          fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
}

void *scp(void *ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
    return ptr;
}

SDL_Surface *surface_from_file(const char *file_path)
{
    int width, height, n;
    unsigned char *pixels = stbi_load(file_path, &width, &height, &n, STBI_rgb_alpha);
    
    if (pixels == NULL) {
        fprintf(stderr, "ERROR: could not load file %s: %s\n", file_path, stbi_failure_reason());
        exit(1);
    }

    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const Uint32 rmask = 0xff000000; 
    const Uint32 gmask = 0x00ff0000; 
    const Uint32 bmask = 0x0000ff00; 
    const Uint32 amask = 0x000000ff; 
    #else // little endian, like x86
    const Uint32 rmask = 0x000000ff;
    const Uint32 gmask = 0x0000ff00;
    const Uint32 bmask = 0x00ff0000;
    const Uint32 amask = 0xff000000;
    #endif

    const int depth = 32;
    const int pitch = 4 * width;

    return scp(SDL_CreateRGBSurfaceFrom((void*)pixels, width, height, depth, pitch,
                                        rmask, gmask, bmask, amask));
}

void render_char(SDL_Renderer *renderer, SDL_Texture *font, char c, Vec2f pos, Uint32  color, float scale)
{
    const size_t index = c - 32;                                                          
    const size_t col = index % FONT_COLS;
    const size_t row = index / FONT_COLS;

    const SDL_Rect src = {
      .x = col * FONT_CHAR_WIDTH,
      .y = row * FONT_CHAR_HEIGHT,
      .w = FONT_CHAR_WIDTH,
      .h = FONT_CHAR_HEIGHT,
    };/* render single character using these lines few above and few below*/
    const SDL_Rect dst = {
      .x = (int) floorf(pos.x),
      .y = (int) floorf(pos.y),
      .w = (int) floorf (FONT_CHAR_WIDTH * scale),
      .h = (int) floorf (FONT_CHAR_HEIGHT * scale),
     };
    scc (SDL_SetTextureColorMod(font,
				(color>>(8*0)) & 0xff,
				(color>>(8*1 )) & 0xff,
				(color>>(8*2)) & 0xff));


    scc (SDL_RenderCopy( renderer,
			 font,
			 &src,
			 &dst)); 
}
  
void render_text(SDL_Renderer *renderer, SDL_Texture *font, const char *text, Vec2f pos, Uint32 color, float scale)
{
  size_t n = strlen(text);

  Vec2f pen = pos;
  for(size_t i = 0; i < n;++i) {
    render_char(renderer, font, text[i], pen, color,scale);
    pen.x += FONT_CHAR_WIDTH * scale ;
       
  }
}


int main(void)
{
    scc(SDL_Init(SDL_INIT_VIDEO));

    SDL_Window *window = scp(SDL_CreateWindow("NAMO", 0, 0, 800, 600,
                                              SDL_WINDOW_RESIZABLE)); /* WINDOW SIZE */
    SDL_Renderer *renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

    SDL_Surface *font_surface = surface_from_file("./charmap-oldschool_white.png");
    SDL_Texture *font_texture = scp(SDL_CreateTextureFromSurface(renderer,font_surface));


    bool quit = false;
    while (!quit) {
        SDL_Event event = {0};
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }
        scc(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0));
        scc(SDL_RenderClear(renderer));

	render_text(renderer, font_texture,"I AM BATMAN", (Vec2f){0.0, 0.0}, 0xFF0000FF, 5.0f);
        SDL_RenderPresent(renderer);
 
    }
   

    SDL_Quit();
    return 0;
 }
