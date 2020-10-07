#pragma once

#include "Games.hpp"
#include "PlayerProperties.hpp"

namespace CatanLeaderboardGenerator {

// TODO: Rewrite the Player class to use an add_game() method for initialization instead of a Games object. This adds one PlayerProperties to the appropriate category vector.

class Player {

public:

  Player() noexcept {}

  Player(const PlayerName& name) noexcept : name_(name) {}

  Player(const PlayerName& name, const std::string& color, const uint_least8_t gnuplot_point_type) noexcept : name_(name), color_(color), gnuplot_point_type_(gnuplot_point_type) {}

  Player(
    const PlayerName& name,
    const std::string& color,
    const uint_least8_t gnuplot_point_type,
    const Games& games
  ) noexcept :
    name_(name),
    color_(color),
    gnuplot_point_type_(gnuplot_point_type)
  {
    for (const Game& game : games) {
      add_game(game);
    }
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

  const uint_least8_t gnuplot_point_type() const noexcept {
    return gnuplot_point_type_;
  }

  std::string print(const GameCategory game_category) const noexcept {
    const std::map<GameCategory, std::vector<PlayerProperties>>::const_iterator category_history{data_.find(game_category)};
    if (category_history != data_.cend() && !category_history->second.empty()) {
      return name_.value() + " : " + std::to_string(category_history->second.back().player_game_category_game_number()) + " games , " + real_number_to_string(category_history->second.back().average_points_per_game(), 2) + " pts , " + category_history->second.back().place_percentage({1}).print() + " 1st , " + category_history->second.back().place_percentage({2}).print() + " 2nd , " + category_history->second.back().place_percentage({3}).print() + " 3rd";
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
    const std::map<CatanLeaderboardGenerator::GameCategory, std::vector<CatanLeaderboardGenerator::PlayerProperties>>::const_iterator found{data_.find(game_category)};
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

  uint_least8_t gnuplot_point_type_{0};

  std::map<GameCategory, std::vector<PlayerProperties>> data_{
    {GameCategory::AnyNumberOfPlayers, {}},
    {GameCategory::ThreeToFourPlayers, {}},
    {GameCategory::FiveToSixPlayers, {}},
    {GameCategory::SevenToEightPlayers, {}}
  };

  void add_game(const Game& game, const GameCategory game_category) noexcept {
    const std::map<GameCategory, std::vector<PlayerProperties>>::iterator history_any_number_of_players{data_.find(GameCategory::AnyNumberOfPlayers)};
    const std::map<GameCategory, std::vector<PlayerProperties>>::iterator history_same_game_category{data_.find(game_category)};
    if (history_any_number_of_players != data_.cend() && history_same_game_category != data_.end()) {
      if (history_any_number_of_players->second.empty() && history_same_game_category->second.empty()) {
        history_same_game_category->second.emplace_back(name_, game_category, game);
      } else if (!history_any_number_of_players->second.empty() && history_same_game_category->second.empty()) {
        history_same_game_category->second.emplace_back(name_, game_category, game, history_any_number_of_players->second.back());
      } else {
        history_same_game_category->second.emplace_back(name_, game_category, game, history_any_number_of_players->second.back(), history_same_game_category->second.back());
      }
    }
  }

  void add_game(const Game& game) noexcept {
    if (game.participant(name_)) {
      add_game(game, game.category());
      add_game(game, GameCategory::AnyNumberOfPlayers);
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
