#include <iomanip>
#include <iostream>
#include <vector>

#include <semcv/semcv.hpp>

const std::vector<std::array<int, 3>> SAMPLE_LEVELS = {
    {0, 127, 255}, {20, 127, 235}, {55, 127, 200}, {90, 127, 165}};
const std::vector<int> NOISE_STDS = {3, 7, 15};

const int noised_cnt = NOISE_STDS.size();
const int samples_cnt = SAMPLE_LEVELS.size();

void print_table(const std::vector<std::vector<std::vector<float>>> &means,
                 const std::vector<std::vector<std::vector<float>>> &stds,
                 int figure) {
  
  std::cout << "| Noise STD ";
  for (int i = 0; i < samples_cnt; ++i) {
    std::cout << "| Brightness = " << SAMPLE_LEVELS[i][figure] << " ";
  }
  std::cout << "|\n";

  for (int i = 0; i < samples_cnt + 1; ++i) {
    std::cout << "|----------------";
  }
  std::cout << "|\n";

  for (int row = 0; row < noised_cnt; ++row) {
    std::cout << "| " << NOISE_STDS[row] << " |";
    for (int col = 0; col < samples_cnt; ++col) {
      auto mean = means[figure][col][row];
      auto std = stds[figure][col][row];

      std::cout << " " << std::fixed << std::setprecision(4) << mean << "; "
                << std << " |";
    }
    std::cout << "\n";
  }
}

int main() {

  std::vector<std::vector<std::vector<float>>> stds(
      3, std::vector<std::vector<float>>(samples_cnt,
                                         std::vector<float>(noised_cnt)));
  std::vector<std::vector<std::vector<float>>> means(
      3, std::vector<std::vector<float>>(samples_cnt,
                                         std::vector<float>(noised_cnt)));

  for (int i = 0; i < samples_cnt; ++i) {
    auto lvl = SAMPLE_LEVELS[i];
    cv::Mat img = gen_tgtimg00(lvl[0], lvl[1], lvl[2]);
    for (int j = 0; j < noised_cnt; ++j) {
      int std = NOISE_STDS[j];
      auto noised = add_noise_gau(img, std);

      // bg - square - circle
      cv::Mat masks[3] = {gen_tgtimg00(1, 0, 0), gen_tgtimg00(0, 1, 0),
                          gen_tgtimg00(0, 0, 1)};
      for (int k = 0; k < 3; ++k) {
        auto params = calc_distribution_params(noised, masks[k]);
        means[k][i][j] = params.first;
        stds[k][i][j] = params.second;
      }
    }
  }

  std::cout << "(Mean, Variance) for Background:\n";
  print_table(means, stds, 0);
  std::cout << "\n(Mean, Variance) for Square:\n";
  print_table(means, stds, 1);
  std::cout << "\n(Mean, Variance) for Circle:\n";
  print_table(means, stds, 2);
}