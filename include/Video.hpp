#include <SDL2/SDL.h>

class Video{
    static void AudioCallback(void *UserData, Uint8 *Buffer, int Len);
    private:
        int Height;
        int Width;
        int Dalay;
        int PixelH;
        int PixelW;
        SDL_Window *Window;
        SDL_Renderer *Renderer;
        SDL_AudioSpec AudioSpec;
    public:
        Video(int Hight, int Width, int Dalay, int PixelH, int PixelW);
        ~Video();
        int DrawPixel(int CordX, int CordY);
        int DrawScreen(uint8_t **Display);
};