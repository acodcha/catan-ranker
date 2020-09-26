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

enum class RealNumber : uint_least8_t {
  FloatingPoint,
  Percentage
};

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

std::string print_date_and_time(const std::tm* datetime) noexcept {
  const int year{datetime->tm_year + 1900};
  const int month{datetime->tm_mon + 1};
  const int day{datetime->tm_mday};
  const int hours{datetime->tm_hour + 1};
  const int minutes{datetime->tm_min + 1};
  const int seconds{datetime->tm_sec + 1};
  const std::string year_string{std::to_string(year)};
  const std::string month_string{month < 10 ? "0" + std::to_string(month) : std::to_string(month)};
  const std::string day_string{day < 10 ? "0" + std::to_string(day) : std::to_string(day)};
  const std::string hours_string{hours < 10 ? "0" + std::to_string(hours) : std::to_string(hours)};
  const std::string minutes_string{minutes < 10 ? "0" + std::to_string(minutes) : std::to_string(minutes)};
  const std::string seconds_string{seconds < 10 ? "0" + std::to_string(seconds) : std::to_string(seconds)};
  return year_string + "-" + month_string + "-" + day_string + " " + hours_string + ":" + minutes_string + ":" + seconds_string;
}

std::string current_local_date_and_time() noexcept {
  const std::time_t number{std::time(0)};
  const std::tm* current{std::localtime(&number)};
  return print_date_and_time(current);
}

std::string current_utc_date_and_time() noexcept {
  const std::time_t number{std::time(0)};
  const std::tm* current{std::gmtime(&number)};
  return print_date_and_time(current) + " UTC";
}

} // namespace CatanLeaderboard
