#pragma once

#include "Game.hpp"

namespace CatanLeaderboard {

class PlayerProperties {

public:

  PlayerProperties() noexcept {}

  PlayerProperties(const std::string& name, const Game& game, const std::optional<PlayerProperties>& previous = {}) noexcept : date_(game.date()) {
    initialize_game_index(previous);
    initialize_points(name, game);
    initialize_average_points_per_game(game, previous);
    initialize_place_counts(game, previous);
  }

  constexpr uint_least8_t game_index() const noexcept {
    return game_index_;
  }

  constexpr uint_least8_t game_number() const noexcept {
    return game_index_ + 1;
  }

  constexpr const Date& date() const noexcept {
    return date_;
  }

  constexpr uint_least8_t points() const noexcept {
    return points_;
  }

  constexpr double average_points_per_game() const noexcept {
    return average_points_per_game_;
  }

  constexpr const std::map<uint_least8_t, uint_least64_t>& place_counts() const noexcept {
    return place_counts_;
  }

  /// \brief Number of Nth place finishes, where N ranges from 1 to 8 inclusive.
  /// \details For example, place_counts_.find(2) returns the number of second-place finishes.
  uint_least64_t place_count(const uint_least8_t place) const noexcept {
    const std::map<uint_least8_t, uint_least64_t>::const_iterator found_place{place_counts_.find(place)};
    if (found_place != place_counts_.cend()) {
      return found_place->second;
    } else {
      return 0;
    }
  }

  struct sort_by_ascending_game_index {
    bool operator()(const PlayerProperties& player_properties_1, const PlayerProperties& player_properties_2) const noexcept {
      return player_properties_1.game_index() < player_properties_2.game_index();
    }
  };

protected:

  uint_least64_t game_index_{0};

  Date date_;

  uint_least8_t points_{0};

  double average_points_per_game_{0.0};

  std::map<uint_least8_t, uint_least64_t> place_counts_;

  void initialize_game_index(const std::optional<PlayerProperties>& previous) noexcept {
    if (previous.has_value()) {
      game_index_ = previous.value().game_index() + 1;
    }
  }

  void initialize_points(const std::string& name, const Game& game) {
    const std::optional<uint_least8_t> found_points{game.points(name)};
    if (found_points.has_value()) {
      points_ = found_points.value();
    } else {
      error("Player " + name + " is not a participant in the game: " + game.print());
    }
  }

  void initialize_average_points_per_game(const Game& game, const std::optional<PlayerProperties>& previous) noexcept {
    if (previous.has_value()) {
      average_points_per_game_ = (previous.value().average_points_per_game() * previous.value().game_number() + (double)points_) / game_number();
    } else {
      average_points_per_game_ = (double)points_;
    }
  }

  void initialize_place_counts(const Game& game, const std::optional<PlayerProperties>& previous) noexcept {





  }

};

} // namespace CatanLeaderboard
