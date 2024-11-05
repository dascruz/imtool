#include "progargs.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

void ProgArgs::printErrorAndExit(std::string const & message, int exitCode) {
  std::cerr << "Error: " << message << '\n';
  exit(exitCode);
}

void ProgArgs::handleInfo(std::string const & inputFilePath, int const argc) {
  if (argc != ARG_COUNT_INFO) {
    printErrorAndExit("Invalid extra arguments for info: " + std::to_string(argc - ARG_COUNT_INFO),
                      ERROR_INVALID_ARGS);
  }
  std::cout << "Displaying metadata for: " << inputFilePath << '\n';
}

void ProgArgs::handleMaxLevel(MaxLevelArgs const & args) {
  if (args.argc != ARG_COUNT_MAXLEVEL) {
    printErrorAndExit("Invalid number of extra arguments for maxlevel: " +
                          std::to_string(args.argc - ARG_COUNT_MAXLEVEL),
                      ERROR_INVALID_ARGS);
  }

  int const maxLevel = std::stoi(args.maxLevelStr);
  if (maxLevel < MAX_LEVEL_MIN || maxLevel > MAX_LEVEL_MAX) {
    printErrorAndExit("Invalid maxlevel: " + args.maxLevelStr, ERROR_INVALID_ARGS);
  }

  std::cout << "Applying max level: " << maxLevel << " to " << args.inputFilePath << '\n';
}

void ProgArgs::handleResize(ResizeArgs const & args) {
  if (args.argc != ARG_COUNT_RESIZE) {
    printErrorAndExit("Invalid number of extra arguments for resize: " +
                          std::to_string(args.argc - ARG_COUNT_RESIZE),
                      ERROR_INVALID_ARGS);
  }

  int const width  = std::stoi(args.widthStr);
  int const height = std::stoi(args.heightStr);
  if (width <= 0) {
    printErrorAndExit("Invalid resize width: " + args.widthStr, ERROR_INVALID_ARGS);
  }
  if (height <= 0) {
    printErrorAndExit("Invalid resize height: " + args.heightStr, ERROR_INVALID_ARGS);
  }

  std::cout << "Resizing " << args.inputFilePath << " to " << width << "x" << height << '\n';
}

void ProgArgs::handleCutFreq(CutFreqArgs const & args) {
  if (args.argc != ARG_COUNT_CUTFREQ) {
    printErrorAndExit("Invalid number of extra arguments for cutfreq: " +
                          std::to_string(args.argc - ARG_COUNT_CUTFREQ),
                      ERROR_INVALID_ARGS);
  }

  int const cutFreq = std::stoi(args.cutFreqStr);
  if (cutFreq <= 0) {
    printErrorAndExit("Invalid cutfreq: " + args.cutFreqStr, ERROR_INVALID_ARGS);
  }

  std::cout << "Cutting frequency in " << args.inputFilePath << " with threshold " << cutFreq
            << '\n';
}

void ProgArgs::handleCompress(CompressArgs const & args) {
  if (args.argc != ARG_COUNT_COMPRESS) {
    printErrorAndExit("Invalid extra arguments for compress: " +
                          std::to_string(args.argc - ARG_COUNT_COMPRESS + 1),
                      ERROR_INVALID_ARGS);
  }

  std::cout << "Compressing " << args.inputFilePath << " to " << args.outputFilePath << '\n';
}
