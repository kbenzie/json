#include <json/json.hpp>

#include <sstream>

struct indent_t {
  indent_t(const char *str) : str(str), count(0) {}

  void operator++() { count++; }
  void operator--() { count--; }

  const char *str;
  uint32_t count;
};

struct diagnostic_t {
  diagnostic_t() : line(0), column(0), index(0), error(nullptr) {}

  uint32_t line;
  uint32_t column;
  uint32_t index;
  const char *error;
};

void push(const indent_t &indent, std::stringstream &stream) {
  for (uint32_t i = 0; i < indent.count; ++i) {
    stream << indent.str;
  }
}

json::value json::read(const std::string &string) { return json::value(); }

void write_value(const json::value &value, indent_t &indent,
                 std::stringstream &stream);

void write_object(const json::object &object, indent_t &indent,
                  std::stringstream &stream) {
  stream << "{";
  ++indent;
  bool first = true;
  for (auto &pair : object) {
    if (!first) {
      stream << ",";
    }
    stream << "\n";
    push(indent, stream);
    stream << "\"" << pair.first << "\": ";
    write_value(pair.second, indent, stream);
    first = false;
  }
  --indent;
  stream << "\n";
  push(indent, stream);
  stream << "}";
}

void write_array(const json::array &array, indent_t &indent,
                 std::stringstream &stream) {
  stream << "[";
  ++indent;
  bool first = true;
  for (auto &value : array) {
    if (!first) {
      stream << ",";
    }
    stream << "\n";
    push(indent, stream);
    write_value(value, indent, stream);
    first = false;
  }
  --indent;
  stream << "\n";
  push(indent, stream);
  stream << "]";
}

void write_value(const json::value &value, indent_t &indent,
                 std::stringstream &stream) {
  switch (value.type()) {
    case json::TYPE_OBJECT:
      write_object(value.object(), indent, stream);
      break;
    case json::TYPE_ARRAY:
      write_array(value.array(), indent, stream);
      break;
    case json::TYPE_NUMBER:
      stream << value.number();
      break;
    case json::TYPE_STRING:
      stream << "\"" << value.string() << "\"";
      break;
    case json::TYPE_BOOL:
      stream << (value.boolean() ? "true" : "false");
      break;
    case json::TYPE_NULL:
      stream << "null";
      break;
  }
}

std::string json::write(const json::value &value, const char *tab) {
  indent_t indent(tab);
  std::stringstream stream;
  write_value(value, indent, stream);
  return stream.str();
}
