#pragma once

#include "Games.hpp"
#include "PlayerProperties.hpp"

namespace CatanLeaderboardGenerator {

class Player {

public:

  Player() noexcept {}

  Player(const PlayerName& name) noexcept : name_(name) {}

  Player(
    const PlayerName& name,
    const Games& games,
    const std::string& color,
    const uint_least8_t gnuplot_point_type
  ) noexcept :
    name_(name),
    color_(color),
    gnuplot_point_type_(gnuplot_point_type)
  {
    uint_least64_t player_game_index{0};
    for (const Game& game : games) {
      if (game.participant(name_)) {
        insert(game, player_game_index, game.category());
        insert(game, player_game_index, GameCategory::AnyNumberOfPlayers);
        ++player_game_index;
      }
    }
  }

  const PlayerName& name() const noexcept {
    return name_;
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

  void insert(const Game& game, const uint_least64_t player_game_index, const GameCategory game_category) noexcept {
    const std::map<GameCategory, std::vector<PlayerProperties>>::iterator history{data_.find(game_category)};
    if (history != data_.end()) {
      if (history->second.empty()) {
        history->second.emplace_back(name_, game_category, game, player_game_index);
      } else {
        history->second.emplace_back(name_, game_category, game, player_game_index, history->second.back());
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
