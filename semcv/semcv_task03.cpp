#include "semcv.hpp"

cv::Mat autocontrast(const cv::Mat &img, const double q_black,
                     const double q_white) {
  const int channels = img.channels();
  cv::Mat result = img.clone();
  const int total_pixels = img.rows * img.cols;

  std::vector<int> hist(256);
  std::vector<int> cum(256);

  for (int c = 0; c < channels; c++) {
    std::fill(hist.begin(), hist.end(), 0);

    for (int y = 0; y < img.rows; ++y) {
      const uchar *src_ptr = img.ptr<uchar>(y) + c;
      for (int x = 0; x < img.cols; ++x, src_ptr += channels) {
        hist[*src_ptr]++;
      }
    }

    cum[0] = hist[0];
    for (int i = 1; i < 256; ++i)
      cum[i] = cum[i - 1] + hist[i];

    int bth = 0;
    while (bth < 255 && cum[bth] <= total_pixels * q_black)
      ++bth;
    int wth = 255;
    while (wth > 0 && cum[wth] >= total_pixels * q_white)
      --wth;

    float scale = (wth > bth) ? 255.0f / (wth - bth) : 1.0f;
    // Обрабатываем канал
    for (int y = 0; y < result.rows; ++y) {
      uchar *dst_ptr = result.ptr<uchar>(y) + c;
      for (int x = 0; x < result.cols; ++x, dst_ptr += channels) {
        int v = (*dst_ptr - bth);
        v = std::clamp(static_cast<int>(v * scale + 0.5f), 0, 255);
        *dst_ptr = static_cast<uchar>(v);
      }
    }
  }

  return result;
}

cv::Mat autocontrast_rgb(const cv::Mat &src, double q_black, double q_white) {
  CV_Assert(src.type() == CV_8UC3);
  // Собираем общую гистограмму
  std::vector<int> hist(256), cum(256);
  int total = src.rows * src.cols * 3;
  for (int y = 0; y < src.rows; ++y) {
    const cv::Vec3b *p = src.ptr<cv::Vec3b>(y);
    for (int x = 0; x < src.cols; ++x) {
      hist[p[x][0]]++;
      hist[p[x][1]]++;
      hist[p[x][2]]++;
    }
  }
  cum[0] = hist[0];
  for (int i = 1; i < 256; ++i)
    cum[i] = cum[i - 1] + hist[i];
  int bth = 0;
  while (bth < 255 && cum[bth] <= total * q_black)
    ++bth;
  int wth = 255;
  while (wth > 0 && cum[wth] >= total * q_white)
    --wth;
  float scale = (wth > bth) ? 255.0f / (wth - bth) : 1.0f;

  // Применяем к каждому пикселю
  cv::Mat dst = src.clone();
  for (int y = 0; y < dst.rows; ++y) {
    cv::Vec3b *p = dst.ptr<cv::Vec3b>(y);
    for (int x = 0; x < dst.cols; ++x) {
      for (int c = 0; c < 3; ++c) {
        int v = static_cast<int>((p[x][c] - bth) * scale + 0.5f);
        p[x][c] = cv::saturate_cast<uchar>(v);
      }
    }
  }
  return dst;
}