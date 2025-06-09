#include <filesystem>
#include <fstream>
#include <json/json.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

// Task 1
std::string strid_from_mat(const cv::Mat &img, const int n = 4);
std::vector<std::filesystem::path>
get_list_of_file_paths(const std::filesystem::path &path_lst);

// Task 2
cv::Mat gen_tgtimg00(const int lev0, const int lev1, const int lev2);
cv::Mat add_noise_gau(const cv::Mat &img, const int std);
cv::Mat draw_histo(const cv::Mat &img, const int background_value = 220,
                   const int channel = 0);
std::pair<float, float> calc_distribution_params(const cv::Mat &img,
                                                 const cv::Mat &mask);

// Task 3
cv::Mat autocontrast(const cv::Mat &img, const double q_black,
                     const double q_white);

cv::Mat autocontrast_rgb(const cv::Mat &img, const double q_black,
                         const double q_white);

// Task 4
std::pair<cv::Mat, std::vector<cv::Rect>>
generate_ellipse_grid(const Json::Value &config, int seed);

std::vector<cv::Rect> find_ellipses(const cv::Mat &image, int threshold,
                                    int kernel_size = 3);

// Task 5
cv::Mat create_squares_with_circles_image();

cv::Mat apply_filters_and_create_collage(const cv::Mat &image);

// Task 6
std::vector<cv::Rect> find_blobs(const cv::Mat &input_image,
                                 int threshold,
                                 const int scale_levels = 20,
                                 const float scale_multiplier = 1.5f,
                                 const float initial_sigma = 1.5f);
