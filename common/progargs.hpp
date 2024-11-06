#pragma once

#include <cstdint>
#include <string>
#include <vector>

class ProgArgs {
  public:
    enum OperationType : std::uint8_t { Info, MaxLevel, Resize, CutFreq, Compress, Invalid };

    static constexpr int INPUT_FILE_INDEX  = 1;
    static constexpr int OUTPUT_FILE_INDEX = 2;
    static constexpr int OPERATION_INDEX   = 3;

    struct OperationArgs {
        std::string operation;
        std::string inputFilePath;
        std::string outputFilePath;
        std::vector<std::string> args;
    };

    [[nodiscard]] static OperationType parseOperation(std::vector<std::string> const & args);

  private:
    static constexpr int ERROR_INVALID_ARGS = -1;
    static constexpr int ARG_COUNT_MIN      = 4;
    static constexpr int ARG_COUNT_INFO     = 0;
    static constexpr int ARG_COUNT_MAXLEVEL = 1;
    static constexpr int ARG_COUNT_RESIZE   = 2;
    static constexpr int ARG_COUNT_CUTFREQ  = 1;
    static constexpr int ARG_COUNT_COMPRESS = 0;
    static constexpr int MAX_LEVEL_MIN      = 0;
    static constexpr int MAX_LEVEL_MAX      = 65535;

    static OperationType parseInfo(OperationArgs const & operationArgs);
    static OperationType parseMaxLevel(OperationArgs const & operationArgs);
    static OperationType parseResize(OperationArgs const & operationArgs);
    static OperationType parseCutFreq(OperationArgs const & operationArgs);
    static OperationType parseCompress(OperationArgs const & operationArgs);

    [[noreturn]] static void ProgArgs::printErrorAndExit(std::string const & message, int exitCode);
};
