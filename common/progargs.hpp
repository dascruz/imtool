#ifndef PROGARGS_HPP
#define PROGARGS_HPP

#include <string>

class ProgArgs {
public:
  static void printErrorAndExit(const std::string &message, int exitCode);
  static void handleInfo(const std::string &inputFilePath, int argc);
  static void handleMaxLevel(const std::string &inputFilePath, const std::string &maxLevelStr, int argc);
  static void handleResize(const std::string &inputFilePath, const std::string &widthStr, const std::string &heightStr, int argc);
  static void handleCutFreq(const std::string &inputFilePath, const std::string &cutFreqStr, int argc);
  static void handleCompress(const std::string &inputFilePath, const std::string &outputFilePath, int argc);
};

#endif // PROGARGS_HPP
