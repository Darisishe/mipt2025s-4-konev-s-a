#include <iostream>

#include <semcv/semcv.hpp>

#include <opencv2/opencv.hpp>

int main(int argc, char **argv) {
  std::string lst_path;
  if (argc != 2) {
    lst_path = std::filesystem::path(WORKING_DIRECTORY) / "samples/task01.lst";
  } else {
    lst_path = std::filesystem::path(argv[1]);
  }

  auto files = get_list_of_file_paths(lst_path);

  for (const auto &file : files) {
    cv::Mat img = cv::imread(file.string(), cv::IMREAD_UNCHANGED);
    auto proper_name = strid_from_mat(img);
    if (proper_name == file.stem().string()) {
      std::cout << file.filename().string() << "\tgood" << std::endl;
    } else {
      std::cout << file.filename().string() << "\tbad, should be "
                << proper_name << std::endl;
    }
  }
}