#include <common/progargs.hpp>
#include <string>
#include <vector>

int main(int const argc, char * argv[]) {
  if (argc < ProgArgs::ARG_COUNT_INFO) {
    ProgArgs::printErrorAndExit("Invalid number of arguments: " + std::to_string(argc - 1),
                                ProgArgs::ERROR_INVALID_ARGS);
  }

  std::vector<std::string> args(argv, argv + argc);
  std::string const inputFilePath  = args[ProgArgs::INPUT_FILE_INDEX];
  std::string const outputFilePath = args[ProgArgs::OUTPUT_FILE_INDEX];
  std::string const operation      = args[ProgArgs::OPERATION_INDEX];

  if (operation == "info") {
    ProgArgs::handleInfo(inputFilePath, argc);
  } else if (operation == "maxlevel") {
    ProgArgs::handleMaxLevel({.inputFilePath = inputFilePath,
                              .maxLevelStr   = args[ProgArgs::EXTRA_ARG1_INDEX],
                              .argc          = argc});
  } else if (operation == "resize") {
    ProgArgs::handleResize({.inputFilePath = inputFilePath,
                            .widthStr      = args[ProgArgs::EXTRA_ARG1_INDEX],
                            .heightStr     = args[ProgArgs::EXTRA_ARG2_INDEX],
                            .argc          = argc});
  } else if (operation == "cutfreq") {
    ProgArgs::handleCutFreq({.inputFilePath = inputFilePath,
                             .cutFreqStr    = args[ProgArgs::EXTRA_ARG1_INDEX],
                             .argc          = argc});
  } else if (operation == "compress") {
    ProgArgs::handleCompress(
        {.inputFilePath = inputFilePath, .outputFilePath = outputFilePath, .argc = argc});
  } else {
    ProgArgs::printErrorAndExit("Invalid option: " + operation, ProgArgs::ERROR_INVALID_ARGS);
  }

  return 0;
}
