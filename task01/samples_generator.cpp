#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

#include <semcv/semcv.hpp>

#include <opencv2/opencv.hpp>

cv::Mat create_random_img(int depth, int channels) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> size_dist(10, 99);

  int height = size_dist(gen);
  int width = size_dist(gen);

  cv::Mat img(height, width, CV_MAKETYPE(depth, channels));
  img = cv::Scalar::all(0);

  return img;
}
const std::vector<int> DEPTHS = {CV_8U,  CV_8S,  CV_16U, CV_16S,
                                 CV_32S, CV_32F, CV_64F};
const std::vector<int> CHANNELS = {4, 3, 1};
const std::vector<std::string> EXTENSTIONS = {".jpg", ".png", ".tiff"};

int main() {

  std::filesystem::path prefix =
      std::filesystem::path(WORKING_DIRECTORY) / "samples";

  std::ofstream lst_file(prefix / "task01.lst");

  int c = 0;

  for (auto depth : DEPTHS) {
    for (auto ch : CHANNELS) {
      cv::Mat img = create_random_img(depth, ch);

      for (const auto &ext : EXTENSTIONS) {
        auto filename = strid_from_mat(img) + ext;
        cv::imwrite((prefix / filename).string(), img);
        lst_file << filename << std::endl;
      }
      std::cout << "Generated image №" << ++c << std::endl;
    }
  }
}