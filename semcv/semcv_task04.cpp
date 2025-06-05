#include "semcv.hpp"
#include <random>

static std::mt19937 engine((std::random_device()()));

inline void initialize_rng(unsigned int seed) { engine.seed(seed); }

int random_between(int min_val, int max_val) {
  std::uniform_int_distribution<int> dist(min_val, max_val);
  return dist(engine);
}

std::vector<cv::Rect> find_ellipses(const cv::Mat& image, int threshold_value, int kernel_size) {
    cv::Mat thresholded_image;
    cv::threshold(image, thresholded_image, threshold_value, 255, cv::THRESH_BINARY);

    // Применение морфологических операций
    cv::Mat structuring_element = cv::getStructuringElement(cv::MORPH_ELLIPSE, 
                                                          cv::Size(kernel_size, kernel_size));
    cv::morphologyEx(thresholded_image, thresholded_image, cv::MORPH_OPEN, structuring_element);
    cv::morphologyEx(thresholded_image, thresholded_image, cv::MORPH_CLOSE, structuring_element);

    // Поиск компонент связности
    cv::Mat labels, stats, centroids;
    int component_count = cv::connectedComponentsWithStats(thresholded_image, labels, stats, centroids);

    std::vector<cv::Rect> detected_bboxes;

    for (int i = 1; i < component_count; i++) {
        int x = stats.at<int>(i, cv::CC_STAT_LEFT);
        int y = stats.at<int>(i, cv::CC_STAT_TOP);
        int w = stats.at<int>(i, cv::CC_STAT_WIDTH);
        int h = stats.at<int>(i, cv::CC_STAT_HEIGHT);
        detected_bboxes.push_back(cv::Rect(x, y, w, h));
    }

    return detected_bboxes;
}


static std::pair<cv::Mat, cv::Rect> build_ellipse_cell(const Json::Value &cfg) {
  int cell_size = cfg["cell_size"].asInt();
  int cell_margin = cfg["cell_margin"].asInt();
  auto ellipse_params = cfg["ellipse"];

  cv::Mat cell_img = cv::Mat::zeros(cell_size, cell_size, CV_8UC1);

  int semi_major = random_between(ellipse_params["axis_low"].asInt(),
                                  ellipse_params["axis_high"].asInt()) /
                   2;
  int semi_minor = random_between(ellipse_params["axis_low"].asInt(),
                                  ellipse_params["axis_high"].asInt()) /
                   2;
  cv::Size ellipse_axes(semi_major, semi_minor);

  int max_axis = std::max(semi_major, semi_minor);
  int min_center = cell_margin + max_axis;
  int max_center = cell_size - cell_margin - max_axis;

  int center_x = random_between(min_center, max_center);
  int center_y = random_between(min_center, max_center);
  cv::Point ellipse_center(center_x, center_y);

  int max_thickness = std::max(1, std::min(semi_major, semi_minor) / 4);
  int thickness = random_between(1, max_thickness);
  int rotation_angle = random_between(0, 180);
  int intensity = random_between(ellipse_params["intensity_low"].asInt(),
                                 ellipse_params["intensity_high"].asInt());

  cv::ellipse(cell_img, ellipse_center, ellipse_axes, rotation_angle, 0, 360,
              cv::Scalar(intensity), -1);

  cv::Mat non_zero_points;
  cv::findNonZero(cell_img, non_zero_points);
  cv::Rect bounding_box = cv::boundingRect(non_zero_points);

  std::normal_distribution<double> blur_distribution(
      cfg["blur_mean"].asDouble(), cfg["blur_std"].asDouble());
  int blur_kernel_size =
      std::max(1, static_cast<int>(std::round(blur_distribution(engine))));
  if (blur_kernel_size % 2 == 0) {
    blur_kernel_size++;
  }
  cv::GaussianBlur(cell_img, cell_img,
                   cv::Size(blur_kernel_size, blur_kernel_size), 0);

  return std::make_pair(cell_img, bounding_box);
}

std::pair<cv::Mat, std::vector<cv::Rect>>
generate_ellipse_grid(const Json::Value &config, int seed) {
  initialize_rng(seed);

  int grid_cell_size = config["cell_size"].asInt();
  int grid_dimension = config["grid_size"].asInt();

  // Создание итогового изображения
  cv::Mat output_image =
      cv::Mat::zeros(grid_cell_size * grid_dimension,
                     grid_cell_size * grid_dimension, CV_8UC1);
  std::vector<cv::Rect> ellipse_bboxes;

  // Заполнение сетки ячейками с эллипсами
  for (int row = 0; row < grid_dimension; row++) {
    for (int col = 0; col < grid_dimension; col++) {
      auto ellipse_data = build_ellipse_cell(config);
      cv::Mat cell_image = ellipse_data.first;
      cv::Rect bbox = ellipse_data.second;

      // Копирование ячейки в итоговое изображение
      cv::Rect cell_roi(col * grid_cell_size, row * grid_cell_size,
                        grid_cell_size, grid_cell_size);
      cell_image.copyTo(output_image(cell_roi));

      bbox.x += col * grid_cell_size;
      bbox.y += row * grid_cell_size;
      ellipse_bboxes.push_back(bbox);
    }
  }

  // Добавление шума
  cv::Mat noise_image(output_image.size(), CV_8UC1);
  cv::randu(noise_image, cv::Scalar(config["noise_low"].asInt()),
            cv::Scalar(config["noise_high"].asInt()));
  cv::add(output_image, noise_image, output_image);

  return std::make_pair(output_image, ellipse_bboxes);
}
