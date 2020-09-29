#pragma once

#include "DataTableFileWriter.hpp"
#include "GnuplotAveragePointsFileWriter.hpp"
#include "ResultsSummaryFileWriter.hpp"

namespace CatanLeaderboardGenerator {

/// \brief Writer for all results files.
class Results {

public:

  Results(const std::experimental::filesystem::path& directory, const Players& players) {
    if (!directory.empty()) {
      std::experimental::filesystem::create_directory(directory);
      if (std::experimental::filesystem::exists(directory) && std::experimental::filesystem::is_directory(directory)) {
        ResultsSummaryFileWriter{directory / "README.md", players};
        create_player_directories(directory, players);
        write_data_table_files(directory, players);
        write_gnuplot_files(directory, players);
        generate_plots(directory, players);
      } else {
        error("Could not create the directory '" + directory.string() + "'.");
      }
    }
  }

protected:

  void create_player_directories(const std::experimental::filesystem::path& directory, const Players& players) {
    for (const Player& player : players) {
      const std::experimental::filesystem::path player_directory{directory / player.name().value()};
      std::experimental::filesystem::create_directory(player_directory);
      if (!std::experimental::filesystem::exists(player_directory) || !std::experimental::filesystem::is_directory(player_directory)) {
        error("Could not create the directory '" + player_directory.string() + "'.");
      }
    }
  }

  void write_data_table_files(const std::experimental::filesystem::path& directory, const Players& players) noexcept {
    for (const Player& player : players) {
      DataTableFileWriter{data_table_file_path(directory, player, GameCategory::AnyNumberOfPlayers), table(player, GameCategory::AnyNumberOfPlayers)};
      DataTableFileWriter{data_table_file_path(directory, player, GameCategory::ThreeToFourPlayers), table(player, GameCategory::ThreeToFourPlayers)};
      DataTableFileWriter{data_table_file_path(directory, player, GameCategory::FiveToSixPlayers), table(player, GameCategory::FiveToSixPlayers)};
      DataTableFileWriter{data_table_file_path(directory, player, GameCategory::SevenToEightPlayers), table(player, GameCategory::SevenToEightPlayers)};
    }
    message("Wrote the data table files for each player.");
  }

  void write_gnuplot_files(const std::experimental::filesystem::path& directory, const Players& players) noexcept {
    for (const Player& player : players) {
      const std::map<GameCategory, std::experimental::filesystem::path> data_paths{
        {GameCategory::AnyNumberOfPlayers, data_table_file_path(directory, player, GameCategory::AnyNumberOfPlayers)},
        {GameCategory::ThreeToFourPlayers, data_table_file_path(directory, player, GameCategory::ThreeToFourPlayers)},
        {GameCategory::FiveToSixPlayers, data_table_file_path(directory, player, GameCategory::FiveToSixPlayers)},
        {GameCategory::SevenToEightPlayers, data_table_file_path(directory, player, GameCategory::SevenToEightPlayers)}
      };
      GnuplotAveragePointsFileWriter{gnuplot_average_points_file_path(directory, player), data_paths};
    }
    message("Wrote the Gnuplot files for each player.");
  }

  void generate_plots(const std::experimental::filesystem::path& directory, const Players& players) const {
    for (const Player& player : players) {
      const std::string average_points_command{"gnuplot " + gnuplot_average_points_file_path(directory, player).string()};
      const int average_points_outcome{std::system(average_points_command.c_str())};
      if (average_points_outcome != 0) {
        error("Could not run the command: " + average_points_command);
      }
    }
    message("Generated the plots for each player.");
  }

  Table table(const Player& player, const GameCategory game_category) const noexcept {
    Column number{""};
    Column date{"Date"};
    Column average_points_per_game{"AveragePoints"};
    Column first_place_percentage{"1stPlace%"};
    Column second_place_percentage{"2ndPlace%"};
    Column third_place_percentage{"3rdPlace%"};
    uint_least64_t counter{0};
    if (!player[game_category].empty()) {
      for (const PlayerProperties& properties : player[game_category]) {
        ++counter;
        number.add_row(counter);
        date.add_row(properties.date());
        average_points_per_game.add_row(properties.average_points_per_game(), 7);
        first_place_percentage.add_row(properties.place_percentage({1}), 5);
        second_place_percentage.add_row(properties.place_percentage({2}), 5);
        third_place_percentage.add_row(properties.place_percentage({3}), 5);
      }
    }
    return {{number, date, average_points_per_game, first_place_percentage, second_place_percentage, third_place_percentage}};
  }

  std::experimental::filesystem::path data_table_file_path(const std::experimental::filesystem::path& directory, const Player& player, const GameCategory game_category) const noexcept {
    return {directory / player.name().value() / std::experimental::filesystem::path{"data_" + replace_character(replace_character(lowercase(label(game_category)), ' ', '_'), '-', '_') + ".dat"}};
  }

  std::experimental::filesystem::path gnuplot_average_points_file_path(const std::experimental::filesystem::path& directory, const Player& player) const noexcept {
    return {directory / player.name().value() / std::experimental::filesystem::path{"average_points.gnuplot"}};
  }

};

} // namespace CatanLeaderboardGenerator
