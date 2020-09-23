#pragma once

#include "Player.hpp"

namespace CatanLeaderboard {

class Players {

public:

  Players(const Games& games) noexcept {
    std::set<PlayerName, PlayerName::sort_alphabetical> games_player_names;
    for (const Game& game : games.data()) {
      const std::set<PlayerName, PlayerName::sort_alphabetical> game_player_names{game.player_names()};
      games_player_names.insert(game_player_names.cbegin(), game_player_names.cend());
    }
    for (const PlayerName& player_name : games_player_names) {
      data_.emplace(player_name, games);
    }
  }

  const std::set<Player, Player::sort_by_alphabetical_name>& data() const noexcept {
    return data_;
  }

  std::string print() const noexcept {
    std::stringstream stream;
    stream << "There are " << data_.size() << " players.";
    stream << std::endl << print_all_games();
    stream << std::endl << print_three_to_four_player_games();
    stream << std::endl << print_five_to_six_player_games();
    stream << std::endl << print_seven_to_eight_player_games();
    return stream.str();
  }

  void message() const noexcept {
    CatanLeaderboard::message(print());
  }

protected:

  std::set<Player, Player::sort_by_alphabetical_name> data_;

  std::string print_all_games() const noexcept {
    std::stringstream stream;
    stream << "All games:";
    for (const Player& player : data_) {
      const std::string text{player.print_all_games()};
      if (!text.empty()) {
        stream << std::endl << "- " << text;
      }
    }
    return stream.str();
  }

  std::string print_three_to_four_player_games() const noexcept {
    std::stringstream stream;
    stream << "3-4 player games:";
    for (const Player& player : data_) {
      const std::string text{player.print_three_to_four_player_games()};
      if (!text.empty()) {
        stream << std::endl << "- " << text;
      }
    }
    return stream.str();
  }

  std::string print_five_to_six_player_games() const noexcept {
    std::stringstream stream;
    stream << "5-6 player games:";
    for (const Player& player : data_) {
      const std::string text{player.print_five_to_six_player_games()};
      if (!text.empty()) {
        stream << std::endl << "- " << text;
      }
    }
    return stream.str();
  }

  std::string print_seven_to_eight_player_games() const noexcept {
    std::stringstream stream;
    stream << "7-8 player games:";
    for (const Player& player : data_) {
      const std::string text{player.print_seven_to_eight_player_games()};
      if (!text.empty()) {
        stream << std::endl << "- " << text;
      }
    }
    return stream.str();
  }

};

} // namespace CatanLeaderboard
