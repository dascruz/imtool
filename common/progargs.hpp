#pragma once

#include <string>

class ProgArgs {
  public:
  static constexpr int ERROR_INVALID_ARGS = -1;
  static constexpr int MAX_LEVEL_MIN      = 0;
  static constexpr int MAX_LEVEL_MAX      = 65535;

  // Expected argument counts for each command
  static constexpr int ARG_COUNT_INFO     = 4;
  static constexpr int ARG_COUNT_MAXLEVEL = 5;
  static constexpr int ARG_COUNT_RESIZE   = 6;
  static constexpr int ARG_COUNT_CUTFREQ  = 5;
  static constexpr int ARG_COUNT_COMPRESS = 4;

  // Argument index positions
  static constexpr int INPUT_FILE_INDEX  = 1;
  static constexpr int OUTPUT_FILE_INDEX = 2;
  static constexpr int OPERATION_INDEX   = 3;
  static constexpr int EXTRA_ARG1_INDEX  = 4;
  static constexpr int EXTRA_ARG2_INDEX  = 5;
  static constexpr int EXTRA_ARG3_INDEX  = 6;

  struct MaxLevelArgs {
    std::string inputFilePath;
    std::string maxLevelStr;
    int argc;
  };

  struct ResizeArgs {
    std::string inputFilePath;
    std::string widthStr;
    std::string heightStr;
    int argc;
  };

  struct CutFreqArgs {
    std::string inputFilePath;
    std::string cutFreqStr;
    int argc;
  };

  struct CompressArgs {
    std::string inputFilePath;
    std::string outputFilePath;
    int argc;
  };

  static void printErrorAndExit(std::string const & message, int exitCode);
  static void handleInfo(std::string const & inputFilePath, int argc);
  static void handleMaxLevel(MaxLevelArgs const & args);
  static void handleResize(ResizeArgs const & args);
  static void handleCutFreq(CutFreqArgs const & args);
  static void handleCompress(CompressArgs const & args);
};
