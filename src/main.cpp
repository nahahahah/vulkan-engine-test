#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cassert>
#include <iomanip>
#include <optional>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <fstream>
#include <filesystem>

#define SDL_MAIN_HANDLED
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <vulkan/vulkan.h>

#include "Application.hpp"

int main(int argc, char* argv[]) {
    (void)(argc);
    (void)(argv);

    auto app = Application();

    try {
        app.Run();        
    }

    catch (std::exception const& err) {
        std::cerr << err.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
