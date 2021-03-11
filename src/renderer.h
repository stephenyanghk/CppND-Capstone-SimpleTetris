#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"
#include <string>

#define BOARD_POSITION_X 2
#define BOARD_POSITION_Y 2
#define BOARD_WIDTH 10	
#define BOARD_HEIGHT 20	

class Board
{
    public:
        bool isFree() const { return is_free; }
        void isFree(bool free) { is_free = free; }

        void setRGBA(int r, int g, int b, int a) {
            colorR = r;
            colorG = g;
            colorB = b;
            colorA = a;
        }

        Uint8 R() { return colorR; }
        Uint8 G() { return colorG; }
        Uint8 B() { return colorB; }
        Uint8 A() { return colorA; }

    private:
        bool is_free {true};
        Uint8 colorR {0};
        Uint8 colorG {0};
        Uint8 colorB {0};
        Uint8 colorA {0};
};

class Renderer {
    public:
        Renderer(const std::string title, const std::size_t screen_width, const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height);
        ~Renderer();

        void ClearScreen()
        {
            SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 0);
  	        SDL_RenderClear(sdl_renderer);
        }

        void DrawWhiteRect(int x, int y)
        {
            SDL_Rect block;
  	        block.w = screen_width / grid_width;
  	        block.h = screen_height / grid_height;
            block.x = x * screen_width / grid_width;
            block.y = y * screen_height / grid_height;
            SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(sdl_renderer, &block);
        }

        void DrawRect(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
        {
            SDL_Rect block;
  	        block.w = screen_width / grid_width - 1;
  	        block.h = screen_height / grid_height - 1;
            block.x = x * screen_width / grid_width + 1;
            block.y = y * screen_height / grid_height + 1;
            SDL_SetRenderDrawColor(sdl_renderer, r, g, b, a);
            SDL_RenderFillRect(sdl_renderer, &block);
        }

        void DrawRectRange(int x, int y, int width, int height, Board bd[BOARD_WIDTH][BOARD_HEIGHT])
        {
            SDL_Rect block;
  	        block.w = screen_width / grid_width - 1;
  	        block.h = screen_height / grid_height - 1;

            for (int i=0; i<width; ++i)
            {
                for (int j=0; j<height; ++j)
                {
                    block.x = (x+i) * screen_width / grid_width + 1;
                    block.y = (y+j) * screen_height / grid_height + 1;
                    SDL_SetRenderDrawColor(sdl_renderer, bd[i][j].R(), bd[i][j].G(), bd[i][j].B(), bd[i][j].A());
                    SDL_RenderFillRect(sdl_renderer, &block);
                }
            }
        }

        void UpdateScreen() { SDL_RenderPresent(sdl_renderer); }

        void UpdateWindowTitle(int level, int score);
        void UpdateWindowTitle(std::string &&s);

    private:
        SDL_Window *sdl_window;
        SDL_Renderer *sdl_renderer;

        const std::string window_title;
        const std::size_t screen_width;
        const std::size_t screen_height;
        const std::size_t grid_width;
        const std::size_t grid_height;
};

#endif // RENDERER_H