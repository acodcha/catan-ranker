#pragma once

#include "Date.hpp"
#include "PlayerNameAndPoints.hpp"

namespace CatanLeaderboard {

class Game {

public:

  Game() noexcept {}

  Game(const Date& date, const std::set<PlayerNameAndPoints, PlayerNameAndPoints::sort_by_descending_points>& player_names_and_points) noexcept : date_(date), player_names_and_points_(player_names_and_points) {
    initialize_player_names();
  }

  /// \brief Constructor that takes a string containing a date and a list of player names and numbers of points such as "2020-03-15 : Alice 10 , Bob 8 , Claire 7 , David 6".
  Game(const std::string& date_with_player_names_and_points) {
    const std::string initialization_error_message{"Cannot parse '" + date_with_player_names_and_points + "' into a date with player names and numbers of points. Expected a format such as '2020-03-15 : Alice 10 , Bob 8 , Claire 7 , David 5'."};
    const std::vector<std::string> date_and_the_rest{split_date_from_the_rest(date_with_player_names_and_points, initialization_error_message)};
    date_ = {date_and_the_rest[0]};
    initialize_player_names_and_points(date_and_the_rest[1], initialization_error_message);
    initialize_player_names();
  }

  constexpr const Date& date() const noexcept {
    return date_;
  }

  constexpr const std::set<PlayerNameAndPoints, PlayerNameAndPoints::sort_by_descending_points>& player_names_and_points() const noexcept {
    return player_names_and_points_;
  }

  constexpr const std::set<std::string>& player_names() const noexcept {
    return player_names_;
  }

  bool participant(const std::string& player_name) const noexcept {
    if (player_names_.find(player_name) != player_names_.cend()) {
      return true;
    } else {
      return false;
    }
  }

  std::optional<uint_least8_t> points(const std::string& player_name) const noexcept {
    for (const PlayerNameAndPoints& player_name_and_points : player_names_and_points_) {
      if (player_name_and_points.player_name() == player_name) {
        return player_name_and_points.points();
      }
    }
    std::optional<uint_least8_t> no_data;
    return no_data;
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

  struct sort_by_ascending_date {
    bool operator()(const Game& game_1, const Game& game_2) const noexcept {
      return game_1.date() < game_2.date();
    }
  };

protected:

  Date date_;

  std::set<PlayerNameAndPoints, PlayerNameAndPoints::sort_by_descending_points> player_names_and_points_;

  std::set<std::string> player_names_;

  std::vector<std::string> split_date_from_the_rest(const std::string& date_with_player_names_and_points, const std::string& initialization_error_message) const {
    const std::vector<std::string> date_and_the_rest{split(remove_whitespace(date_with_player_names_and_points), ':')};
    if (date_and_the_rest.size() != 2) {
      error(initialization_error_message);
    }
    return date_and_the_rest;
  }

  void initialize_player_names_and_points(const std::string& player_names_and_points, const std::string& initialization_error_message) {
    const std::vector<std::string> player_names_and_points_vector{split(remove_whitespace(player_names_and_points), ',')};
    if (player_names_and_points_vector.empty()) {
      error(initialization_error_message);
    }
    for (const std::string& player_name_and_points : player_names_and_points_vector) {
      player_names_and_points_.emplace(player_name_and_points);
    }
  }

  void initialize_player_names() noexcept {
    for (const PlayerNameAndPoints& player_name_and_points : player_names_and_points_) {
      player_names_.insert(player_name_and_points.player_name());
    }
  }

};

} // namespace CatanLeaderboard
