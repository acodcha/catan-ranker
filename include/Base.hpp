#pragma once

#include "String.hpp"

namespace CatanLeaderboard {

/// \brief Print a general-purpose message to the console.
inline void message(const std::string &text) noexcept {
  std::cout << text << std::endl;
}

/// \brief Print a warning to the console.
inline void warning(const std::string &text) noexcept {
  std::cout << "Warning: " << text << std::endl;
}

/// \brief Throw an exception.
inline void error(const std::string &text) {
  throw std::runtime_error(text);
}

template <typename Enumeration> const std::map<Enumeration, std::string> labels;

template <typename Enumeration> std::string print(const Enumeration enumeration) noexcept {
  return labels<Enumeration>.find(enumeration)->second;
}

template <typename Enumeration> const std::unordered_map<std::string, Enumeration> spellings;

template <typename Enumeration> std::optional<Enumeration> parse(const std::string& spelling) noexcept {
  const typename std::unordered_map<std::string, Enumeration>::const_iterator enumeration{spellings<Enumeration>.find(spelling)};
  if (enumeration != spellings<Enumeration>.cend()) {
    return {enumeration->second};
  } else {
    return {};
  }
}

} // namespace CatanLeaderboard
