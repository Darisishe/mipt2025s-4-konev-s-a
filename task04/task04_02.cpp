#include <fstream>
#include <iostream>

#include <semcv/semcv.hpp>

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: " << argv[0] << " <image_path> <markup_path> <marked_image_path>\n";
    return 1;
  }

  auto image_path = argv[1];
  auto output_path = argv[2];
  auto marked_path = argv[3];

  cv::Mat gray = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
  if (gray.empty()) {
    std::cerr << "Error: cannot open or read image '" << image_path << "'\n";
    return EXIT_FAILURE;
  }

  int max_value_in_first_row = 0;
  for (int x = 0; x < gray.cols; ++x) {
    max_value_in_first_row = std::max(max_value_in_first_row,
                                      static_cast<int>(gray.at<uchar>(0, x)));
  }

  std::vector<cv::Rect> boxes =
      find_ellipses(gray, max_value_in_first_row, 3);

  cv::Mat colored;
  cv::cvtColor(gray, colored, cv::COLOR_GRAY2BGR);

  std::ofstream ofs(output_path);
  if (!ofs.is_open()) {
    std::cerr << "Error: cannot write to '" << output_path << "'\n";
    return EXIT_FAILURE;
  }

  for (const auto &r : boxes) {
    ofs << r.x << ' ' << r.y << ' ' << r.width << ' ' << r.height << '\n';
    cv::rectangle(colored, r, cv::Scalar(0, 255, 0), 1);
  }
  ofs.close();

  if (!cv::imwrite(marked_path, colored)) {
    std::cerr << "Error: failed to save marked image to '" << marked_path
              << "'\n";
    return EXIT_FAILURE;
  }
}