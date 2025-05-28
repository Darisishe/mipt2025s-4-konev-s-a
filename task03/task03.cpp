#include <exception>
#include <iostream>

#include <opencv2/opencv.hpp>

#include <semcv/semcv.hpp>

int main(int argc, char *argv[]) {
  if (argc != 6) {
    std::cerr << "Usage: " << argv[0]
              << " <mode> <input> <q_black> <q_white> <output>\n";
    return EXIT_FAILURE;
  }
  const std::string mode = argv[1];
  const std::string inPathStr = argv[2];
  const double qBlack = std::strtod(argv[3], nullptr);
  const double qWhite = std::strtod(argv[4], nullptr);
  const std::string outPathStr = argv[5];

  cv::Mat temp = cv::imread(inPathStr, cv::IMREAD_UNCHANGED);
  if (temp.empty()) {
    std::cerr << "Error: could not open '" << inPathStr << "'.\n";
    return EXIT_FAILURE;
  }

  // Выбираем режим чтения: gray или color
  int readFlag =
      (temp.channels() == 1) ? cv::IMREAD_GRAYSCALE : cv::IMREAD_COLOR;
  cv::Mat src = cv::imread(inPathStr, readFlag);
  if (src.empty()) {
    std::cerr << "Error: failed to reload '" << inPathStr << "' with flag.\n";
    return EXIT_FAILURE;
  }

  cv::Mat result;
  try {
    if (mode == "naive") {
      result = autocontrast(src, qBlack, qWhite);
    } else if (mode == "rgb") {
      result = autocontrast_rgb(src, qBlack, qWhite);
    } else {
      std::cerr << "Error: unknown mode '" << mode
                << "'. Use 'naive' or 'rgb'.\n";
      return EXIT_FAILURE;
    }
  } catch (const std::exception &e) {
    std::cerr << "Exception during contrast: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  if (!cv::imwrite(outPathStr, result)) {
    std::cerr << "Error: could not write to '" << outPathStr << "'.\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}