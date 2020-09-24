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
    stream << std::endl << print(GameCategory::AnyNumberOfPlayers);
    stream << std::endl << print(GameCategory::ThreeToFourPlayers);
    stream << std::endl << print(GameCategory::FiveToSixPlayers);
    stream << std::endl << print(GameCategory::SevenToEightPlayers);
    return stream.str();
  }

  void message() const noexcept {
    CatanLeaderboard::message(print());
  }

protected:

  std::set<Player, Player::sort_by_alphabetical_name> data_;

  std::string print(const GameCategory game_category) const noexcept {
    std::stringstream stream;
    switch (game_category) {
      case GameCategory::AnyNumberOfPlayers:
        stream << "All games:";
        break;
      case GameCategory::ThreeToFourPlayers:
        stream << "3-4 player games:";
        break;
      case GameCategory::FiveToSixPlayers:
        stream << "5-6 player games:";
        break;
      case GameCategory::SevenToEightPlayers:
        stream << "7-8 player games:";
        break;
    }
    for (const Player& player : data_) {
      const std::string text{player.print(game_category)};
      if (!text.empty()) {
        stream << std::endl << "- " << text;
      }
    }
    return stream.str();
  }

};

} // namespace CatanLeaderboard
