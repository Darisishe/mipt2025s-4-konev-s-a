#include <exception>
#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

#include <semcv/semcv.hpp>

// Склейка изображений в один ряд
cv::Mat hconcatImgs(const std::vector<cv::Mat> &imgs) {
  cv::Mat out;
  cv::hconcat(imgs, out);
  return out;
}

// Склейка изображений в один столбец
cv::Mat vconcatImgs(const std::vector<cv::Mat> &imgs) {
  cv::Mat out;
  cv::vconcat(imgs, out);
  return out;
}

void printUsage(const char *progName) {
  std::cout << "Usage:\n"
            << "  " << progName << " <output_image> [output_histograms]\n\n"
            << "Arguments:\n"
            << "  <output_image>         Path to save the concatenated noisy "
               "images (required)\n"
            << "  [output_histograms]    Path to save the concatenated "
               "histograms (optional)\n\n"
            << "Example:\n"
            << "  " << progName << " result.png hists.png\n";
}

int main(int argc, char *argv[]) {
  const std::vector<std::array<int, 3>> SAMPLE_LEVELS = {
      {0, 127, 255}, {20, 127, 235}, {55, 127, 200}, {90, 127, 165}};
  const std::vector<int> NOISE_STDS = {3, 7, 15};

  if (argc < 2) {
    printUsage(argv[0]);

    return EXIT_FAILURE;
  }
  std::vector<cv::Mat> originals, orig_hists;
  originals.reserve(SAMPLE_LEVELS.size());
  orig_hists.reserve(SAMPLE_LEVELS.size());

  for (size_t i = 0; i < SAMPLE_LEVELS.size(); ++i) {
    const auto &lvl = SAMPLE_LEVELS[i];
    cv::Mat img = gen_tgtimg00(lvl[0], lvl[1], lvl[2]);
    originals.push_back(img);

    int bg = ((i + 1) % 2 == 0) ? 150 : 220;
    orig_hists.push_back(draw_histo(img, bg));
  }

  cv::Mat baseRow = hconcatImgs(originals);
  cv::Mat baseHistRow = hconcatImgs(orig_hists);

  std::vector<cv::Mat> imageRows = {baseRow};
  std::vector<cv::Mat> histoRows = {baseHistRow};

  int tileW = baseRow.cols / static_cast<int>(SAMPLE_LEVELS.size());
int tileH = baseRow.rows;
  for (const int& std : NOISE_STDS) {
    cv::Mat noisy = add_noise_gau(baseRow, std);

    imageRows.push_back(noisy);

    std::vector<cv::Mat> rowHists;
    rowHists.reserve(SAMPLE_LEVELS.size());
    for (size_t j = 0; j < SAMPLE_LEVELS.size(); ++j) {
      cv::Rect roi(j * tileW, 0, tileW, tileH);
      cv::Mat tile = noisy(roi);
      int bg = ((j + (&std - &NOISE_STDS[0])) % 2 == 0) ? 150 : 220;
      rowHists.push_back(draw_histo(tile, bg));
    }
    histoRows.push_back(hconcatImgs(rowHists));
  }

  cv::Mat finalImages = vconcatImgs(imageRows);

  try {
    if (!cv::imwrite(argv[1], finalImages)) {
      throw std::runtime_error("imwrite failed");
    }
  } catch (...) {
    std::cerr << "Error: can't write image to '" << argv[1] << "'\n";
    return EXIT_FAILURE;
  }

  if (argc >= 3) {
    cv::Mat finalHists = vconcatImgs(histoRows);
    try {
      if (!cv::imwrite(argv[2], finalHists)) {
        throw std::runtime_error("imwrite failed");
      }
    } catch (...) {
      std::cerr << "Error: can't write histogram to '" << argv[2] << "'\n";
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}