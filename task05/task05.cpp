#include <iostream>
#include <opencv2/opencv.hpp>
#include <semcv/semcv.hpp>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0]
              << " <squared_circles_image_path> <collage_path>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string image_path(argv[1]);
  cv::Mat image = create_squares_with_circles_image();

  std::string collage_path(argv[2]);
  cv::Mat collage = apply_filters_and_create_collage(image);

  if (!cv::imwrite(image_path, image)) {
    std::cerr << "Error: Failed to save image to " << image_path << std::endl;
    return EXIT_FAILURE;
  }

  if (!cv::imwrite(collage_path, collage)) {
    std::cerr << "Error: Failed to save image to " << collage_path << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}