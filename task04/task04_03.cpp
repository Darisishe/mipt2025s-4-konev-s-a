#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <semcv/semcv.hpp>

std::vector<cv::Rect> loadRects(const std::string &path, std::ofstream &log) {
  std::vector<cv::Rect> rects;
  std::ifstream in(path);
  if (!in.is_open()) {
    log << "Error: unable to open \"" << path << "\"\n";
    return rects;
  }
  int x, y, w, h;
  while (in >> x >> y >> w >> h) {
    rects.emplace_back(x, y, w, h);
  }
  if (rects.empty()) {
    log << "Warning: no rectangles found in \"" << path << "\"\n";
  }
  return rects;
}

float IoU(const cv::Rect &A, const cv::Rect &B) {
  int x1 = std::max(A.x, B.x);
  int y1 = std::max(A.y, B.y);
  int x2 = std::min(A.x + A.width, B.x + B.width);
  int y2 = std::min(A.y + A.height, B.y + B.height);
  if (x2 <= x1 || y2 <= y1) {
    return 0.0f;
  }
  float interArea = static_cast<float>((x2 - x1) * (y2 - y1));
  float areaA = static_cast<float>(A.width * A.height);
  float areaB = static_cast<float>(B.width * B.height);
  float unionArea = areaA + areaB - interArea;
  return interArea / unionArea;
}

std::array<float, 3> computeStats(const std::vector<cv::Rect> &groundTruth,
                                  const std::vector<cv::Rect> &predictions) {
  const float threshold = 0.5f;
  int tp = 0;
  float sumIoU = 0.0f;

  for (const auto &pred : predictions) {
    float best = 0.0f;
    for (const auto &gt : groundTruth) {
      best = std::max(best, IoU(pred, gt));
    }
    sumIoU += best;
    if (best >= threshold) {
      ++tp;
    }
  }

  float meanIoU = predictions.empty() ? 0.0f : (sumIoU / predictions.size());
  float precision = predictions.empty()
                        ? 0.0f
                        : (static_cast<float>(tp) / predictions.size());
  float recall = groundTruth.empty()
                     ? 0.0f
                     : (static_cast<float>(tp) / groundTruth.size());

  return {meanIoU, precision, recall};
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0]
              << " <truths_list.lst> <predictions_list.lst> <report_path>\n";
    return EXIT_FAILURE;
  }

  std::string refListFile = argv[1];
  std::string detListFile = argv[2];
  std::string reportFile = argv[3];

  std::ofstream report(reportFile);
  if (!report.is_open()) {
    std::cerr << "Error: cannot write to \"" << reportFile << "\"\n";
    return EXIT_FAILURE;
  }

  std::ifstream refListStream(refListFile);
  std::ifstream detListStream(detListFile);
  if (!refListStream.is_open() || !detListStream.is_open()) {
    std::cerr << "Error: cannot open list files\n";
    return EXIT_FAILURE;
  }

  std::string refPath, detPath;
  std::vector<std::array<float, 3>> statsList;

  while (std::getline(refListStream, refPath) &&
         std::getline(detListStream, detPath)) {
    report << "Evaluating:\n  Truths: " << refPath << "\n  Predictions: " << detPath
           << "\n";

    auto groundTruth = loadRects(refPath, report);
    auto predictions = loadRects(detPath, report);

    if (groundTruth.empty() || predictions.empty()) {
      report << "Skipped (empty set) for these files.\n";
    } else {
      auto metrics = computeStats(groundTruth, predictions);
      statsList.push_back(metrics);
      report << "  Detections: " << predictions.size()
             << ", GroundTruths: " << groundTruth.size() << "\n";
      report << "  Mean IoU:   " << metrics[0] << "\n";
      report << "  Precision:  " << metrics[1] << "\n";
      report << "  Recall:     " << metrics[2] << "\n";
    }
    report << "----------------------------------------\n";
  }

  float avgIoU = 0.0f, avgP = 0.0f, avgR = 0.0f;
  for (const auto &m : statsList) {
    avgIoU += m[0];
    avgP += m[1];
    avgR += m[2];
  }
  if (!statsList.empty()) {
    avgIoU /= statsList.size();
    avgP /= statsList.size();
    avgR /= statsList.size();
  }
  report << "Overall Averages:\n";
  report << "  Mean IoU:  " << avgIoU << "\n";
  report << "  Precision: " << avgP << "\n";
  report << "  Recall:    " << avgR << "\n";

  return EXIT_SUCCESS;
}
