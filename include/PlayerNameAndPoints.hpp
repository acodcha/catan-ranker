#pragma once

#include "Base.hpp"

namespace CatanLeaderboard {

/// \brief Class that contains a player name and his/her number of points in a game.
class PlayerNameAndPoints {

public:

  PlayerNameAndPoints() noexcept {}

  PlayerNameAndPoints(const std::string& player_name, const uint_least8_t points) noexcept : player_name_(player_name), points_(points) {}

  /// \brief Constructor that takes a string containing a player name and a number of points such as "Alice 10".
  PlayerNameAndPoints(const std::string& player_and_points) {
    const std::string sanitized_player_and_points{remove_whitespace(player_and_points)};
    std::string points;
    for (const char character : sanitized_player_and_points) {
      if (isdigit(character)) {
        points += character;
      } else {
        player_name_ += character;
      }
    }
    const std::string error_message{"Cannot parse '" + player_and_points + "' into a player name and a number of points. Expected a format such as 'Alice 10'."};
    if (player_name_.empty()) {
      error(error_message);
    }
    const std::optional<uint_least64_t> number{string_to_natural_number(points)};
    if (number.has_value()) {
      points_ = number.value();
    } else {
      error(error_message);
    }
  }

  const std::string& player_name() const noexcept {
    return player_name_;
  }

  constexpr uint_least8_t points() const noexcept {
    return points_;
  }

  std::string print() const noexcept {
    return player_name_ + " " + std::to_string(points_);
  }

  bool operator==(const PlayerNameAndPoints& other) const noexcept {
    return player_name_ == other.player_name_ && points_ == other.points_;
  }

  bool operator!=(const PlayerNameAndPoints& other) const noexcept {
    return player_name_ != other.player_name_ || points_ != other.points_;
  }

protected:

  std::string player_name_;

  uint_least8_t points_{0};

};

class sort_by_descending_points {

public:

  bool operator()(const PlayerNameAndPoints& player_name_and_points_1, const PlayerNameAndPoints& player_name_and_points_2) const noexcept {
    if (player_name_and_points_1.points() > player_name_and_points_2.points()) {
      return true;
    } else if (player_name_and_points_1.points() < player_name_and_points_2.points()) {
      return false;
    } else {
      return player_name_and_points_1.player_name() < player_name_and_points_2.player_name();
    }
  }

};

} // namespace CatanLeaderboard
