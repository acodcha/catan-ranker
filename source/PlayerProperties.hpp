#pragma once

#include "EloRating.hpp"
#include "Percentage.hpp"

namespace CatanLeaderboardGenerator {

class PlayerProperties {

public:

  PlayerProperties() noexcept {}

  PlayerProperties(
    const PlayerName& name,
    const GameCategory game_category,
    const Game& game,
    const std::map<PlayerName, EloRating, PlayerName::sort>& previous_elo_ratings,
    const std::optional<PlayerProperties>& previous_any_number_of_players = std::optional<PlayerProperties>{},
    const std::optional<PlayerProperties>& previous_same_game_category = std::optional<PlayerProperties>{}
  ) noexcept :
    game_index_(game.index()),
    date_(game.date())
  {
    initialize_game_category_game_index(game_category, game);
    initialize_player_game_index(previous_any_number_of_players);
    initialize_player_game_category_game_index(previous_same_game_category);
    initialize_average_points_per_game(name, game, previous_same_game_category);
    initialize_place_counts(name, game, previous_same_game_category);
    initialize_place_percentages();
    initialize_elo_rating(name, game, previous_elo_ratings);
    initialize_average_elo_rating(previous_same_game_category);
  }

  /// \brief Game number of this game.
  constexpr uint64_t game_number() const noexcept {
    return game_index_ + 1;
  }

  /// \brief Game number within its game category of this game.
  constexpr uint64_t game_category_game_number() const noexcept {
    return game_category_game_index_ + 1;
  }

  /// \brief Number of games played by this player in any game category.
  constexpr uint64_t player_game_number() const noexcept {
    return player_game_index_ + 1;
  }

  /// \brief Number of games played by this player in this game's game category.
  constexpr uint64_t player_game_category_game_number() const noexcept {
    return player_game_category_game_index_ + 1;
  }

  constexpr const Date& date() const noexcept {
    return date_;
  }

  constexpr double average_points_per_game() const noexcept {
    return average_points_per_game_;
  }

  /// \brief Number of Nth place finishes.
  uint64_t place_count(const Place place) const noexcept {
    const std::map<Place, uint64_t, Place::sort>::const_iterator found{place_counts_.find(place)};
    if (found != place_counts_.cend()) {
      return found->second;
    } else {
      return 0;
    }
  }

  /// \brief Ratio of Nth place finishes.
  Percentage place_percentage(const Place place) const noexcept {
    const std::map<Place, Percentage, Place::sort>::const_iterator found{place_percentages_.find(place)};
    if (found != place_percentages_.cend()) {
      return found->second;
    } else {
      return {0.0};
    }
  }

  constexpr const EloRating& elo_rating() const noexcept {
    return elo_rating_;
  }

  constexpr const EloRating& average_elo_rating() const noexcept {
    return average_elo_rating_;
  }

  std::string print() const noexcept {
    return std::to_string(player_game_category_game_number()) + " games , " + average_elo_rating_.print() + " average rating , " + elo_rating_.print() + " current rating , " + real_number_to_string(average_points_per_game_, 3) + " pts , " + place_percentage({1}).print() + " 1st , " + place_percentage({2}).print() + " 2nd , " + place_percentage({3}).print() + " 3rd";
  }

  struct sort {
    bool operator()(const PlayerProperties& player_properties_1, const PlayerProperties& player_properties_2) const noexcept {
      return player_properties_1.player_game_category_game_index_ < player_properties_2.player_game_category_game_index_;
    }
  };

protected:

  uint64_t game_index_{0};

  uint64_t game_category_game_index_{0};

  uint64_t player_game_index_{0};

  uint64_t player_game_category_game_index_{0};

  Date date_;

  double average_points_per_game_{0.0};

  std::map<Place, uint64_t, Place::sort> place_counts_;

  std::map<Place, Percentage, Place::sort> place_percentages_;

  EloRating elo_rating_;

  EloRating average_elo_rating_;

  void initialize_game_category_game_index(const GameCategory game_category, const Game& game) noexcept {
    if (game_category == GameCategory::AnyNumberOfPlayers) {
      game_category_game_index_ = game_index_;
    } else {
      game_category_game_index_ = game.category_index();
    }
  }

  void initialize_player_game_index(const std::optional<PlayerProperties>& previous_any_number_of_players) noexcept {
    if (previous_any_number_of_players.has_value()) {
      player_game_index_ = previous_any_number_of_players.value().player_game_index_ + 1;
    }
  }

  void initialize_player_game_category_game_index(const std::optional<PlayerProperties>& previous_same_game_category) noexcept {
    if (previous_same_game_category.has_value()) {
      player_game_category_game_index_ = previous_same_game_category.value().player_game_category_game_index_ + 1;
    }
  }

  void initialize_average_points_per_game(const PlayerName& name, const Game& game, const std::optional<PlayerProperties>& previous_same_game_category) noexcept {
    const std::optional<Points> found_points{game.points_limited_to_10(name)};
    if (found_points.has_value()) {
      if (previous_same_game_category.has_value()) {
        average_points_per_game_ = (previous_same_game_category.value().average_points_per_game_ * previous_same_game_category.value().player_game_category_game_number() + found_points.value().value()) / player_game_category_game_number();
      } else {
        average_points_per_game_ = static_cast<double>(found_points.value().value());
      }
    } else {
      error("Player " + name.value() + " is not a participant in the game: " + game.print());
    }
  }

  void initialize_place_counts(const PlayerName& name, const Game& game, const std::optional<PlayerProperties>& previous_same_game_category) noexcept {
    if (previous_same_game_category.has_value()) {
      place_counts_ = previous_same_game_category.value().place_counts_;
    }
    const std::optional<Place> found_place{game.place(name)};
    if (found_place.has_value()) {
      const std::map<Place, uint64_t, Place::sort>::iterator element{place_counts_.find(found_place.value())};
      if (element != place_counts_.end()) {
        ++(element->second);
      } else {
        place_counts_.emplace(found_place.value(), 1);
      }
    } else {
      error("Player " + name.value() + " is not a participant in the game: " + game.print());
    }
  }

  void initialize_place_percentages() noexcept {
    for (const std::pair<Place, uint64_t>& element : place_counts_) {
      place_percentages_.insert({element.first, {static_cast<double>(element.second) / player_game_category_game_number()}});
    }
  }

  void initialize_elo_rating(
    const PlayerName& player_name,
    const Game& game,
    const std::map<PlayerName, EloRating, PlayerName::sort>& previous_elo_ratings
  ) noexcept {
    elo_rating_ = update_elo_rating(player_name, game, previous_elo_ratings);
  }

  void initialize_average_elo_rating(const std::optional<PlayerProperties>& previous_same_game_category) noexcept {
    if (previous_same_game_category.has_value()) {
      average_elo_rating_ = (previous_same_game_category.value().average_elo_rating_ * previous_same_game_category.value().player_game_category_game_number() + elo_rating_) / (player_game_category_game_number());
    } else {
      average_elo_rating_ = elo_rating_;
    }
  }

};

} // namespace CatanLeaderboardGenerator
