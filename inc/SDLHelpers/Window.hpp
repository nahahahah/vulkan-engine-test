#ifndef SDL_HELPERS_WINDOW_HPP
#define SDL_HELPERS_WINDOW_HPP

#include <iostream>
#include <cassert>

#include <SDL3/SDL.h>

class Window {
    public:
        Window() = default;
        Window(int width, int height, std::string const& label);
        ~Window();

        std::string Label() { return _label; }
        void Label(std::string const& label) { _label = label; }

        SDL_Window* Handle() { return _handle; }
        SDL_Window* Handle() const { return _handle; }

    private:
        std::string _label = "";
        SDL_Window* _handle = nullptr;
};

#endif // SDL_HELPERS_WINDOW_HPP
