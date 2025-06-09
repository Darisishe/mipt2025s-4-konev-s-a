#include "semcv.hpp"

const int BOUNDING_BOX_AREA_THRES = 50;

std::vector<cv::Rect> find_blobs(const cv::Mat &input_image,
                                 int threshold,
                                 const int scale_levels,
                                 const float scale_multiplier,
                                 const float initial_sigma) {
  cv::Mat binary_img = input_image > threshold;
  cv::Mat float_img;
  binary_img.convertTo(float_img, CV_32F);

  std::vector<cv::Mat> laplacian_pyramid;
  std::vector<float> sigma_values;

  float current_sigma = initial_sigma;

  for (int level = 0; level < scale_levels; ++level) {
    int kernel_size = static_cast<int>(2 * std::ceil(3 * current_sigma) + 1);

    cv::Mat smoothed_img;
    cv::GaussianBlur(float_img, smoothed_img,
                     cv::Size(kernel_size, kernel_size), current_sigma);

    cv::Mat laplacian_img;
    cv::Laplacian(smoothed_img, laplacian_img, CV_32F);

    laplacian_img = laplacian_img * current_sigma * current_sigma;

    laplacian_pyramid.push_back(laplacian_img);
    sigma_values.push_back(current_sigma);

    current_sigma *= scale_multiplier;
  }

  std::vector<cv::Point3f> blob_centers;
  const int neighborhood_size = 3;
  const float response_threshold = 3.0f;

  for (int level = 1; level < scale_levels - 1; ++level) {
    const cv::Mat &current_level = laplacian_pyramid[level];
    const cv::Mat &previous_level = laplacian_pyramid[level - 1];
    const cv::Mat &next_level = laplacian_pyramid[level + 1];

    for (int row = neighborhood_size;
         row < current_level.rows - neighborhood_size; ++row) {
      for (int col = neighborhood_size;
           col < current_level.cols - neighborhood_size; ++col) {
        float central_value = current_level.at<float>(row, col);

        if (central_value < response_threshold)
          continue;

        bool is_local_max = true;
        for (int dr = -neighborhood_size;
             dr <= neighborhood_size && is_local_max; ++dr) {
          for (int dc = -neighborhood_size;
               dc <= neighborhood_size && is_local_max; ++dc) {
            if (dr == 0 && dc == 0)
              continue;
            if (current_level.at<float>(row + dr, col + dc) >= central_value ||
                previous_level.at<float>(row + dr, col + dc) >= central_value ||
                next_level.at<float>(row + dr, col + dc) >= central_value) {
              is_local_max = false;
              break;
            }
          }
        }

        if (is_local_max) {
          blob_centers.emplace_back(col, row, sigma_values[level]);
        }
      }
    }
  }

  std::vector<std::vector<cv::Point>> object_contours;
  cv::findContours(binary_img, object_contours, cv::RETR_EXTERNAL,
                   cv::CHAIN_APPROX_SIMPLE);

  std::vector<cv::Rect> detected_boxes;
  for (const auto &center_point : blob_centers) {
    cv::Point2f blob_center(center_point.x, center_point.y);

    for (const auto &contour : object_contours) {
      if (cv::pointPolygonTest(contour, blob_center, false) >= 0) {
        cv::Rect bounding_box = cv::boundingRect(contour);
        if (bounding_box.area() >= BOUNDING_BOX_AREA_THRES) {
          detected_boxes.push_back(bounding_box);
        }
        break;
      }
    }
  }

  std::sort(detected_boxes.begin(), detected_boxes.end(),
            [](const cv::Rect &box1, const cv::Rect &box2) {
              if (box1.x != box2.x)
                return box1.x < box2.x;
              if (box1.y != box2.y)
                return box1.y < box2.y;
              if (box1.width != box2.width)
                return box1.width < box2.width;
              return box1.height < box2.height;
            });

  auto unique_end = std::unique(detected_boxes.begin(), detected_boxes.end(),
                                [](const cv::Rect &box1, const cv::Rect &box2) {
                                  return box1.x == box2.x && box1.y == box2.y &&
                                         box1.width == box2.width &&
                                         box1.height == box2.height;
                                });
  detected_boxes.erase(unique_end, detected_boxes.end());

  return detected_boxes;
}
