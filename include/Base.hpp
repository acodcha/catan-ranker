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

} // namespace CatanLeaderboard
