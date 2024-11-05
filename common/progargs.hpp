#pragma once

#include <string>
#include <vector>

class ProgArgs {
  public:
    static constexpr int ERROR_INVALID_ARGS = -1;
    static constexpr int MAX_LEVEL_MIN      = 0;
    static constexpr int MAX_LEVEL_MAX      = 65535;
    static constexpr int ARG_COUNT_INFO     = 4;
    static constexpr int ARG_COUNT_MAXLEVEL = 5;
    static constexpr int ARG_COUNT_RESIZE   = 6;
    static constexpr int ARG_COUNT_CUTFREQ  = 5;
    static constexpr int ARG_COUNT_COMPRESS = 4;

    static constexpr int INPUT_FILE_INDEX  = 1;
    static constexpr int OUTPUT_FILE_INDEX = 2;
    static constexpr int OPERATION_INDEX   = 3;

    static constexpr int MODE_SOA = 0;
    static constexpr int MODE_AOS = 1;

    struct OperationArgs {
        std::string operation;
        std::string inputFilePath;
        std::string outputFilePath;
        std::vector<std::string> args;
        int argc;
        int mode;
    };

    static void printErrorAndExit(std::string const & message, int exitCode);
    static void handleInfo(OperationArgs const & operationArgs);
    static void handleMaxLevel(OperationArgs const & operationArgs);
    static void handleResize(OperationArgs const & operationArgs);
    static void handleCutFreq(OperationArgs const & operationArgs);
    static void handleCompress(OperationArgs const & operationArgs);
    static void handleOperation(OperationArgs const & operationArgs);
};
