#include "semcv.hpp"

const int SQUARE_DIMENSION = 127;
const int CIRCLE_SIZE = 40;
const int GRID_WIDTH = 3;
const int GRID_HEIGHT = 2;
const int TOTAL_WIDTH = SQUARE_DIMENSION * GRID_WIDTH;
const int TOTAL_HEIGHT = SQUARE_DIMENSION * GRID_HEIGHT;

void render_square_with_circle(cv::Mat &canvas, int left, int top,
                               int square_shade, int circle_shade) {
  cv::rectangle(canvas, cv::Point(left, top),
                cv::Point(left + SQUARE_DIMENSION, top + SQUARE_DIMENSION),
                square_shade, cv::FILLED);
  int mid_x = left + SQUARE_DIMENSION / 2;
  int mid_y = top + SQUARE_DIMENSION / 2;
  cv::circle(canvas, cv::Point(mid_x, mid_y), CIRCLE_SIZE, circle_shade,
             cv::FILLED);
}

cv::Mat create_squares_with_circles_image() {
  cv::Mat output(TOTAL_HEIGHT, TOTAL_WIDTH, CV_8UC1, cv::Scalar(0));

  std::vector<std::pair<int, int>> shades = {{0, 127},   {127, 0},   {255, 0},
                                             {127, 255}, {255, 127}, {0, 255}};

  for (size_t idx = 0; idx < shades.size(); ++idx) {
    int x_coord = (idx % GRID_WIDTH) * SQUARE_DIMENSION;
    int y_coord = (idx / GRID_WIDTH) * SQUARE_DIMENSION;
    render_square_with_circle(output, x_coord, y_coord, shades[idx].first,
                              shades[idx].second);
  }

  return output;
}

cv::Mat adjust_for_display(const cv::Mat &source) {
  cv::Mat adjusted;
  double min_level, max_level;
  cv::minMaxLoc(source, &min_level, &max_level);

  if (max_level > min_level) {
    adjusted = (source - min_level) * (255.0 / (max_level - min_level));
  } else {
    adjusted = source.clone();
  }

  adjusted.convertTo(adjusted, CV_8UC1);
  return adjusted;
}

cv::Mat apply_filters_and_create_collage(const cv::Mat &input) {
  cv::Mat base_image;
  input.convertTo(base_image, CV_32F);

  cv::Mat filter1 = (cv::Mat_<float>(2, 2) << -1, 0, 0, 1);
  cv::Mat filter2 = (cv::Mat_<float>(2, 2) << 0, -1, 1, 0);

  cv::Mat filtered1, filtered2;
  cv::filter2D(base_image, filtered1, CV_32F, filter1);
  cv::filter2D(base_image, filtered2, CV_32F, filter2);

  cv::Mat amplitude;
  cv::magnitude(filtered1, filtered2, amplitude);

  cv::Mat disp_filtered1 = adjust_for_display(filtered1);
  cv::Mat disp_filtered2 = adjust_for_display(filtered2);
  cv::Mat disp_amplitude = adjust_for_display(amplitude);

  cv::Mat disp_color;
  std::vector<cv::Mat> layers = {disp_amplitude, disp_filtered2,
                                 disp_filtered1};
  cv::merge(layers, disp_color);

  cv::Mat disp_filtered1_bgr, disp_filtered2_bgr, disp_amplitude_bgr,
      disp_color_bgr;
  cv::cvtColor(disp_filtered1, disp_filtered1_bgr, cv::COLOR_GRAY2BGR);
  cv::cvtColor(disp_filtered2, disp_filtered2_bgr, cv::COLOR_GRAY2BGR);
  cv::cvtColor(disp_amplitude, disp_amplitude_bgr, cv::COLOR_GRAY2BGR);
  disp_color.copyTo(disp_color_bgr);

  cv::Mat row_upper, row_lower;
  cv::hconcat(disp_filtered1_bgr, disp_filtered2_bgr, row_upper);
  cv::hconcat(disp_amplitude_bgr, disp_color_bgr, row_lower);
  cv::Mat final_collage;
  cv::vconcat(row_upper, row_lower, final_collage);

  return final_collage;
}