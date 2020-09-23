#pragma once

#include "Game.hpp"

namespace CatanLeaderboard {

class PlayerProperties {

public:

  PlayerProperties() noexcept {}

  PlayerProperties(const PlayerName& name, const Game& game, const std::optional<PlayerProperties>& previous = {}) noexcept : date_(game.date()) {
    initialize_game_index(previous);
    initialize_average_points_per_game(name, game, previous);
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

  /// \brief Ratio of Nth place finishes.
  double place_ratio(const Place place) const noexcept {
    const std::map<Place, double, Place::sort_ascending>::const_iterator found{place_ratios_.find(place)};
    if (found != place_ratios_.cend()) {
      return found->second;
    } else {
      return 0.0;
    }
  }

  /// \brief Percentage of Nth place finishes. Returned as a string, such as "30%".
  std::string place_percentage(const Place place) const noexcept {
    return std::to_string((uint_least64_t)std::round(place_ratio(place) * 100)) + "%";
  }

  struct sort_by_ascending_game_index {
    bool operator()(const PlayerProperties& player_properties_1, const PlayerProperties& player_properties_2) const noexcept {
      return player_properties_1.game_index() < player_properties_2.game_index();
    }
  };

protected:

  uint_least64_t game_index_{0};

  Date date_;

  double average_points_per_game_{0.0};

  std::map<Place, uint_least64_t, Place::sort_ascending> place_counts_;

  std::map<Place, double, Place::sort_ascending> place_ratios_;

  void initialize_game_index(const std::optional<PlayerProperties>& previous) noexcept {
    if (previous.has_value()) {
      game_index_ = previous.value().game_index() + 1;
    }
  }

  void initialize_average_points_per_game(const PlayerName& name, const Game& game, const std::optional<PlayerProperties>& previous) noexcept {
    const std::optional<Points> found_points{game.points(name)};
    if (found_points.has_value()) {
      // The rounding is used to accommodate points values such as 10.1.
      // Such values are used to identify the winning player in games where multiple players score 10 points.
      const Points points{std::round(found_points.value().value())};
      if (previous.has_value()) {
        average_points_per_game_ = (previous.value().average_points_per_game() * previous.value().game_number() + points.value()) / game_number();
      } else {
        average_points_per_game_ = points.value();
      }
    } else {
      error("Player " + name.value() + " is not a participant in the game: " + game.print());
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
    for (const std::pair<Place, uint_least64_t>& element : place_counts_) {
      place_ratios_.emplace(element.first, (double)element.second / game_number());
    }
  }

};

} // namespace CatanLeaderboard
