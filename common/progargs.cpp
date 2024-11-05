#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

class ProgArgs {
  public:
    static void printErrorAndExit(std::string const & message, int exitCode) {
      std::cerr << "Error: " << message << '\n';
      exit(exitCode);
    }

    static void handleInfo(std::string const & inputFilePath, int argc) {
      if (argc != 4) {
        printErrorAndExit("Invalid extra arguments for info: " + std::to_string(argc - 4), -1);
      }
      // Handle 'info' operation here.
      std::cout << "Displaying metadata for: " << inputFilePath << '\n';
    }

    static void handleMaxLevel(std::string const & inputFilePath, std::string const & maxLevelStr,
                               int argc) {
      if (argc != 5) {
        printErrorAndExit(
            "Invalid number of extra arguments for maxlevel: " + std::to_string(argc - 4), -1);
      }

      int maxLevel = std::stoi(maxLevelStr);
      if (maxLevel < 0 || maxLevel > 65535) {
        printErrorAndExit("Invalid maxlevel: " + maxLevelStr, -1);
      }

      // Handle 'maxlevel' operation here.
      std::cout << "Applying max level: " << maxLevel << " to " << inputFilePath << '\n';
    }

    static void handleResize(std::string const & inputFilePath, std::string const & widthStr,
                             std::string const & heightStr, int argc) {
      if (argc != 6) {
        printErrorAndExit(
            "Invalid number of extra arguments for resize: " + std::to_string(argc - 4), -1);
      }

      int width  = std::stoi(widthStr);
      int height = std::stoi(heightStr);
      if (width <= 0) { printErrorAndExit("Invalid resize width: " + widthStr, -1); }
      if (height <= 0) { printErrorAndExit("Invalid resize height: " + heightStr, -1); }

      // Handle 'resize' operation here.
      std::cout << "Resizing " << inputFilePath << " to " << width << "x" << height << '\n';
    }

    static void handleCutFreq(std::string const & inputFilePath, std::string const & cutFreqStr,
                              int argc) {
      if (argc != 5) {
        printErrorAndExit(
            "Invalid number of extra arguments for cutfreq: " + std::to_string(argc - 4), -1);
      }

      int cutFreq = std::stoi(cutFreqStr);
      if (cutFreq <= 0) { printErrorAndExit("Invalid cutfreq: " + cutFreqStr, -1); }

      // Handle 'cutfreq' operation here.
      std::cout << "Cutting frequency in " << inputFilePath << " with threshold " << cutFreq
                << '\n';
    }

    static void handleCompress(std::string const & inputFilePath,
                               std::string const & outputFilePath, int argc) {
      if (argc != 4) {
        printErrorAndExit("Invalid extra arguments for compress: " + std::to_string(argc - 3), -1);
      }

      // Handle 'compress' operation here.
      std::cout << "Compressing " << inputFilePath << " to " << outputFilePath << '\n';
    }
};

int main(int argc, char * argv[]) {
  if (argc < 4) {
    ProgArgs::printErrorAndExit("Invalid number of arguments: " + std::to_string(argc - 1), -1);
  }

  std::vector<std::string> args(argv, argv + argc);
  std::string const & inputFilePath  = args[1];
  std::string const & outputFilePath = args[2];
  std::string const & operation      = args[3];

  if (operation == "info") {
    ProgArgs::handleInfo(inputFilePath, argc);
  } else if (operation == "maxlevel") {
    ProgArgs::handleMaxLevel(inputFilePath, args[4], argc);
  } else if (operation == "resize") {
    ProgArgs::handleResize(inputFilePath, args[4], args[5], argc);
  } else if (operation == "cutfreq") {
    ProgArgs::handleCutFreq(inputFilePath, args[4], argc);
  } else if (operation == "compress") {
    ProgArgs::handleCompress(inputFilePath, outputFilePath, argc);
  } else {
    ProgArgs::printErrorAndExit("Invalid option: " + operation, -1);
  }

  return 0;
}
