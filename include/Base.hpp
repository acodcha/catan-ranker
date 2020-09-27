#pragma once

#include "String.hpp"

namespace CatanLeaderboardGenerator {

/// \brief Namespace containing information about the program and its compilation.
namespace Program {

const std::string Title{"Catan Leaderboard Generator"};

const std::string CompilationDateAndTime{std::string{__DATE__} + ", " + std::string{__TIME__}};

const std::string Description{"Generates a simple leaderboard for Catan games."};

} // namespace Program

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
  const std::map<uint_least8_t, GameCategory>::const_iterator found_game_category{game_categories.find(number_of_players)};
  if (found_game_category != game_categories.cend()) {
    return found_game_category->second;
  } else {
    error("A game cannot have " + std::to_string(number_of_players) + " players.");
  }
}

std::string date_and_time(const std::tm* date_and_time) noexcept {
  const int year{date_and_time->tm_year + 1900};
  const int month{date_and_time->tm_mon + 1};
  const int day{date_and_time->tm_mday};
  const int hours{date_and_time->tm_hour};
  const int minutes{date_and_time->tm_min};
  const std::string year_string{std::to_string(year)};
  const std::string month_string{month < 10 ? "0" + std::to_string(month) : std::to_string(month)};
  const std::string day_string{day < 10 ? "0" + std::to_string(day) : std::to_string(day)};
  const std::string hours_string{hours < 10 ? "0" + std::to_string(hours) : std::to_string(hours)};
  const std::string minutes_string{minutes < 10 ? "0" + std::to_string(minutes) : std::to_string(minutes)};
  return year_string + "-" + month_string + "-" + day_string + " " + hours_string + ":" + minutes_string;
}

std::string current_local_date_and_time() noexcept {
  const std::time_t number{std::time(0)};
  const std::tm* current{std::localtime(&number)};
  return date_and_time(current);
}

std::string current_utc_date_and_time() noexcept {
  const std::time_t number{std::time(0)};
  const std::tm* current{std::gmtime(&number)};
  return date_and_time(current) + " UTC";
}

} // namespace CatanLeaderboardGenerator
