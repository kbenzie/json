#include <json/json.hpp>

#include <cmath>

const char *typeStr(json::type type) {
  switch (type) {
    case json::TYPE_OBJECT:
      return "Object";
    case json::TYPE_ARRAY:
      return "Array";
    case json::TYPE_NUMBER:
      return "Number";
    case json::TYPE_STRING:
      return "String";
    case json::TYPE_BOOL:
      return "Bool";
    case json::TYPE_NULL:
      return "Null";
    default:
      return nullptr;
  }
}

int main(int argc, char **argv) {
  json::value value;
  printf("%s\n", typeStr(value.type()));

  value = json::value(42);
  printf("%s\n", typeStr(value.type()));

  value = json::value("String");
  printf("%s\n", typeStr(value.type()));

  json::object object;
  value = json::value(object);
  printf("%s\n", typeStr(value.type()));

  json::array array;
  value = json::value(array);
  printf("%s\n", typeStr(value.type()));

  value = json::value(true);
  printf("%s\n", typeStr(value.type()));

  value = json::value();
  printf("%s\n", typeStr(value.type()));

  value = json::value{
    json::object{
      {"key", json::value(42)},
      {"string", json::value("value")},
    }
  };
  printf("%s\n", typeStr(value.type()));

  object = value.object();
  printf("%s\n", typeStr(object.get("key").type()));
  printf("Value: %f\n", object.get("key").number());
  printf("%s\n", typeStr(object.get("string").type()));
  printf("Value: %s\n", object.get("string").string().c_str());

  value = json::value{
    json::array{
      json::value(42),
      json::value("string"),
      json::value(false)
    }
  };

  array = value.array();
  for (auto &value : array) {
    printf("%s\n", typeStr(value.type()));
    switch (value.type()) {
      case json::TYPE_NUMBER:
        printf("Value: %f\n", value.number());
        break;
      case json::TYPE_STRING:
        printf("Value: %s\n", value.string().c_str());
        break;
      case json::TYPE_BOOL:
        printf("Value: %s\n", (value.boolean()) ? "true" : "false");
        break;
      default:
        break;
    }
  }

  value = json::value{
    json::object{
      {"key", json::value(42)},
      {"string", json::value("value")},
      {"array", json::value(json::array{
        json::value(4 * atan(1)),
        json::value("string"),
        json::value(true),
        json::value(),
      })}
    }
  };

  std::string json = json::write(value, "  ");
  printf("%s\n", json.c_str());

  value = json::read(json);

  printf("%s\n", typeStr(value.type()));
  object = value.object();

  for (auto &value : object) {
    printf("\"%s\": %s\n", value.first.c_str(), typeStr(value.second.type()));
  }

  array = object.get("array").array();
  for (auto &value : array) {
    switch (value.type()) {
      case json::TYPE_OBJECT:
      case json::TYPE_ARRAY:
        break;
      case json::TYPE_NUMBER:
        printf("%f\n", value.number());
        break;
      case json::TYPE_STRING:
        printf("\"%s\"\n", value.string().c_str());
        break;
      case json::TYPE_BOOL:
        printf("%s\n", value.boolean() ? "true" : "false");
        break;
      case json::TYPE_NULL:
        printf("null\n");
        break;
    }
  }

  json::array friends = json::array{
    json::value(json::object{
      {"id", json::value(0)},
      {"name", json::value("Ayala Benjamin")},
    }),
    json::value(json::object{
      {"id", json::value(1)},
      {"name", json::value("Cummings Lara")},
    }),
    json::value(json::object{
      {"id", json::value(2)},
      {"name", json::value("Terri Salas")},
    }),
  };

  printf("%s\n", json::write(json::value(friends), "  ").c_str());

  return 0;
}
