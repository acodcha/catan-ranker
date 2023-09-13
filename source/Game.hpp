#pragma once

#include "Date.hpp"
#include "Place.hpp"
#include "PlayerName.hpp"
#include "Points.hpp"

namespace CatanRanker {

class Game {
public:
  Game() noexcept {}

  /// \brief Constructor that takes a string containing a date and a list of
  /// player names and numbers of points such as "2020-03-15 : Alice 10 , Bob 8
  /// , Claire 7 , David 6".
  Game(const std::string&
           date_with_winning_points_with_player_names_and_points) {
    const std::string initialization_error_message{
      "Cannot parse '" + date_with_winning_points_with_player_names_and_points +
      "' into a date with player names and numbers of points. Expected a format such as '2020-03-15 : Alice 10, Bob 8, Claire 7, David 5'."};
    const std::vector<std::string> split_text{
        split_date_from_winning_points_from_player_names_and_points(
            date_with_winning_points_with_player_names_and_points,
            initialization_error_message)};
    date_ = {split_text[0]};
    initialize_winning_points(split_text[1], initialization_error_message);
    initialize_player_names_and_points(
        split_text[2], initialization_error_message);
    initialize_player_names_and_places();
    check_number_of_players(
        date_with_winning_points_with_player_names_and_points);
  }

  constexpr const Date& date() const noexcept {
    return date_;
  }

  constexpr const Points& winning_points() const noexcept {
    return winning_points_;
  }

  bool participant(const PlayerName& player_name) const noexcept {
    if (player_names_.find(player_name) != player_names_.cend()) {
      return true;
    } else {
      return false;
    }
  }

  /// \brief Ignores points in excess of the winning points and scales the
  /// points as if the game were a 10-point game.
  std::optional<double> adjusted_points(
      const PlayerName& player_name) const noexcept {
    const std::optional<Points> raw_points{points(player_name)};
    if (raw_points.has_value()) {
      const Points limited_points{
          std::min(raw_points.value(), winning_points_)};
      return static_cast<double>(limited_points.value())
             / static_cast<double>(winning_points_.value()) * 10.0;
    } else {
      std::optional<double> no_data;
      return no_data;
    }
  }

  std::optional<Points> points(const PlayerName& player_name) const noexcept {
    const std::map<PlayerName, Points, PlayerName::sort>::const_iterator
        element{player_names_to_points_.find(player_name)};
    if (element != player_names_to_points_.cend()) {
      return element->second;
    } else {
      std::optional<Points> no_data;
      return no_data;
    }
  }

  std::optional<Place> place(const PlayerName& player_name) const noexcept {
    const std::map<PlayerName, Place, PlayerName::sort>::const_iterator element{
        player_names_to_places_.find(player_name)};
    if (element != player_names_to_places_.cend()) {
      return element->second;
    } else {
      std::optional<Place> no_data;
      return no_data;
    }
  }

  int8_t number_of_players() const noexcept {
    return static_cast<int8_t>(player_names_.size());
  }

  GameCategory category() const noexcept {
    return game_category(number_of_players());
  }

  const std::set<PlayerName, PlayerName::sort>& player_names() const noexcept {
    return player_names_;
  }

  std::set<PlayerName, PlayerName::sort> player_names(
      const Points& points) const noexcept {
    const auto range{points_to_player_names_.equal_range(points)};
    std::set<PlayerName, PlayerName::sort> data;
    for (auto element = range.first; element != range.second; ++element) {
      data.insert(element->second);
    }
    return data;
  }

  std::set<PlayerName, PlayerName::sort> player_names(
      const Place& place) const noexcept {
    const auto range{places_to_player_names_.equal_range(place)};
    std::set<PlayerName, PlayerName::sort> data;
    for (auto element = range.first; element != range.second; ++element) {
      data.insert(element->second);
    }
    return data;
  }

  /// \brief Game index. Starts at 0.
  constexpr int64_t index() const noexcept {
    return index_;
  }

  /// \brief Game number, such as 5th game globally. Starts at 1. Equals the
  /// game index plus one.
  constexpr int64_t number() const noexcept {
    return index_ + 1;
  }

  /// \brief Game index within its game category. Starts at 0.
  constexpr int64_t category_index() const noexcept {
    return category_index_;
  }

  /// \brief Game number within its game category, such as 5th game in the 5-6
  /// player game category. Starts at 1. Equals the game category index plus
  /// one.
  constexpr int64_t category_number() const noexcept {
    return category_index_ + 1;
  }

  /// \brief The games are not necessarily sorted when first constructed, so the
  /// index is set later by the Games class.
  void set_indices(const int64_t index, const int64_t category_index) noexcept {
    index_ = index;
    category_index_ = category_index;
  }

  std::string print_results() const noexcept {
    std::string text;
    std::size_t counter{0};
    for (const std::pair<const Place, PlayerName>& place_and_player_name :
         places_to_player_names_) {
      text += place_and_player_name.first.print() + " "
              + place_and_player_name.second.value() + " "
              + player_names_to_points_.find(place_and_player_name.second)
                    ->second.print();
      if (counter + 1 < places_to_player_names_.size()) {
        text += " , ";
      }
      ++counter;
    }
    return text;
  }

  std::string print() const noexcept {
    return date_.print() + " : " + winning_points_.print() + " : "
           + print_results();
  }

  struct sort {
    bool operator()(const Game& game_1, const Game& game_2) const noexcept {
      return game_1.date() < game_2.date();
    }
  };

  struct const_iterator
    : public std::set<PlayerName, PlayerName::sort>::const_iterator {
    const_iterator(
        const std::set<PlayerName, PlayerName::sort>::const_iterator i) noexcept
      : std::set<PlayerName, PlayerName::sort>::const_iterator(i) {}
  };

  bool empty() const noexcept {
    return player_names_.empty();
  }

  std::size_t size() const noexcept {
    return player_names_.size();
  }

  const_iterator cbegin() const noexcept {
    return const_iterator(player_names_.cbegin());
  }

  const_iterator begin() const noexcept {
    return const_iterator(player_names_.cbegin());
  }

  const_iterator cend() const noexcept {
    return const_iterator(player_names_.cend());
  }

  const_iterator end() const noexcept {
    return const_iterator(player_names_.cend());
  }

private:
  Date date_;

  Points winning_points_{10};

  std::set<PlayerName, PlayerName::sort> player_names_;

  std::map<PlayerName, Points, PlayerName::sort> player_names_to_points_;

  std::multimap<Points, PlayerName, Points::sort> points_to_player_names_;

  std::map<PlayerName, Place, PlayerName::sort> player_names_to_places_;

  std::multimap<Place, PlayerName, Place::sort> places_to_player_names_;

  /// \brief Game index.
  int64_t index_{0};

  /// \brief Game index within its game category.
  int64_t category_index_{0};

  std::vector<std::string>
  split_date_from_winning_points_from_player_names_and_points(
      const std::string& date_with_winning_points_with_player_names_and_points,
      const std::string& initialization_error_message) const {
    const std::vector<std::string> split_text{split(
        remove_whitespace(
            date_with_winning_points_with_player_names_and_points),
        ':')};
    if (split_text.size() != 3) {
      error(initialization_error_message);
    }
    return split_text;
  }

  void initialize_winning_points(
      const std::string& winning_points,
      const std::string& initialization_error_message) {
    const std::optional<int64_t> optional_winning_points{
        string_to_integer_number(winning_points)};
    if (optional_winning_points.has_value()
        && optional_winning_points.value() > MinimumPoints.value()
        && optional_winning_points.value() < MaximumPoints.value()) {
      winning_points_ = {optional_winning_points.value()};
    } else {
      error(initialization_error_message);
    }
  }

  void initialize_player_names_and_points(
      const std::string& player_names_and_points,
      const std::string& initialization_error_message) {
    const std::vector<std::string> player_names_and_points_vector{
        split(remove_whitespace(player_names_and_points), ',')};
    if (player_names_and_points_vector.empty()) {
      error(initialization_error_message);
    }
    for (const std::string& player_name_and_points :
         player_names_and_points_vector) {
      std::string player_name_string;
      std::string points_string;
      bool special_first_place{false};
      for (const char character : player_name_and_points) {
        if (character == '*') {
          special_first_place = true;
        } else if (isdigit(character)) {
          points_string += character;
        } else {
          player_name_string += character;
        }
      }
      if (player_name_string.empty()) {
        error(initialization_error_message);
      }
      const PlayerName player_name{player_name_string};
      const std::optional<int64_t> points_optional_number{
          string_to_integer_number(points_string)};
      if (!points_optional_number.has_value()
          || (points_optional_number.has_value()
              && (points_optional_number.value() < MinimumPoints.value()
                  || points_optional_number.value() > MaximumPoints.value()))) {
        error(initialization_error_message);
      }
      const Points points{points_optional_number.value()};
      const std::pair<std::set<PlayerName>::iterator, bool>
          player_names_insert_outcome{player_names_.insert(player_name)};
      if (!player_names_insert_outcome.second) {
        error(initialization_error_message);
      }
      const std::pair<std::map<PlayerName, Points>::iterator, bool>
          player_names_to_points_outcome{
              player_names_to_points_.emplace(player_name, points)};
      if (!player_names_to_points_outcome.second) {
        error(initialization_error_message);
      }
      points_to_player_names_.emplace(points, player_name);
      if (special_first_place) {
        player_names_to_places_.insert({player_name, {1}});
        places_to_player_names_.insert({{1}, player_name});
      }
    }
  }

  void initialize_player_names_and_places() noexcept {
    Points latest_points{MaximumPoints + 1};
    Place latest_place{0};
    PlayerName special_first_place_player;
    const std::multimap<Place, PlayerName, Place::sort>::const_iterator
        found_special_first_place{places_to_player_names_.find({1})};
    if (found_special_first_place != places_to_player_names_.cend()) {
      latest_place = found_special_first_place->first;
      special_first_place_player = found_special_first_place->second;
    }
    for (const auto& element : points_to_player_names_) {
      if (special_first_place_player != element.second) {
        if (latest_points > element.first) {
          latest_points = element.first;
          ++latest_place;
        }
        player_names_to_places_.emplace(element.second, latest_place);
        places_to_player_names_.emplace(latest_place, element.second);
      }
    }
  }

  void check_number_of_players(
      const std::string& date_with_player_names_and_points) const {
    if (player_names_.size() < 3 || player_names_.size() > 8) {
      error("The game '" + date_with_player_names_and_points +
      "' has an invalid number of players. A Catan game must have 3 to 8 players.");
    }
  }
};

}  // namespace CatanRanker
