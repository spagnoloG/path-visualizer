#include "dataloader.h"
#include "constants.hpp"
#include <iostream>

int main() {
    try {
        Dataset dataset(DATASET_PATH);
        dataset.load_and_display_images(); 
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
