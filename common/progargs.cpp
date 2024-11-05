#include <common/progargs.hpp>
#include <iostream>

void ProgArgs::printErrorAndExit(std::string const & message, int const exitCode) {
  std::cerr << "Error: " << message << '\n';
  exit(exitCode);
}

void ProgArgs::handleInfo(OperationArgs const & operationArgs) {
  if (operationArgs.argc != ARG_COUNT_INFO) {
    printErrorAndExit("Invalid number of extra arguments for info: " +
                          std::to_string(operationArgs.argc - ARG_COUNT_INFO),
                      ERROR_INVALID_ARGS);
  }
  std::cout << "Displaying metadata for: " << operationArgs.inputFilePath << '\n';
  // if (operationArgs.mode == ProgArgs::MODE_SOA) {
  //   // Handle structure of arrays mode
  // } else if (operationArgs.mode == ProgArgs::MODE_AOS) {
  //   // Handle array of structures mode
  // }
}

void ProgArgs::handleMaxLevel(OperationArgs const & operationArgs) {
  if (operationArgs.argc != ARG_COUNT_MAXLEVEL) {
    printErrorAndExit("Invalid number of extra arguments for maxlevel: " +
                          std::to_string(operationArgs.argc - ARG_COUNT_MAXLEVEL + 1),
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

  std::cout << "Applying max level: " << maxLevel << " to " << operationArgs.inputFilePath
            << " and saving to " << operationArgs.outputFilePath << '\n';
  // if (operationArgs.mode == ProgArgs::MODE_SOA) {
  //   // Handle structure of arrays mode
  // } else if (operationArgs.mode == ProgArgs::MODE_AOS) {
  //   // Handle array of structures mode
  // }
}

void ProgArgs::handleResize(OperationArgs const & operationArgs) {
  if (operationArgs.argc != ARG_COUNT_RESIZE) {
    printErrorAndExit("Invalid number of extra arguments for resize: " +
                          std::to_string(operationArgs.argc - ARG_COUNT_RESIZE + 2),
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

  std::cout << "Resizing " << operationArgs.inputFilePath << " to " << width << "x" << height
            << " and saving to " << operationArgs.outputFilePath << '\n';
  // if (operationArgs.mode == ProgArgs::MODE_SOA) {
  //   // Handle structure of arrays mode
  // } else if (operationArgs.mode == ProgArgs::MODE_AOS) {
  //   // Handle array of structures mode
  // }
}

void ProgArgs::handleCutFreq(OperationArgs const & operationArgs) {
  if (operationArgs.argc != ARG_COUNT_CUTFREQ) {
    printErrorAndExit("Invalid number of extra arguments for cutfreq: " +
                          std::to_string(operationArgs.argc - ARG_COUNT_CUTFREQ + 1),
                      ERROR_INVALID_ARGS);
  }
  int const cutFreq = std::stoi(operationArgs.args[0]);
  if (cutFreq <= 0) {
    printErrorAndExit("Invalid cutfreq: " + operationArgs.args[0], ERROR_INVALID_ARGS);
  }
  std::cout << "Cutting frequency in " << operationArgs.inputFilePath << " with threshold "
            << cutFreq << " and saving to " << operationArgs.outputFilePath << '\n';
  // if (operationArgs.mode == ProgArgs::MODE_SOA) {
  //   // Handle structure of arrays mode
  // } else if (operationArgs.mode == ProgArgs::MODE_AOS) {
  //   // Handle array of structures mode
  // }
}

void ProgArgs::handleCompress(OperationArgs const & operationArgs) {
  if (operationArgs.argc != ARG_COUNT_COMPRESS) {
    printErrorAndExit("Invalid number of extra arguments for compress: " +
                          std::to_string(operationArgs.argc - ARG_COUNT_COMPRESS),
                      ERROR_INVALID_ARGS);
  }
  std::cout << "Compressing " << operationArgs.inputFilePath << " and saving to "
            << operationArgs.outputFilePath << '\n';
  // if (operationArgs.mode == ProgArgs::MODE_SOA) {
  //   // Handle structure of arrays mode
  // } else if (operationArgs.mode == ProgArgs::MODE_AOS) {
  //   // Handle array of structures mode
  // }
}

void ProgArgs::handleOperation(OperationArgs const & operationArgs) {
  if (operationArgs.operation == "info") {
    handleInfo(operationArgs);
  } else if (operationArgs.operation == "maxlevel") {
    handleMaxLevel(operationArgs);
  } else if (operationArgs.operation == "resize") {
    handleResize(operationArgs);
  } else if (operationArgs.operation == "cutfreq") {
    handleCutFreq(operationArgs);
  } else if (operationArgs.operation == "compress") {
    handleCompress(operationArgs);
  } else {
    printErrorAndExit("Invalid option: " + operationArgs.operation, ERROR_INVALID_ARGS);
  }
}
