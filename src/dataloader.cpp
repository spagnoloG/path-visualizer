#include "dataloader.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;
using json = nlohmann::json;

Dataset::Dataset(const std::string& data_path, int size_x, int size_y, bool grayscale)
    : data_path_(data_path), size_x_(size_x), size_y_(size_y), grayscale_(grayscale) {
    load_image_paths();
    load_metadata();
}

void Dataset::load_and_display_images() const {
    if (image_paths_.empty() || metadata_.empty()) {
        throw std::runtime_error("Image paths or metadata is empty, cannot load images.");
    }

    for (size_t i = 0; i < image_paths_.size(); ++i) {
        cv::Mat image = get_image(i);  // Use the get_image method to retrieve the image
        cv::imshow("Image", image);
        display_metadata(i);
        cv::waitKey(0);
    }
}

cv::Mat Dataset::get_image(size_t index) const {
    if (index >= image_paths_.size()) {
        throw std::out_of_range("Index out of range for image paths.");
    }

    cv::Mat image;
    if (grayscale_) {
        image = cv::imread(image_paths_[index], cv::IMREAD_GRAYSCALE);
    } else {
        image = cv::imread(image_paths_[index], cv::IMREAD_COLOR);
    }

    if (image.empty()) {
        throw std::runtime_error("Could not open or find the image: " + image_paths_[index]);
    }

    if (image.cols != size_x_ || image.rows != size_y_) {
        cv::resize(image, image, cv::Size(size_x_, size_y_));
    }

    return image;
}

void Dataset::load_image_paths() {
    std::string images_folder = data_path_ + "/footage/";

    if (!fs::exists(images_folder)) {
        throw std::runtime_error("Footage folder does not exist at: " + images_folder);
    }

    for (const auto& entry : fs::directory_iterator(images_folder)) {
        if (entry.path().extension() == ".jpeg") {
            image_paths_.push_back(entry.path().string());
        }
    }

    if (image_paths_.empty()) {
        throw std::runtime_error("No images found in the directory: " + images_folder);
    }

    std::sort(image_paths_.begin(), image_paths_.end());
}

void Dataset::load_metadata() {
    std::string metadata_file;
    
    for (const auto& entry : fs::directory_iterator(data_path_)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            metadata_file = entry.path().string();
            break;
        }
    }

    if (metadata_file.empty()) {
        throw std::runtime_error("Could not find any JSON file in the directory: " + data_path_);
    }

    std::ifstream file(metadata_file);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open metadata file at: " + metadata_file);
    }

    json metadata_json;
    try {
        file >> metadata_json;
        metadata_ = metadata_json.at("cameraFrames").get<std::vector<json>>();
    } catch (const json::exception& e) {
        throw std::runtime_error("Error parsing metadata file: " + std::string(e.what()));
    }

    if (metadata_.empty()) {
        throw std::runtime_error("Metadata file contains no frames.");
    }

    std::cout << "Successfully loaded metadata from: " << metadata_file << std::endl;
}

void Dataset::display_metadata(size_t idx) const {
    if (idx >= metadata_.size()) {
        throw std::out_of_range("Index out of bounds for metadata access.");
    }

    const auto& meta = metadata_[idx];
    if (meta.contains("coordinate")) {
        std::cout << "Metadata for image " << idx << ": " << std::endl;
        std::cout << "Latitude: " << meta["coordinate"]["latitude"] << std::endl;
        std::cout << "Longitude: " << meta["coordinate"]["longitude"] << std::endl;
    } else {
        std::cerr << "Metadata missing coordinate information for image " << idx << std::endl;
    }
}

