#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace progargs {
  enum OperationType : std::uint8_t { Info, MaxLevel, Resize, CutFreq, Compress, Invalid };

  struct OperationArgs {
      std::string inputFilePath;
      std::string outputFilePath;
      std::string operation;
      std::vector<std::string> args;
  };

  struct ParsedOperationArgs {
      std::string inputFilePath;
      std::string outputFilePath;
      OperationType operation;
      std::vector<std::uint16_t> args;

      explicit ParsedOperationArgs(std::string inputPath = "", std::string outputPath = "",
                                   OperationType operationType          = Invalid,
                                   std::vector<std::uint16_t> arguments = {})
        : inputFilePath(std::move(inputPath)), outputFilePath(std::move(outputPath)),
          operation(operationType), args(std::move(arguments)) { }
  };

  [[nodiscard]] ParsedOperationArgs parseOperation(std::vector<std::string> const & args);

  inline constexpr int INPUT_FILE_INDEX  = 1;
  inline constexpr int OUTPUT_FILE_INDEX = 2;
  inline constexpr int OPERATION_INDEX   = 3;

  inline constexpr int ARG_COUNT_MIN      = 4;
  inline constexpr int ARG_COUNT_INFO     = 0;
  inline constexpr int ARG_COUNT_MAXLEVEL = 1;
  inline constexpr int ARG_COUNT_RESIZE   = 2;
  inline constexpr int ARG_COUNT_CUTFREQ  = 1;
  inline constexpr int ARG_COUNT_COMPRESS = 0;

  inline constexpr int MAX_LEVEL_MIN = 1;
  inline constexpr int MAX_LEVEL_MAX = 65535;
}  // namespace progargs
