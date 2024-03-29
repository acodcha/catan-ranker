#pragma once

#include "Player.hpp"

namespace CatanRanker {

class Players {
public:
  Players(const Games& games) noexcept {
    // Obtain the player names and their total number of games played.
    std::map<PlayerName, int64_t> player_names_and_number_of_games;
    for (const Game& game : games) {
      const std::set<PlayerName, PlayerName::sort> game_player_names{
          game.player_names()};
      for (const PlayerName& player_name : game_player_names) {
        const std::map<PlayerName, int64_t>::iterator found{
            player_names_and_number_of_games.find(player_name)};
        if (found != player_names_and_number_of_games.end()) {
          ++found->second;
        } else {
          player_names_and_number_of_games.insert({player_name, 1});
        }
      }
    }
    std::multimap<int64_t, PlayerName, std::greater<int64_t>>
        number_of_games_and_player_names;
    for (const std::pair<const PlayerName, int64_t>&
             player_name_and_number_of_games :
         player_names_and_number_of_games) {
      number_of_games_and_player_names.insert(
          {player_name_and_number_of_games.second,
           player_name_and_number_of_games.first});
    }
    // Initialize the players with a player name and color.
    std::size_t counter{0};
    for (const std::pair<const int64_t, PlayerName>&
             number_of_games_and_player_name :
         number_of_games_and_player_names) {
      // Only assign a color to a few players with the most games.
      if (counter < ColorSequence.size()) {
        data_.insert({number_of_games_and_player_name.second, color(counter)});
      } else {
        data_.insert({number_of_games_and_player_name.second});
      }
      ++counter;
    }
    // Update all the players one game at a time.
    for (const Game& game : games) {
      // Obtain each player's previous Elo ratings.
      // These are needed for updating each player's Elo rating after the
      // current game.
      std::map<PlayerName, std::map<GameCategory, EloRating>, PlayerName::sort>
          previous_elo_ratings;
      for (const Player& player : data_) {
        std::map<GameCategory, EloRating> previous_elo_rating;
        for (const GameCategory game_category :
             {GameCategory::AnyNumberOfPlayers, game.category()}) {
          if (player[game_category].empty()) {
            previous_elo_rating.insert({game_category, {}});
          } else {
            previous_elo_rating.insert(
                {game_category, player[game_category].back().elo_rating()});
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
    const std::set<Player, Player::sort>::const_iterator datum{
        data_.find({name})};
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

  EloRating highest_elo_rating(
      const GameCategory game_category) const noexcept {
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

  struct const_iterator
    : public std::set<Player, Player::sort>::const_iterator {
    const_iterator(
        const std::set<Player, Player::sort>::const_iterator i) noexcept
      : std::set<Player, Player::sort>::const_iterator(i) {}
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

private:
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

}  // namespace CatanRanker
