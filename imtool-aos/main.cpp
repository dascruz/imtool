#include <common/progargs.hpp>
#include <string>
#include <vector>

int main(int const argc, char * argv[]) {
  std::vector<std::string> const args(argv, argv + argc);

  switch (progargs::ParsedOperationArgs const parsedOperationArgs = progargs::parseOperation(args);
          parsedOperationArgs.operation) {
    case progargs::Info:
      break;
    case progargs::MaxLevel:
      break;
    case progargs::Resize:
      break;
    case progargs::CutFreq:
      break;
    case progargs::Compress:
      break;
    default:
      break;
  }

  return 0;
}
