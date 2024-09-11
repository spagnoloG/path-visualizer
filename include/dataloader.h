#ifndef DATALOADER_H
#define DATALOADER_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>

class Dataset {
public:
    Dataset(const std::string& data_path, int size_x = 1920, int size_y = 1080, bool grayscale = false);

    void load_and_display_images() const;

private:
    std::string data_path_;
    int size_x_, size_y_;
    bool grayscale_;
    std::vector<std::string> image_paths_;
    std::vector<nlohmann::json> metadata_;

    void load_image_paths();
    void load_metadata();
    cv::Mat get_image(size_t index) const;
    void display_metadata(size_t index) const;
};

#endif // DATALOADER_H
