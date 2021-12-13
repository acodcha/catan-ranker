#pragma once

#include "Games.hpp"
#include "PlayerProperties.hpp"

namespace CatanLeaderboardGenerator {

class Player {

public:

  Player() noexcept {}

  /// \brief Create a player who will not appear in plots.
  Player(const PlayerName& name) noexcept : name_(name) {}

  /// \brief Create a player who will appear in plots.
  Player(const PlayerName& name, const std::string& color) noexcept : name_(name), color_(color) {}

  /// \brief Update a player with a new game.
  Player(const Player& player, const Game& game, const std::map<PlayerName, std::map<GameCategory, EloRating>, PlayerName::sort>& previous_elo_ratings) noexcept : name_(player.name_), color_(player.color_), gnuplot_point_type_(player.gnuplot_point_type_), data_(player.data_) {
    add_game(game, previous_elo_ratings);
  }

  const PlayerName& name() const noexcept {
    return name_;
  }

  const std::optional<PlayerProperties> latest_properties(const GameCategory game_category) const noexcept {
    const std::map<GameCategory, std::vector<PlayerProperties>>::const_iterator category_history{data_.find(game_category)};
    if (category_history != data_.cend() && !category_history->second.empty()) {
      return category_history->second.back();
    } else {
      const std::optional<PlayerProperties> no_data;
      return no_data;
    }
  }

  const std::string& color() const noexcept {
    return color_;
  }

  const int8_t gnuplot_point_type() const noexcept {
    return gnuplot_point_type_;
  }

  EloRating lowest_elo_rating(const GameCategory game_category) const noexcept {
    EloRating lowest;
    const std::map<GameCategory, std::vector<PlayerProperties>>::const_iterator found{data_.find(game_category)};
    for (const PlayerProperties& player_properties : found->second) {
      if (lowest > player_properties.elo_rating()) {
        lowest = player_properties.elo_rating();
      }
    }
    return lowest;
  }

  EloRating highest_elo_rating(const GameCategory game_category) const noexcept {
    EloRating highest;
    const std::map<GameCategory, std::vector<PlayerProperties>>::const_iterator found{data_.find(game_category)};
    for (const PlayerProperties& player_properties : found->second) {
      if (highest < player_properties.elo_rating()) {
        highest = player_properties.elo_rating();
      }
    }
    return highest;
  }

  EloRating lowest_elo_rating() const noexcept {
    EloRating lowest;
    for (const GameCategory game_category : GameCategories) {
      const EloRating current{lowest_elo_rating(game_category)};
      if (lowest > current) {
        lowest = current;
      }
    }
    return lowest;
  }

  EloRating highest_elo_rating() const noexcept {
    EloRating highest;
    for (const GameCategory game_category : GameCategories) {
      const EloRating current{highest_elo_rating(game_category)};
      if (highest < current) {
        highest = current;
      }
    }
    return highest;
  }

  std::string print(const GameCategory game_category) const noexcept {
    const std::map<GameCategory, std::vector<PlayerProperties>>::const_iterator category_history{data_.find(game_category)};
    if (category_history != data_.cend() && !category_history->second.empty()) {
      return name_.value() + " : " + category_history->second.back().print();
    } else {
      return {};
    }
  }

  bool operator==(const Player& other) const noexcept {
    return name_ == other.name_;
  }

  bool operator!=(const Player& other) const noexcept {
    return name_ != other.name_;
  }

  const std::vector<PlayerProperties>& operator[](const GameCategory game_category) const noexcept {
    const std::map<GameCategory, std::vector<PlayerProperties>>::const_iterator found{data_.find(game_category)};
    return found->second;
  }

  struct sort {
    bool operator()(const Player& player_1, const Player& player_2) const noexcept {
      return PlayerName::sort()(player_1.name(), player_2.name());
    }
  };

  struct const_iterator : public std::map<GameCategory, std::vector<PlayerProperties>>::const_iterator {
    const_iterator(const std::map<GameCategory, std::vector<PlayerProperties>>::const_iterator i) noexcept : std::map<GameCategory, std::vector<PlayerProperties>>::const_iterator(i) {}
  };

  std::size_t size() const noexcept {
    return data_.size();
  }

  const_iterator cbegin() const noexcept {
   return const_iterator(data_.cbegin());
  }

  const_iterator begin() const noexcept {
   return cbegin();
  }

  const_iterator cend() const noexcept {
   return const_iterator(data_.cend());
  }

  const_iterator end() const noexcept {
   return cend();
  }

protected:

  PlayerName name_;

  std::string color_;

  int8_t gnuplot_point_type_{0};

  std::map<GameCategory, std::vector<PlayerProperties>> data_{
    {GameCategory::AnyNumberOfPlayers, {}},
    {GameCategory::ThreeToFourPlayers, {}},
    {GameCategory::FiveToSixPlayers, {}},
    {GameCategory::SevenToEightPlayers, {}}
  };

  void add_game(const Game& game, const std::map<PlayerName, std::map<GameCategory, EloRating>, PlayerName::sort>& previous_elo_ratings) noexcept {
    if (game.participant(name_)) {
      add_game(game, game.category(), previous_elo_ratings);
      add_game(game, GameCategory::AnyNumberOfPlayers, previous_elo_ratings);
    }
  }

  void add_game(const Game& game, const GameCategory game_category, const std::map<PlayerName, std::map<GameCategory, EloRating>, PlayerName::sort>& previous_elo_ratings) noexcept {
    // Get appropriate previous Elo ratings.
    std::map<PlayerName, EloRating, PlayerName::sort> previous;
    for (const std::pair<PlayerName, std::map<GameCategory, EloRating>> previous_elo_rating : previous_elo_ratings) {
      const std::map<GameCategory, EloRating>::const_iterator found{previous_elo_rating.second.find(game_category)};
      if (found != previous_elo_rating.second.cend()) {
        previous.emplace(previous_elo_rating.first, found->second);
      } else {
        error("Game category " + label(game_category) + " is missing from the previous Elo ratings map.");
      }
    }
    // Add new game.
    const std::map<GameCategory, std::vector<PlayerProperties>>::iterator history_any_number_of_players{data_.find(GameCategory::AnyNumberOfPlayers)};
    const std::map<GameCategory, std::vector<PlayerProperties>>::iterator history_same_game_category{data_.find(game_category)};
    if (history_any_number_of_players != data_.cend() && history_same_game_category != data_.end()) {
      if (history_any_number_of_players->second.empty() && history_same_game_category->second.empty()) {
        history_same_game_category->second.emplace_back(name_, game_category, game, previous);
      } else if (!history_any_number_of_players->second.empty() && history_same_game_category->second.empty()) {
        history_same_game_category->second.emplace_back(name_, game_category, game, previous, history_any_number_of_players->second.back());
      } else {
        history_same_game_category->second.emplace_back(name_, game_category, game, previous, history_any_number_of_players->second.back(), history_same_game_category->second.back());
      }
    }
  }

};

} // namespace CatanLeaderboardGenerator

namespace std {

  template <> struct hash<CatanLeaderboardGenerator::Player> {

    size_t operator()(const CatanLeaderboardGenerator::Player& player) const {
      return hash<CatanLeaderboardGenerator::PlayerName>()(player.name());
    }

  };

} // namespace std
