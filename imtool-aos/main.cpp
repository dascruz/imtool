#include <common/progargs.hpp>
#include <string>
#include <vector>

int main(int const argc, char * argv[]) {
  std::vector<std::string> const args(argv, argv + argc);

  ProgArgs::OperationType const operationType = ProgArgs::parseOperation(args);

  return 0;
}
