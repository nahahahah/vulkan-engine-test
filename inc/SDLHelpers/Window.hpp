#ifndef SDL_HELPERS_WINDOW_HPP
#define SDL_HELPERS_WINDOW_HPP

#include <iostream>

#include <SDL3/SDL.h>

class Window {
    public:
        Window();
        ~Window();

        SDL_Window* Handle() {
            return _handle;
        }

        SDL_Window* Handle() const {
            return _handle;
        }

    private:
        SDL_Window* _handle = nullptr;
};

#endif // SDL_HELPERS_WINDOW_HPP
