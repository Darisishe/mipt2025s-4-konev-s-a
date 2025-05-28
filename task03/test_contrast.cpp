#include <semcv/semcv.hpp>

// Параметры для автоконтраста
const double Q_BLACK = 0.1;
const double Q_WHITE = 0.9;

const std::vector<std::string> CHANNELS = {"blue", "green", "red"};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_image>\n";
    return EXIT_FAILURE;
  }

  const auto baseDir = std::filesystem::path(WORKING_DIRECTORY) / "testing";
  const std::string inputPath = argv[1];
  const auto srcPath = baseDir / inputPath;

  auto filename = srcPath.filename().string();

  cv::Mat tmp = cv::imread(srcPath.string(), cv::IMREAD_UNCHANGED);
  if (tmp.empty()) {
    std::cerr << "Error: cannot load '" << srcPath << "'\n";
    return EXIT_FAILURE;
  }

  int flag = (tmp.channels() == 1) ? cv::IMREAD_GRAYSCALE : cv::IMREAD_COLOR;
  cv::Mat img = cv::imread(srcPath.string(), flag);
  if (img.empty()) {
    std::cerr << "Error: cannot reload '" << srcPath << "' with flag\n";
    return EXIT_FAILURE;
  }

  for (int c = 0; c < img.channels(); ++c) {
    std::string prefix = "hist_original_";
    if (img.channels() > 1) {
      prefix = "hist_" + CHANNELS[c] + "_original_";
    }

    cv::imwrite((baseDir / (prefix + filename)).string(),
                draw_histo(img, 255, c));
  }

  const std::string tag = std::to_string(int(Q_BLACK * 100)) + "-" +
                          std::to_string(int(Q_WHITE * 100));

  cv::Mat mono = autocontrast(img, Q_BLACK, Q_WHITE);
  cv::imwrite((baseDir / ("autocont_" + tag + "_" + filename)).string(), mono);
  for (int c = 0; c < img.channels(); ++c) {
    std::string prefix = "hist_autocont_mono_";
    if (img.channels() > 1) {
      prefix = "hist_" + CHANNELS[c] + "_autocont_mono_";
    }

    cv::imwrite((baseDir / (prefix + tag + "_" + filename)).string(),
                draw_histo(mono, 255, c));
  }

  if (img.channels() == 3) {
    cv::Mat color = autocontrast_rgb(img, Q_BLACK, Q_WHITE);
    cv::imwrite(
        (baseDir / ("autocont_colored_" + tag + "_" + filename)).string(),
        color);
    for (int c = 0; c < CHANNELS.size(); ++c) {

      cv::imwrite((baseDir / ("hist_" + CHANNELS[c] + "_autocont_colored_" +
                              tag + "_" + filename))
                      .string(),
                  draw_histo(color, 255, c));
    }
  }

  return EXIT_SUCCESS;
}
