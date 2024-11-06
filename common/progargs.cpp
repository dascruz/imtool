#include <common/progargs.hpp>
#include <imgaos/imageaos.hpp>
#include <iostream>

[[noreturn]] void ProgArgs::printErrorAndExit(std::string const & message, int const exitCode) {
  std::cerr << "Error: " << message << '\n';
  exit(exitCode);
}

ProgArgs::OperationType ProgArgs::parseInfo(OperationArgs const & operationArgs) {
  if (operationArgs.args.size() != ARG_COUNT_INFO) {
    printErrorAndExit("Invalid number of extra arguments for info: " +
                          std::to_string(operationArgs.args.size() - ARG_COUNT_INFO),
                      ERROR_INVALID_ARGS);
  }

  return Info;
}

ProgArgs::OperationType ProgArgs::parseMaxLevel(OperationArgs const & operationArgs) {
  if (operationArgs.args.size() != ARG_COUNT_MAXLEVEL) {
    printErrorAndExit("Invalid number of extra arguments for maxlevel: " +
                          std::to_string(operationArgs.args.size() - ARG_COUNT_MAXLEVEL + 1),
                      ERROR_INVALID_ARGS);
  }

  int maxLevel = 0;
  try {
    maxLevel = std::stoi(operationArgs.args[0]);
  } catch (std::invalid_argument const &) {
    printErrorAndExit("Invalid maxlevel: " + operationArgs.args[0], ERROR_INVALID_ARGS);
  } catch (std::out_of_range const &) {
    printErrorAndExit("Invalid maxlevel (out of range): " + operationArgs.args[0],
                      ERROR_INVALID_ARGS);
  }

  if (maxLevel < MAX_LEVEL_MIN || maxLevel > MAX_LEVEL_MAX) {
    printErrorAndExit("Invalid maxlevel: " + operationArgs.args[0], ERROR_INVALID_ARGS);
  }

  return MaxLevel;
}

ProgArgs::OperationType ProgArgs::parseResize(OperationArgs const & operationArgs) {
  if (operationArgs.args.size() != ARG_COUNT_RESIZE) {
    printErrorAndExit("Invalid number of extra arguments for resize: " +
                          std::to_string(operationArgs.args.size() - ARG_COUNT_RESIZE + 2),
                      ERROR_INVALID_ARGS);
  }

  int width  = 0;
  int height = 0;
  try {
    width  = std::stoi(operationArgs.args[0]);
    height = std::stoi(operationArgs.args[1]);
  } catch (std::invalid_argument const &) {
    printErrorAndExit("Invalid resize arguments: non-integer value provided", ERROR_INVALID_ARGS);
  } catch (std::out_of_range const &) {
    printErrorAndExit("Invalid resize arguments: value out of range", ERROR_INVALID_ARGS);
  }

  if (width <= 0) {
    printErrorAndExit("Invalid resize width: " + operationArgs.args[0], ERROR_INVALID_ARGS);
  }
  if (height <= 0) {
    printErrorAndExit("Invalid resize height: " + operationArgs.args[1], ERROR_INVALID_ARGS);
  }

  return Resize;
}

ProgArgs::OperationType ProgArgs::parseCutFreq(OperationArgs const & operationArgs) {
  if (operationArgs.args.size() != ARG_COUNT_CUTFREQ) {
    printErrorAndExit("Invalid number of extra arguments for cutfreq: " +
                          std::to_string(operationArgs.args.size() - ARG_COUNT_CUTFREQ + 1),
                      ERROR_INVALID_ARGS);
  }
  int const cutFreq = std::stoi(operationArgs.args[0]);
  if (cutFreq <= 0) {
    printErrorAndExit("Invalid cutfreq: " + operationArgs.args[0], ERROR_INVALID_ARGS);
  }

  return CutFreq;
}

ProgArgs::OperationType ProgArgs::parseCompress(OperationArgs const & operationArgs) {
  if (operationArgs.args.size() != ARG_COUNT_COMPRESS) {
    printErrorAndExit("Invalid number of extra arguments for compress: " +
                          std::to_string(operationArgs.args.size() - ARG_COUNT_COMPRESS),
                      ERROR_INVALID_ARGS);
  }

  return Compress;
}

namespace {
  ProgArgs::OperationType mapOperationToEnum(std::string const & operation) {
    if (operation == "info") { return ProgArgs::OperationType::Info; }
    if (operation == "maxlevel") { return ProgArgs::OperationType::MaxLevel; }
    if (operation == "resize") { return ProgArgs::OperationType::Resize; }
    if (operation == "cutfreq") { return ProgArgs::OperationType::CutFreq; }
    if (operation == "compress") { return ProgArgs::OperationType::Compress; }
    return ProgArgs::OperationType::Invalid;
  }
}  // namespace

ProgArgs::OperationType ProgArgs::parseOperation(std::vector<std::string> const & args) {
  if (args.size() < ARG_COUNT_MIN) {
    printErrorAndExit("Invalid number of arguments: " + std::to_string(args.size() - 1),
                      ERROR_INVALID_ARGS);
  }

  OperationArgs const operationArgs = {.inputFilePath  = args[INPUT_FILE_INDEX],
                                       .outputFilePath = args[OUTPUT_FILE_INDEX],
                                       .operation      = args[OPERATION_INDEX],
                                       .args =
                                           std::vector(args.begin() + ARG_COUNT_MIN, args.end())};

  switch (mapOperationToEnum(operationArgs.operation)) {
    case Info:
      return parseInfo(operationArgs);
    case MaxLevel:
      return parseMaxLevel(operationArgs);
    case Resize:
      return parseResize(operationArgs);
    case CutFreq:
      return parseCutFreq(operationArgs);
    case Compress:
      return parseCompress(operationArgs);
    default:
      printErrorAndExit("Invalid option: " + operationArgs.operation, ERROR_INVALID_ARGS);
  }
}