#ifndef _JSON_HPP_
#define _JSON_HPP_

#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace json {
// Enumerations
enum type {
  TYPE_OBJECT,
  TYPE_ARRAY,
  TYPE_NUMBER,
  TYPE_STRING,
  TYPE_BOOL,
  TYPE_NULL,
};

// Forward declarations
class value;
class object;
class array;
typedef std::pair<const std::string, json::value> pair;

// API
json::value read(const std::string &string);
std::string write(const json::value &value, const char *indent = "  ");

// Helpers
#define ENABLE_IF_NUMBER(Type) \
  typename std::enable_if<std::is_arithmetic<Type>::value, Type>::type *

// Objects
class value {
 public:
  // Constructors
  value();
  explicit value(json::object object);
  explicit value(json::pair pair);
  explicit value(json::array array);
  template <typename Number>
  value(Number number, ENABLE_IF_NUMBER(Number) = 0);
  explicit value(const char *string);
  explicit value(std::string string);
  explicit value(bool boolean);

  // Accessors
  json::type &type();
  const json::type &type() const;
  json::object &object();
  const json::object &object() const;
  json::array &array();
  const json::array &array() const;
  double &number();
  const double &number() const;
  std::string &string();
  const std::string &string() const;
  bool &boolean();
  const bool &boolean() const;

 private:
  struct store_base {
    virtual ~store_base() {}
  };

  template <typename Type>
  struct store : public store_base {
    store(Type value) : value(value) {}
    Type value;
  };

  json::type mType;
  std::shared_ptr<store_base> mStore;
};

class object {
 public:
  // Types
  typedef std::map<std::string, json::value>::iterator iterator;
  typedef std::map<std::string, json::value>::const_iterator const_iterator;

  // Constructors
  object();
  object(std::string key, json::value value);
  object(json::pair pair);
  template <typename Type>
  object(std::string key, Type value);
  object(std::initializer_list<json::pair> values);

  // Accessors
  void add(std::string key, json::value value);
  void add(json::pair pair);

  json::value &get(std::string key);
  const json::value &get(std::string key) const;

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

 private:
  std::map<std::string, json::value> mEntries;
};

class array {
 public:
  // Types
  typedef std::vector<json::value>::iterator iterator;
  typedef std::vector<json::value>::const_iterator const_iterator;

  // Constructors
  array(std::initializer_list<json::value> values);
  template <typename... Args>
  array(Args... args);

  // Accessors
  void append(json::value value);
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;
  json::value &operator[](const size_t index);
  const json::value &operator[](const size_t index) const;
  json::value &at(const size_t index);
  const json::value &at(const size_t index) const;

  size_t size();
  size_t size() const;

 private:
  std::vector<json::value> mEntries;
};

// Implementations
value::value() : mType(TYPE_NULL), mStore(nullptr) {}
value::value(json::object object)
    : mType(TYPE_OBJECT),
      mStore(std::make_shared<store<json::object>>(object)) {}
value::value(json::pair pair)
    : mType(TYPE_OBJECT), mStore(std::make_shared<store<json::object>>(pair)) {}
value::value(json::array array)
    : mType(TYPE_ARRAY), mStore(std::make_shared<store<json::array>>(array)) {}
template <typename Number>
value::value(Number number, ENABLE_IF_NUMBER(Number))
    : mType(TYPE_NUMBER), mStore(std::make_shared<store<double>>(number)) {}
value::value(const char *string)
    : mType(TYPE_STRING),
      mStore(std::make_shared<store<std::string>>(string)) {}
value::value(std::string string)
    : mType(TYPE_STRING),
      mStore(std::make_shared<store<std::string>>(string)) {}
value::value(bool boolean)
    : mType(TYPE_BOOL), mStore(std::make_shared<store<bool>>(boolean)) {}
json::type &value::type() { return mType; }
const json::type &value::type() const { return mType; }

#undef ENABLE_IF_NUMBER
#define CAST(Type) static_cast<store<Type> *>(mStore.get())

json::object &value::object() { return CAST(json::object)->value; }
const json::object &value::object() const { return CAST(json::object)->value; }
json::array &value::array() { return CAST(json::array)->value; }
const json::array &value::array() const { return CAST(json::array)->value; }
double &value::number() { return CAST(double)->value; }
const double &value::number() const { return CAST(double)->value; }
std::string &value::string() { return CAST(std::string)->value; }
const std::string &value::string() const { return CAST(std::string)->value; }
bool &value::boolean() { return CAST(bool)->value; }
const bool &value::boolean() const { return CAST(bool)->value; }

#undef CAST

object::object() {}
object::object(std::string key, json::value value) { mEntries[key] = value; }
object::object(json::pair pair) { mEntries.insert(pair); }
template <typename Type>
object::object(std::string key, Type value) {
  mEntries[key] = value;
}
object::object(std::initializer_list<json::pair> pairs) {
  mEntries.insert(pairs);
}
void object::add(std::string key, json::value value) { mEntries[key] = value; }
void object::add(json::pair pair) { mEntries.insert(pair); }
json::value &object::get(std::string key) { return mEntries.at(key); }
const json::value &object::get(std::string key) const {
  return mEntries.at(key);
}
object::iterator object::begin() { return mEntries.begin(); }
object::const_iterator object::begin() const { return mEntries.begin(); }
object::iterator object::end() { return mEntries.end(); }
object::const_iterator object::end() const { return mEntries.end(); }

array::array(std::initializer_list<json::value> values) : mEntries(values) {}
template <typename... Args>
array::array(Args... args)
    : mEntries(args...) {}
void array::append(json::value value) { mEntries.push_back(value); }
array::iterator array::begin() { return mEntries.begin(); }
array::const_iterator array::begin() const { return mEntries.begin(); }
array::iterator array::end() { return mEntries.end(); }
array::const_iterator array::end() const { return mEntries.end(); }
json::value &array::operator[](const size_t index) { return mEntries[index]; }
const json::value &array::operator[](const size_t index) const {
  return mEntries[index];
}
json::value &array::at(const size_t index) { return mEntries.at(index); }
const json::value &array::at(const size_t index) const {
  return mEntries.at(index);
}
size_t array::size() { return mEntries.size(); }
size_t array::size() const { return mEntries.size(); }
}

#endif
