#include <common/progargs.hpp>
#include <imgaos/imageaos.hpp>
#include <string>
#include <vector>

int main(int const argc, char * argv[]) {
  std::vector<std::string> const args(argv, argv + argc);

  progargs::ParsedOperationArgs const parsedOperationArgs = progargs::parseOperation(args);
  imageaos::Image image;
  image.loadFromFile(parsedOperationArgs.inputFilePath);

  switch (parsedOperationArgs.operation) {
    case progargs::Info:
      image.displayMetadata();
      break;
    case progargs::MaxLevel:
      image.modifyMaxLevel(parsedOperationArgs.args[0]);
      if (!image.saveToFile(parsedOperationArgs.outputFilePath)) { return EXIT_FAILURE; }
    default:
      break;
  }

  return 0;
}
