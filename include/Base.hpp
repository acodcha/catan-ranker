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

enum class GameCategory : uint_least8_t {
  AnyNumberOfPlayers,
  ThreeToFourPlayers,
  FiveToSixPlayers,
  SevenToEightPlayers
};

const std::map<uint_least8_t, GameCategory> game_categories{
  {3, GameCategory::ThreeToFourPlayers},
  {4, GameCategory::ThreeToFourPlayers},
  {5, GameCategory::FiveToSixPlayers},
  {6, GameCategory::FiveToSixPlayers},
  {7, GameCategory::SevenToEightPlayers},
  {8, GameCategory::SevenToEightPlayers}
};

GameCategory game_category(const uint_least8_t number_of_players) {
  const std::map<uint_least8_t, CatanLeaderboard::GameCategory>::const_iterator found_game_category{game_categories.find(number_of_players)};
  if (found_game_category != game_categories.cend()) {
    return found_game_category->second;
  } else {
    error("A game cannot have " + std::to_string(number_of_players) + " players.");
  }
}

} // namespace CatanLeaderboard
