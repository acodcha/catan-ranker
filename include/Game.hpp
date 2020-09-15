#pragma once

#include "Date.hpp"
#include "PlayerNameAndPoints.hpp"

namespace CatanLeaderboard {

class Game {

public:

  Game() noexcept {}

  Game(const Date& date, const std::set<PlayerNameAndPoints, sort_by_descending_points>& player_names_and_points) noexcept : date_(date), player_names_and_points_(player_names_and_points) {
    initialize_player_names();
  }

  /// \brief Constructor that takes a string containing a date and a list of player names and numbers of points such as "2020-03-15 : Alice 10 , Bob 8 , Claire 7 , David 6".
  Game(const std::string& date_with_player_names_and_points) {
    const std::string error_message{"Cannot parse '" + date_with_player_names_and_points + "' into a date with player names and numbers of points. Expected a format such as '2020-03-15 : Alice 10 , Bob 8 , Claire 7 , David 6'."};
    const std::vector<std::string> date_and_the_rest{split(remove_whitespace(date_with_player_names_and_points), ':')};
    if (date_and_the_rest.size() != 2) {
      error(error_message);
    }
    date_ = {date_and_the_rest[0]};
    const std::vector<std::string> player_names_and_points{split(remove_whitespace(date_and_the_rest[1]), ',')};
    if (player_names_and_points.empty()) {
      error(error_message);
    }
    for (const std::string& player_name_and_points : player_names_and_points) {
      player_names_and_points_.emplace(player_name_and_points);
    }
    initialize_player_names();
  }

  constexpr const Date& date() const noexcept {
    return date_;
  }

  constexpr const std::set<PlayerNameAndPoints, sort_by_descending_points>& player_names_and_points() const noexcept {
    return player_names_and_points_;
  }

  constexpr const std::set<std::string>& player_names() const noexcept {
    return player_names_;
  }

  std::string print() const noexcept {
    std::string text{date_.print() + " : "};
    std::size_t counter{0};
    for (const PlayerNameAndPoints& player_name_and_points : player_names_and_points_) {
      text.append(player_name_and_points.print());
      if (counter + 1 < player_names_and_points_.size()) {
        text.append(" , ");
      }
      ++counter;
    }
    return text;
  }

protected:

  Date date_;

  std::set<PlayerNameAndPoints, sort_by_descending_points> player_names_and_points_;

  std::set<std::string> player_names_;

  void initialize_player_names() noexcept {
    for (const PlayerNameAndPoints& player_name_and_points : player_names_and_points_) {
      player_names_.insert(player_name_and_points.player_name());
    }
  }

};

class sort_by_ascending_date {

public:

  bool operator()(const Game& game_1, const Game& game_2) const noexcept {
    return game_1.date() < game_2.date();
  }

};

} // namespace CatanLeaderboard
