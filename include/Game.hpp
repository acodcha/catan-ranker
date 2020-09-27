#pragma once

#include "Date.hpp"
#include "Place.hpp"
#include "PlayerName.hpp"
#include "Points.hpp"

namespace CatanLeaderboardGenerator {

class Game {

public:

  Game() noexcept {}

  /// \brief Constructor that takes a string containing a date and a list of player names and numbers of points such as "2020-03-15 : Alice 10 , Bob 8 , Claire 7 , David 6".
  Game(const std::string& date_with_player_names_and_points) {
    const std::string initialization_error_message{"Cannot parse '" + date_with_player_names_and_points + "' into a date with player names and numbers of points. Expected a format such as '2020-03-15 : Alice 10 , Bob 8 , Claire 7 , David 5'."};
    const std::vector<std::string> date_and_the_rest{split_date_from_the_rest(date_with_player_names_and_points, initialization_error_message)};
    date_ = {date_and_the_rest[0]};
    initialize_player_names_and_points(date_and_the_rest[1], initialization_error_message);
    initialize_player_names_and_places();
    check_number_of_players(date_with_player_names_and_points);
  }

  constexpr const Date& date() const noexcept {
    return date_;
  }

  bool participant(const PlayerName& player_name) const noexcept {
    if (player_names_.find(player_name) != player_names_.cend()) {
      return true;
    } else {
      return false;
    }
  }

  std::optional<Points> points(const PlayerName& player_name) const noexcept {
    const std::map<PlayerName, Points, PlayerName::sort>::const_iterator element{player_names_to_points_.find(player_name)};
    if (element != player_names_to_points_.cend()) {
      return element->second;
    } else {
      std::optional<Points> no_data;
      return no_data;
    }
  }

  std::optional<Place> place(const PlayerName& player_name) const noexcept {
    const std::map<PlayerName, Place, PlayerName::sort>::const_iterator element{player_names_to_places_.find(player_name)};
    if (element != player_names_to_places_.cend()) {
      return element->second;
    } else {
      std::optional<Place> no_data;
      return no_data;
    }
  }

  uint_least8_t number_of_players() const noexcept {
    return (uint_least8_t)player_names_.size();
  }

  const std::set<PlayerName, PlayerName::sort>& player_names() const noexcept {
    return player_names_;
  }

  std::set<PlayerName, PlayerName::sort> player_names(const Points& points) const noexcept {
    const auto range{points_to_player_names_.equal_range(points)};
    std::set<PlayerName, PlayerName::sort> data;
    for (auto element = range.first; element != range.second; ++element) {
      data.insert(element->second);
    }
    return data;
  }

  std::set<PlayerName, PlayerName::sort> player_names(const Place& place) const noexcept {
    const auto range{places_to_player_names_.equal_range(place)};
    std::set<PlayerName, PlayerName::sort> data;
    for (auto element = range.first; element != range.second; ++element) {
      data.insert(element->second);
    }
    return data;
  }

  std::string print() const noexcept {
    std::string text{date_.print() + " : "};
    std::size_t counter{0};
    for (const std::pair<Points, PlayerName>& datum : points_to_player_names_) {
      text += player_names_to_places_.find(datum.second)->second.print() + " " + datum.second.value() + " " + datum.first.print();
      if (counter + 1 < points_to_player_names_.size()) {
        text += " , ";
      }
      ++counter;
    }
    return text;
  }

  struct sort {
    bool operator()(const Game& game_1, const Game& game_2) const noexcept {
      return game_1.date() < game_2.date();
    }
  };

protected:

  Date date_;

  std::set<PlayerName, PlayerName::sort> player_names_;

  std::map<PlayerName, Points, PlayerName::sort> player_names_to_points_;

  std::multimap<Points, PlayerName, Points::sort> points_to_player_names_;

  std::map<PlayerName, Place, PlayerName::sort> player_names_to_places_;

  std::multimap<Place, PlayerName, Place::sort> places_to_player_names_;

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
      std::string player_name_string;
      std::string points_string;
      for (const char character : player_name_and_points) {
        if (isdigit(character) || character == '.') {
          points_string += character;
        } else {
          player_name_string += character;
        }
      }
      if (player_name_string.empty()) {
        error(initialization_error_message);
      }
      const PlayerName player_name{player_name_string};
      const std::optional<double> points_optional_number{string_to_real_number(points_string)};
      if (!points_optional_number.has_value()) {
        error(initialization_error_message);
      }
      const Points points{points_optional_number.value()};
      const std::pair<std::set<PlayerName>::iterator, bool> player_names_insert_outcome{player_names_.insert(player_name)};
      if (!player_names_insert_outcome.second) {
        error(initialization_error_message);
      }
      const std::pair<std::map<PlayerName, Points>::iterator, bool> player_names_to_points_outcome{player_names_to_points_.emplace(player_name, points)};
      if (!player_names_to_points_outcome.second) {
        error(initialization_error_message);
      }
      points_to_player_names_.emplace(points, player_name);
    }
  }

  void initialize_player_names_and_places() noexcept {
    Points latest_points{100.0};
    Place latest_place{0};
    for (const auto& element : points_to_player_names_) {
      if (latest_points > element.first) {
        latest_points = element.first;
        ++latest_place;
      }
      player_names_to_places_.emplace(element.second, latest_place);
      places_to_player_names_.emplace(latest_place, element.second);
    }
  }

  void check_number_of_players(const std::string& date_with_player_names_and_points) const {
    if (player_names_.size() < 3 || player_names_.size() > 8) {
      error("The game '" + date_with_player_names_and_points + "' has an invalid number of players. A Catan game must have 3 to 8 players.");
    }
  }

};

} // namespace CatanLeaderboardGenerator
