#include <json/json.hpp>

#include <stdio.h>

#include <string>
#include <fstream>

enum result_t : int {
  SUCCESS = 0,
  FAILURE = 1,
};

enum option_t {
  // TODO: Options?
};

void print_usage(const char *arg0) { printf("USAGE: %s <json file>\n", arg0); }

int main(int argc, char **argv) {
  if (2 != argc) {
    print_usage(argv[0]);
    return FAILURE;
  }
  for (int i = 1; i < argc; ++i) {
    switch (argv[i][0]) {
      // TODO: Options?
      default:
        break;
    }
  }

  std::ifstream file(argv[1]);
  std::string str((std::istreambuf_iterator<char>(file)),
                  std::istreambuf_iterator<char>());

  json::value value = json::read(str);
  std::string json = json::write(value, "  ");

  printf("%s\n", json.c_str());

  return SUCCESS;
}
