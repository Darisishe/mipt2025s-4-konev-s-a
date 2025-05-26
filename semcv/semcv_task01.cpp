#include "semcv.hpp"

#include <cmath>

std::string depth_as_str(const cv::Mat &img) {
  int depth = img.depth();
  switch (depth) {
  case CV_8U:
    return "uint08";
  case CV_8S:
    return "sint08";
  case CV_16U:
    return "uint16";
  case CV_16S:
    return "sint16";
  case CV_32S:
    return "sint32";
  case CV_32F:
    return "real32";
  case CV_64F:
    return "real64";
  default:
    throw std::invalid_argument("Unknown image depth!");
  }
}

std::string strid_from_mat(const cv::Mat &img, const int n) {
  auto height = img.size().height;
  auto width = img.size().width;
  auto channels = img.channels();

  if (std::max(height, width) >= std::pow(10, n)) {
    throw std::invalid_argument("Image size is too large for n = " +
                                std::to_string(n));
  }

  auto width_str = std::to_string(width);
  auto width_len = width_str.length();
  for (int i = 0; i < n - width_len; i++) {
    width_str = "0" + width_str;
  }

  auto height_str = std::to_string(height);
  auto height_len = height_str.length();
  for (int i = 0; i < n - height_len; i++) {
    height_str = "0" + height_str;
  }

  auto channelsStr = std::to_string(channels);

  auto typeStr = depth_as_str(img);

  return width_str + "x" + height_str + "." + std::to_string(channels) + "." +
         depth_as_str(img);
}

using std::filesystem::path;

std::vector<path> get_list_of_file_paths(const path &path_lst) {
  std::vector<path> file_paths;

  std::string line;
  std::ifstream lst_file(path_lst);

  while (std::getline(lst_file, line)) {
    file_paths.push_back(path_lst.parent_path() / line);
  }

  return file_paths;
}
