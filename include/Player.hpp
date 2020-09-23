#pragma once

#include "Games.hpp"
#include "PlayerProperties.hpp"

namespace CatanLeaderboard {

class Player {

public:

  Player() noexcept {}

  Player(const PlayerName& name) noexcept : name_(name) {}

  Player(const PlayerName& name, const Games& games) noexcept : name_(name) {
    for (const Game& game : games.data()) {
      if (game.participant(name_)) {
        if (game.number_of_players() <= 4) {
          if (three_to_four_player_games_.empty()) {
            three_to_four_player_games_.emplace_back(name_, game);
          } else {
            three_to_four_player_games_.emplace_back(name_, game, three_to_four_player_games_.back());
          }
          all_games_.push_back(three_to_four_player_games_.back());
        } else if (game.number_of_players() <= 6) {
          if (five_to_six_player_games_.empty()) {
            five_to_six_player_games_.emplace_back(name_, game);
          } else {
            five_to_six_player_games_.emplace_back(name_, game, five_to_six_player_games_.back());
          }
          all_games_.push_back(five_to_six_player_games_.back());
        } else {
          if (seven_to_eight_player_games_.empty()) {
            seven_to_eight_player_games_.emplace_back(name_, game);
          } else {
            seven_to_eight_player_games_.emplace_back(name_, game, seven_to_eight_player_games_.back());
          }
          all_games_.push_back(seven_to_eight_player_games_.back());
        }
      }
    }
  }

  const PlayerName& name() const noexcept {
    return name_;
  }

  std::string print_all_games() const noexcept {
    if (all_games_.empty()) {
      return {};
    } else {
      return name_.value() + " : " + std::to_string(all_games_.back().game_number()) + " games , " + real_number_to_string(all_games_.back().average_points_per_game()) + " points/game , " + all_games_.back().place_percentage({1}) + " 1st , " + all_games_.back().place_percentage({2}) + " 2nd , " + all_games_.back().place_percentage({3}) + " 3rd";
    }
  }

  std::string print_three_to_four_player_games() const noexcept {
    if (three_to_four_player_games_.empty()) {
      return {};
    } else {
      return name_.value() + " : " + std::to_string(three_to_four_player_games_.back().game_number()) + " games , " + real_number_to_string(three_to_four_player_games_.back().average_points_per_game()) + " points/game , " + three_to_four_player_games_.back().place_percentage({1}) + " 1st , " + three_to_four_player_games_.back().place_percentage({2}) + " 2nd , " + three_to_four_player_games_.back().place_percentage({3}) + " 3rd";
    }
  }

  std::string print_five_to_six_player_games() const noexcept {
    if (five_to_six_player_games_.empty()) {
      return {};
    } else {
      return name_.value() + " : " + std::to_string(five_to_six_player_games_.back().game_number()) + " games , " + real_number_to_string(five_to_six_player_games_.back().average_points_per_game()) + " points/game , " + five_to_six_player_games_.back().place_percentage({1}) + " 1st , " + five_to_six_player_games_.back().place_percentage({2}) + " 2nd , " + five_to_six_player_games_.back().place_percentage({3}) + " 3rd";
    }
  }

  std::string print_seven_to_eight_player_games() const noexcept {
    if (seven_to_eight_player_games_.empty()) {
      return {};
    } else {
      return name_.value() + " : " + std::to_string(seven_to_eight_player_games_.back().game_number()) + " games , " + real_number_to_string(seven_to_eight_player_games_.back().average_points_per_game()) + " points/game , " + seven_to_eight_player_games_.back().place_percentage({1}) + " 1st , " + seven_to_eight_player_games_.back().place_percentage({2}) + " 2nd , " + seven_to_eight_player_games_.back().place_percentage({3}) + " 3rd";
    }
  }

  bool operator==(const Player& other) const noexcept {
    return name_ == other.name_;
  }

  bool operator!=(const Player& other) const noexcept {
    return name_ != other.name_;
  }

  struct sort_by_alphabetical_name {
    bool operator()(const Player& player_1, const Player& player_2) const noexcept {
      return PlayerName::sort_alphabetical()(player_1.name(), player_2.name());
    }
  };

protected:

  PlayerName name_;

  std::vector<PlayerProperties> all_games_;

  std::vector<PlayerProperties> three_to_four_player_games_;

  std::vector<PlayerProperties> five_to_six_player_games_;

  std::vector<PlayerProperties> seven_to_eight_player_games_;

};

} // namespace CatanLeaderboard

namespace std {

  template <> struct hash<CatanLeaderboard::Player> {

    size_t operator()(const CatanLeaderboard::Player& player) const {
      return hash<CatanLeaderboard::PlayerName>()(player.name());
    }

  };

} // namespace std
