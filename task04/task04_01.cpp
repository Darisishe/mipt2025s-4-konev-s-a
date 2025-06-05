
#include <iostream>
#include <fstream>
#include <filesystem>

#include <json/json.h>
#include <semcv/semcv.hpp>

Json::Value parse_сonfig(const std::filesystem::path &cfg_path) {
  std::ifstream in(cfg_path);
  if (!in.is_open()) {
    throw std::runtime_error("Cannot open config file");
  }

  Json::Value root;
  Json::CharReaderBuilder builder;
  std::string errs;
  if (!Json::parseFromStream(builder, in, &root, &errs)) {
    throw std::runtime_error("JSON parse error: " + errs);
  }
  return root;
}

int main(int argc, char *argv[]) {
  if (argc < 2 || argc == 3 || argc > 5) {
    std::cerr
        << "Usage:\n"
        << "  " << argv[0]
        << " <config.json>                     # creates default config file\n"
        << "  " << argv[0] << " <config.json> <out.png> <boxes.txt> [<seed>]\n";
    return EXIT_FAILURE;
  }

  std::filesystem::path cfg_path = argv[1];

  if (argc == 2) {
    std::filesystem::path default_cfg =
        std::filesystem::path(WORKING_DIRECTORY) / "generator_config.json";
    std::ifstream src(default_cfg, std::ios::binary);
    std::ofstream dst(cfg_path, std::ios::binary);
    if (!src.is_open() || !dst.is_open()) {
      std::cerr << "Error: cannot create config file\n";
      return EXIT_FAILURE;
    }
    dst << src.rdbuf();
    return EXIT_SUCCESS;
  }

  Json::Value config;
  try {
    config = parse_сonfig(cfg_path);
  } catch (const std::exception &ex) {
    std::cerr << "Failed to load config: " << ex.what() << "\n";
    return EXIT_FAILURE;
  }

  std::filesystem::path image_out = argv[2];
  std::filesystem::path markup_out = argv[3];

  // Either take default seed value (from config) or from arg
  int seed_value = (argc >= 5 ? std::stoi(argv[4]) : config["seed"].asInt());

  auto [out_img, boxes] = generate_ellipse_grid(config, seed_value);

  if (!cv::imwrite(image_out.string(), out_img)) {
    std::cerr << "Error: failed to write image to " << image_out << "\n";
    return EXIT_FAILURE;
  }

  std::ofstream mark_file(markup_out);
  if (!mark_file.is_open()) {
    std::cerr << "Error: cannot write markup to " << markup_out << "\n";
    return EXIT_FAILURE;
  }
  for (const auto &r : boxes) {
    mark_file << r.x << " " << r.y << " " << r.width << " " << r.height << "\n";
  }
  mark_file.close();

  return EXIT_SUCCESS;
}