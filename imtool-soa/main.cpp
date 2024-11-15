#include <common/progargs.hpp>
#include <imgsoa/imagesoa.hpp>
#include <string>
#include <vector>

int main(int const argc, char * argv[]) {
  std::vector<std::string> const args(argv, argv + argc);

  progargs::ParsedOperationArgs const parsedOperationArgs = progargs::parseOperation(args);
  imagesoa::Image image;

  image.loadFromFile(parsedOperationArgs.inputFilePath);

  switch (parsedOperationArgs.operation) {
    case progargs::Info:
      image.displayMetadata();
      break;
    case progargs::MaxLevel:
      image.modifyMaxLevel(static_cast<unsigned short>(parsedOperationArgs.args[0]));
      if (!image.saveToFile(parsedOperationArgs.outputFilePath)) { return -1; }
      break;
    case progargs::Resize:
      image.resize(parsedOperationArgs.args[0], parsedOperationArgs.args[1]);
      if (!image.saveToFile(parsedOperationArgs.outputFilePath)) { return -1; }
      break;
    case progargs::CutFreq:
      image.cutfreq(parsedOperationArgs.args[0]);
      if (!image.saveToFile(parsedOperationArgs.outputFilePath)) { return -1; }
      break;
    case progargs::Compress:
      if (!image.saveToFileCompress(parsedOperationArgs.outputFilePath)) { return -1; }
      break;
    default:
      break;
  }

  return 0;
}
