#pragma once

#include "Player.hpp"

namespace CatanLeaderboard {

class Players {

public:

  Players(const Games& games) noexcept {
    std::set<PlayerName, PlayerName::sort> games_player_names;
    for (const Game& game : games.data()) {
      const std::set<PlayerName, PlayerName::sort> game_player_names{game.player_names()};
      games_player_names.insert(game_player_names.cbegin(), game_player_names.cend());
    }
    for (const PlayerName& player_name : games_player_names) {
      data_.insert({player_name, {player_name, games}});
    }
  }

  const Player& player(const PlayerName& name) const {
    const std::map<PlayerName, Player, PlayerName::sort>::const_iterator datum{data_.find(name)};
    if (datum != data_.cend()) {
      return datum->second;
    } else {
      error(name.value() + " is not a player.");
    }
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

  std::map<PlayerName, Player, PlayerName::sort> data_;

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
    for (const std::pair<PlayerName, Player>& datum : data_) {
      const std::string text{datum.second.print(game_category)};
      if (!text.empty()) {
        stream << std::endl << "- " << text;
      }
    }
    return stream.str();
  }

};

} // namespace CatanLeaderboard
