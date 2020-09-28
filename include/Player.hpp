#pragma once

#include "Games.hpp"
#include "PlayerProperties.hpp"

namespace CatanLeaderboardGenerator {

class Player {

public:

  Player() noexcept {}

  Player(const PlayerName& name) noexcept : name_(name) {}

  Player(const PlayerName& name, const Games& games) noexcept : name_(name) {
    for (const Game& game : games) {
      if (game.participant(name_)) {
        insert(game, game_category(game.number_of_players()));
        insert(game, GameCategory::AnyNumberOfPlayers);
      }
    }
  }

  const PlayerName& name() const noexcept {
    return name_;
  }

  std::string print(const GameCategory game_category) const noexcept {
    const std::map<GameCategory, std::vector<PlayerProperties>>::const_iterator category_history{data_.find(game_category)};
    if (category_history != data_.cend() && !category_history->second.empty()) {
      return name_.value() + " : " + std::to_string(category_history->second.back().game_number()) + " games , " + real_number_to_string(category_history->second.back().average_points_per_game(), 2) + " pts , " + category_history->second.back().place_percentage({1}).print() + " 1st , " + category_history->second.back().place_percentage({2}).print() + " 2nd , " + category_history->second.back().place_percentage({3}).print() + " 3rd";
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

  std::map<GameCategory, std::vector<PlayerProperties>> data_{
    {GameCategory::AnyNumberOfPlayers, {}},
    {GameCategory::ThreeToFourPlayers, {}},
    {GameCategory::FiveToSixPlayers, {}},
    {GameCategory::SevenToEightPlayers, {}}
  };

  void insert(const Game& game, const GameCategory game_category) noexcept {
    const std::map<GameCategory, std::vector<PlayerProperties>>::iterator history{data_.find(game_category)};
    if (history != data_.end()) {
      if (history->second.empty()) {
        history->second.emplace_back(name_, game);
      } else {
        history->second.emplace_back(name_, game, history->second.back());
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
