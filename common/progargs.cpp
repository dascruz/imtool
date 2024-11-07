#include <common/progargs.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace progargs {
  [[noreturn]] void progargs::printErrorAndExit(std::string const & message) {
    std::cerr << "Error: " << message << "\n";
    exit(-1);
  }

  ParsedOperationArgs progargs::parseInfo(OperationArgs const & operationArgs) {
    if (operationArgs.args.size() != ARG_COUNT_INFO) {
      printErrorAndExit("Invalid number of extra arguments for info: " +
                        std::to_string(operationArgs.args.size()));
    }

    ParsedOperationArgs parsedArgs;
    parsedArgs.inputFilePath = operationArgs.inputFilePath;
    parsedArgs.operation     = Info;

    return parsedArgs;
  }

  ParsedOperationArgs progargs::parseMaxLevel(OperationArgs const & operationArgs) {
    if (operationArgs.args.size() != ARG_COUNT_MAXLEVEL) {
      printErrorAndExit("Invalid number of extra arguments for maxlevel: " +
                        std::to_string(operationArgs.args.size()));
    }

    int maxLevel = 0;
    try {
      maxLevel = std::stoi(operationArgs.args[0]);
    } catch (std::invalid_argument const &) {
      printErrorAndExit("Invalid maxlevel: " + operationArgs.args[0]);
    } catch (std::out_of_range const &) {
      printErrorAndExit("Invalid maxlevel (out of range): " + operationArgs.args[0]);
    }

    if (maxLevel < MAX_LEVEL_MIN || maxLevel > MAX_LEVEL_MAX) {
      printErrorAndExit("Invalid maxlevel: " + operationArgs.args[0]);
    }

    ParsedOperationArgs parsedArgs;
    parsedArgs.inputFilePath  = operationArgs.inputFilePath;
    parsedArgs.outputFilePath = operationArgs.outputFilePath;
    parsedArgs.operation      = MaxLevel;
    parsedArgs.args           = {static_cast<std::uint16_t>(maxLevel)};

    return parsedArgs;
  }

  ParsedOperationArgs progargs::parseResize(OperationArgs const & operationArgs) {
    if (operationArgs.args.size() != ARG_COUNT_RESIZE) {
      printErrorAndExit("Invalid number of extra arguments for resize: " +
                        std::to_string(operationArgs.args.size()));
    }

    int width  = 0;
    int height = 0;
    try {
      width  = std::stoi(operationArgs.args[0]);
      height = std::stoi(operationArgs.args[1]);
    } catch (std::invalid_argument const &) {
      printErrorAndExit("Invalid resize arguments: non-integer value provided");
    } catch (std::out_of_range const &) {
      printErrorAndExit("Invalid resize arguments: value out of range");
    }

    if (width <= 0) { printErrorAndExit("Invalid resize width: " + operationArgs.args[0]); }
    if (height <= 0) { printErrorAndExit("Invalid resize height: " + operationArgs.args[1]); }

    ParsedOperationArgs parsedArgs;
    parsedArgs.inputFilePath  = operationArgs.inputFilePath;
    parsedArgs.outputFilePath = operationArgs.outputFilePath;
    parsedArgs.operation      = Resize;
    parsedArgs.args = {static_cast<std::uint16_t>(width), static_cast<std::uint16_t>(height)};

    return parsedArgs;
  }

  ParsedOperationArgs progargs::parseCutFreq(OperationArgs const & operationArgs) {
    if (operationArgs.args.size() != ARG_COUNT_CUTFREQ) {
      printErrorAndExit("Invalid number of extra arguments for cutfreq: " +
                        std::to_string(operationArgs.args.size()));
    }
    int cutFreq = 0;
    try {
      cutFreq = std::stoi(operationArgs.args[0]);
    } catch (std::invalid_argument const &) {
      printErrorAndExit("Invalid cutfreq: non-integer value provided");
    } catch (std::out_of_range const &) {
      printErrorAndExit("Invalid cutfreq (out of range): " + operationArgs.args[0]);
    }

    if (cutFreq <= 0) { printErrorAndExit("Invalid cutfreq: " + operationArgs.args[0]); }

    ParsedOperationArgs parsedArgs;
    parsedArgs.inputFilePath  = operationArgs.inputFilePath;
    parsedArgs.outputFilePath = operationArgs.outputFilePath;
    parsedArgs.operation      = CutFreq;
    parsedArgs.args           = {static_cast<std::uint16_t>(cutFreq)};

    return parsedArgs;
  }

  ParsedOperationArgs progargs::parseCompress(OperationArgs const & operationArgs) {
    if (operationArgs.args.size() != ARG_COUNT_COMPRESS) {
      printErrorAndExit("Invalid number of extra arguments for compress: " +
                        std::to_string(operationArgs.args.size() - ARG_COUNT_COMPRESS));
    }

    ParsedOperationArgs parsedArgs;
    parsedArgs.inputFilePath  = operationArgs.inputFilePath;
    parsedArgs.outputFilePath = operationArgs.outputFilePath;
    parsedArgs.operation      = Compress;

    return parsedArgs;
  }

  namespace {
    OperationType mapOperationToEnum(std::string const & operation) {
      if (operation == "info") { return Info; }
      if (operation == "maxlevel") { return MaxLevel; }
      if (operation == "resize") { return Resize; }
      if (operation == "cutfreq") { return CutFreq; }
      if (operation == "compress") { return Compress; }
      return Invalid;
    }
  }  // namespace

  ParsedOperationArgs progargs::parseOperation(std::vector<std::string> const & args) {
    if (args.size() < ARG_COUNT_MIN) {
      printErrorAndExit("Invalid number of arguments: " + std::to_string(args.size() - 1));
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
        printErrorAndExit("Invalid option: " + operationArgs.operation);
    }
  }
}  // namespace progargs