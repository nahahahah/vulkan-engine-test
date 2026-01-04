#include <SDLHelpers/Window.hpp>

Window::Window(int width, int height, std::string const& label) : _label(label) {
    SDL_PropertiesID windowProperties = SDL_CreateProperties();
    SDL_SetStringProperty(windowProperties, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "Window");
    SDL_SetNumberProperty(windowProperties, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED);
    SDL_SetNumberProperty(windowProperties, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED);
    SDL_SetNumberProperty(windowProperties, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, width);
    SDL_SetNumberProperty(windowProperties, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, height);
    SDL_SetNumberProperty(windowProperties, SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    
    _handle = SDL_CreateWindowWithProperties(windowProperties);
    
    if (_handle == nullptr) {
        std::cerr << "Unable to create \"" << _label << "\" window (reason: " << SDL_GetError() << ")" << std::endl;
    }
    
    else {
        std::clog << "\"" << _label << "\" window created successfully: <SDL_Window " << _handle << ">" << std::endl; 
    }

    SDL_DestroyProperties(windowProperties);
}

Window::~Window() {
    if (_handle != nullptr) {
        SDL_DestroyWindow(_handle);
        _handle = nullptr;
        std::clog << "\"" << _label << "\" window destroyed successfully" << std::endl;
    }
}
