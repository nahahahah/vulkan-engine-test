#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <memory>

#include "Application.hpp"

int main(int argc, char* argv[]) {
    (void)(argc);
    (void)(argv);

    std::unique_ptr<Application> app = nullptr;

    try {
        app = std::make_unique<Application>();
        app->Run();        
    }

    catch (std::exception const& err) {
        std::cerr << err.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
