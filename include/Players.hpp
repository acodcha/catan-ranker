#pragma once

#include "Player.hpp"

namespace CatanLeaderboardGenerator {

class Players {

public:

  Players(const Games& games) noexcept {
    // Obtain the player names.
    std::set<PlayerName, PlayerName::sort> games_player_names;
    for (const Game& game : games) {
      const std::set<PlayerName, PlayerName::sort> game_player_names{game.player_names()};
      games_player_names.insert(game_player_names.cbegin(), game_player_names.cend());
    }
    // Initialize the players with a player name, color, and Gnuplot point type.
    uint_least64_t counter{0};
    for (const PlayerName& player_name : games_player_names) {
      data_.insert({player_name, color(counter), gnuplot_point_type(counter)});
      ++counter;
    }
    // Update all the players one game at a time.
    for (const Game& game : games) {
      // Obtain each player's previous Elo ratings.
      // These are needed for updating each player's Elo rating after the current game.
      std::map<PlayerName, std::map<GameCategory, EloRating>, PlayerName::sort> previous_elo_ratings;
      for (const Player& player : data_) {
        std::map<GameCategory, EloRating> previous_elo_rating;
        for ( const GameCategory game_category : {GameCategory::AnyNumberOfPlayers, game.category()}) {
          if (player[game_category].empty()) {
            previous_elo_rating.insert({game_category, {}});
          } else {
            previous_elo_rating.insert({game_category, player[game_category].back().elo_rating()});
          }
        }
        previous_elo_ratings.insert({player.name(), previous_elo_rating});
      }
      // Update each player with the current game.
      std::set<Player, Player::sort> updated_data;
      for (const Player& player : data_) {
        updated_data.emplace(player, game, previous_elo_ratings);
      }
      data_ = updated_data;
    }
    message(print());
  }

  const Player& find(const PlayerName& name) const {
    const std::set<Player, Player::sort>::const_iterator datum{data_.find({name})};
    if (datum != data_.cend()) {
      return *datum;
    } else {
      error(name.value() + " is not a player.");
    }
  }

  EloRating lowest_elo_rating(const GameCategory game_category) const noexcept {
    EloRating lowest;
    for (const Player& player : data_) {
      EloRating player_lowest{player.lowest_elo_rating(game_category)};
      if (lowest > player_lowest) {
        lowest = player_lowest;
      }
    }
    return lowest;
  }

  EloRating highest_elo_rating(const GameCategory game_category) const noexcept {
    EloRating highest;
    for (const Player& player : data_) {
      EloRating player_highest{player.highest_elo_rating(game_category)};
      if (highest < player_highest) {
        highest = player_highest;
      }
    }
    return highest;
  }

  std::string print() const noexcept {
    std::stringstream stream;
    stream << "There are " << data_.size() << " players:";
    for (const Player& player : data_) {
      stream << std::endl << "- " << player.name().value();
    }
    stream << std::endl << print(GameCategory::AnyNumberOfPlayers);
    stream << std::endl << print(GameCategory::ThreeToFourPlayers);
    stream << std::endl << print(GameCategory::FiveToSixPlayers);
    stream << std::endl << print(GameCategory::SevenToEightPlayers);
    return stream.str();
  }

  struct const_iterator : public std::set<Player, Player::sort>::const_iterator {
    const_iterator(const std::set<Player, Player::sort>::const_iterator i) noexcept : std::set<Player, Player::sort>::const_iterator(i) {}
  };

  bool empty() const noexcept {
    return data_.empty();
  }

  std::size_t size() const noexcept {
    return data_.size();
  }

  const_iterator cbegin() const noexcept {
   return const_iterator(data_.cbegin());
  }

  const_iterator begin() const noexcept {
   return const_iterator(data_.cbegin());
  }

  const_iterator cend() const noexcept {
   return const_iterator(data_.cend());
  }

  const_iterator end() const noexcept {
   return const_iterator(data_.cend());
  }

protected:

  std::set<Player, Player::sort> data_;

  std::string print(const GameCategory game_category) const noexcept {
    std::stringstream stream;
    switch (game_category) {
      case GameCategory::AnyNumberOfPlayers:
        stream << "Results for all games:";
        break;
      case GameCategory::ThreeToFourPlayers:
        stream << "Results for 3-4 player games:";
        break;
      case GameCategory::FiveToSixPlayers:
        stream << "Results for 5-6 player games:";
        break;
      case GameCategory::SevenToEightPlayers:
        stream << "Results for 7-8 player games:";
        break;
    }
    for (const Player& datum : data_) {
      const std::string text{datum.print(game_category)};
      if (!text.empty()) {
        stream << std::endl << "- " << text;
      }
    }
    return stream.str();
  }

};

} // namespace CatanLeaderboardGenerator
