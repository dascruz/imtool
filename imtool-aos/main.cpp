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

  ProgArgs::OperationArgs const operationArgs = {
    .operation      = operation,
    .inputFilePath  = inputFilePath,
    .outputFilePath = outputFilePath,
    .args           = std::vector(args.begin() + ProgArgs::ARG_COUNT_INFO, args.end()),
    .argc           = argc,
    .mode           = ProgArgs::MODE_AOS};

  ProgArgs::handleOperation(operationArgs);

  return 0;
}
