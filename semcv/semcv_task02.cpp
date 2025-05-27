#include "semcv.hpp"

cv::Mat gen_tgtimg00(const int lev0, const int lev1, const int lev2) {
  const int IMAGE_SIZE = 256;
  const int INTERNAL_SIZE = 209;
  const int RADIUS = 83;

  cv::Mat result(IMAGE_SIZE, IMAGE_SIZE, CV_8UC1, cv::Scalar(lev0));

  cv::Point img_center(IMAGE_SIZE / 2, IMAGE_SIZE / 2);

  cv::Rect square(img_center.x - INTERNAL_SIZE / 2,
                  img_center.y - INTERNAL_SIZE / 2, INTERNAL_SIZE,
                  INTERNAL_SIZE);

  cv::rectangle(result, square, cv::Scalar(lev1), cv::FILLED);

  cv::circle(result, img_center, RADIUS, cv::Scalar(lev2), cv::FILLED);

  return result;
}

cv::Mat add_noise_gau(const cv::Mat &src, const int std) {
  cv::Mat src16;
  src.convertTo(src16, CV_16S);

  cv::Mat noise16(src.size(), CV_16S);
  cv::randn(noise16, 0, std);

  cv::Mat sum16;
  cv::add(src16, noise16, sum16, cv::noArray(), CV_16S);

  cv::Mat result;
  sum16.convertTo(result, CV_8U);

  return result;
}

std::pair<float, float> calc_distribution_params(const cv::Mat &img,
                                                 const cv::Mat &mask) {
  cv::Scalar mean_val, std_val;
  cv::meanStdDev(img, mean_val, std_val, mask);

  return std::make_pair((float)(mean_val[0]), (float)(std_val[0]));
}

cv::Mat draw_histo(const cv::Mat &img, const int background_value) {
  cv::Mat hist;
  const int bins = 256;
  float range[] = {0.0f, 256.0f};
  const float *ranges[] = {range};
  int channels[] = {0};
  cv::calcHist(&img, 1, channels, cv::Mat(), hist, 1, &bins, ranges);

  // 2) Нормализуем так, чтобы максимум стал 250
  double minVal, maxVal;
  cv::minMaxLoc(hist, &minVal, &maxVal);
  if (maxVal > 0) {
    hist *= (250.0 / maxVal);
  }

  cv::Mat histImage(256, 256, CV_8UC1, cv::Scalar(background_value));

  // 4) Рисуем чёрные столбцы шириной 1 пиксель
  for (int i = 0; i < bins; ++i) {
    int h = static_cast<int>(hist.at<float>(i));
    if (h > 0) {
      cv::line(histImage, cv::Point(i, 255), cv::Point(i, 255 - h),
               cv::Scalar(0), 1);
    }
  }

  return histImage;
}