#pragma once

#include "Game.hpp"

namespace CatanLeaderboard {

class PlayerProperties {

public:

  PlayerProperties() noexcept {}

  PlayerProperties(const PlayerName& name, const Game& game, const std::optional<PlayerProperties>& previous = {}) noexcept : date_(game.date()) {
    initialize_game_index(previous);
    initialize_points(name, game);
    initialize_average_points_per_game(game, previous);
    initialize_place_counts(name, game, previous);
    initialize_place_ratios();
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

  constexpr Points points() const noexcept {
    return points_;
  }

  constexpr double average_points_per_game() const noexcept {
    return average_points_per_game_;
  }

  const std::map<Place, uint_least64_t, Place::sort_ascending>& place_counts() const noexcept {
    return place_counts_;
  }

  /// \brief Number of Nth place finishes.
  uint_least64_t place_count(const Place place) const noexcept {
    const std::map<Place, uint_least64_t, Place::sort_ascending>::const_iterator found{place_counts_.find(place)};
    if (found != place_counts_.cend()) {
      return found->second;
    } else {
      return 0;
    }
  }

  /// \brief Ratio of Nth place finishes. Multiply by 100 to get a percentage.
  double place_ratio(const Place place) const noexcept {
    const std::map<Place, double, Place::sort_ascending>::const_iterator found{place_ratios_.find(place)};
    if (found != place_ratios_.cend()) {
      return found->second;
    } else {
      return 0.0;
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

  Points points_{0.0};

  double average_points_per_game_{0.0};

  std::map<Place, uint_least64_t, Place::sort_ascending> place_counts_;

  std::map<Place, double, Place::sort_ascending> place_ratios_;

  void initialize_game_index(const std::optional<PlayerProperties>& previous) noexcept {
    if (previous.has_value()) {
      game_index_ = previous.value().game_index() + 1;
    }
  }

  void initialize_points(const PlayerName& name, const Game& game) {
    const std::optional<Points> found_points{game.points(name)};
    if (found_points.has_value()) {
      points_ = found_points.value();
    } else {
      error("Player " + name.value() + " is not a participant in the game: " + game.print());
    }
  }

  void initialize_average_points_per_game(const Game& game, const std::optional<PlayerProperties>& previous) noexcept {
    if (previous.has_value()) {
      average_points_per_game_ = (previous.value().average_points_per_game() * previous.value().game_number() + (double)points_.value()) / game_number();
    } else {
      average_points_per_game_ = (double)points_.value();
    }
  }

  void initialize_place_counts(const PlayerName& name, const Game& game, const std::optional<PlayerProperties>& previous) noexcept {
    if (previous.has_value()) {
      place_counts_ = previous.value().place_counts_;
    }
    const std::optional<Place> found_place{game.place(name)};
    if (found_place.has_value()) {
      const std::map<Place, uint_least64_t, Place::sort_ascending>::iterator element{place_counts_.find(found_place.value())};
      if (element != place_counts_.end()) {
        ++(element->second);
      } else {
        place_counts_.emplace(found_place.value(), 1);
      }
    } else {
      error("Player " + name.value() + " is not a participant in the game: " + game.print());
    }
  }

  void initialize_place_ratios() noexcept {
    for (const auto& element : place_counts_) {
      place_ratios_.emplace(element.first, (double)element.second / game_number());
    }
  }

};

} // namespace CatanLeaderboard
