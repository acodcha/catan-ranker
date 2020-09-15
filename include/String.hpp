#pragma once

#include "Include.hpp"

namespace CatanLeaderboard {

std::vector<std::string> split_by_whitespace(const std::string& text) noexcept {
  std::istringstream stream{text};
  std::vector<std::string> words{std::istream_iterator<std::string>{stream}, std::istream_iterator<std::string>{}};
  return words;
}

std::vector<std::string> split(const std::string& text, const char delimiter) noexcept {
  std::stringstream stream(text);
  std::string word;
  std::vector<std::string> words;
  while (std::getline(stream, word, delimiter)) {
    words.push_back(word);
  }
  return words;
}

std::string remove_whitespace(const std::string& text) noexcept {
  std::string new_text{text};
  new_text.erase(remove_if(new_text.begin(), new_text.end(), ::isspace), new_text.end());
  return new_text;
}

/// \brief Pad a string to a given length using trailing spaces.
/// \details If the string is already longer than the given length, nothing is changed.
std::string pad_to_length(const std::string& text, uint_least64_t length) noexcept {
  std::string padded_text{text};
  if (length > padded_text.length()) {
    padded_text.append(length - padded_text.length(), ' ');
  }
  return padded_text;
}

std::string real_number_to_string(const double value) noexcept {
  if (value == 0.0) {
    return {"0"};
  } else {
    const double absolute_value{std::abs(value)};
    std::ostringstream stream;
    if (absolute_value >= 10000.0 || absolute_value < 0.001) {
      stream << std::scientific << value;
      return stream.str();
    } else if (absolute_value >= 1000.0) {
      stream << std::fixed << std::setprecision(3) << value;
      return stream.str();
    } else if (absolute_value >= 100.0) {
      stream << std::fixed << std::setprecision(4) << value;
      return stream.str();
    } else if (absolute_value >= 10.0) {
      stream << std::fixed << std::setprecision(5) << value;
      return stream.str();
    } else if (absolute_value >= 1.0) {
      stream << std::fixed << std::setprecision(6) << value;
      return stream.str();
    } else if (absolute_value >= 0.1) {
      stream << std::fixed << std::setprecision(7) << value;
      return stream.str();
    } else if (absolute_value >= 0.01) {
      stream << std::fixed << std::setprecision(8) << value;
      return stream.str();
    } else {
      stream << std::fixed << std::setprecision(9) << value;
      return stream.str();
    }
  }
}

std::optional<uint_least64_t> string_to_natural_number(const std::string& text) noexcept {
  char* end = 0;
  const unsigned long long int value = std::strtoull(text.c_str(), &end, 10);
  if (end != text.c_str() && *end == '\0' && value != ULLONG_MAX) {
    return {(uint_least64_t)(value)};
  }
  const std::optional<uint_least64_t> no_value;
  return no_value;
}

std::optional<int_least64_t> string_to_integer_number(const std::string& text) noexcept {
  char* end = 0;
  const long long int value = std::strtoll(text.c_str(), &end, 10);
  if (end != text.c_str() && *end == '\0' && value != LLONG_MAX) {
    return {(int_least64_t)(value)};
  }
  const std::optional<int_least64_t> no_value;
  return no_value;
}

std::optional<double> string_to_real_number(const std::string& text) noexcept {
  char* end = 0;
  const double value = strtod(text.c_str(), &end);
  if (end != text.c_str() && *end == '\0' && value != HUGE_VAL && value != -HUGE_VAL) {
    return {value};
  }
  const std::optional<double> no_value;
  return no_value;
}

} // namespace CatanLeaderboard
