#include <SDL2/SDL.h>

class Video{
    private:
        int Height;
        int Width;
        int Dalay;
        int PixelH;
        int PixelW;
        SDL_Window *Window;
        SDL_Renderer *Renderer;
    public:
        Video(int Hight, int Width, int Dalay, int PixelH, int PixelW);
        ~Video();
        int DrawPixel(int CordX, int CordY);
        int DrawScreen(uint8_t **Display);
};