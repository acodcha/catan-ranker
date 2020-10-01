#pragma once

#include "DataTableFileWriter.hpp"
#include "GnuplotPlayerAveragePointsFileWriter.hpp"
#include "GnuplotPlayerPlacePercentageFileWriter.hpp"
#include "ResultsPlayerSummaryFileWriter.hpp"
#include "ResultsSummaryFileWriter.hpp"

namespace CatanLeaderboardGenerator {

/// \brief Results files writer.
class Results {

public:

  Results(const std::experimental::filesystem::path& directory, const Games& games, const Players& players) {
    if (!directory.empty()) {
      std::experimental::filesystem::create_directory(directory);
      if (std::experimental::filesystem::exists(directory) && std::experimental::filesystem::is_directory(directory)) {
        ResultsSummaryFileWriter{directory / std::experimental::filesystem::path{"README.md"}, games, players};
        create_player_directories(directory, players);
        write_player_summary_files(directory, games, players);
        write_player_data_table_files(directory, players);
        write_player_gnuplot_files(directory, players);
        generate_player_plots(directory, players);
      } else {
        error("Could not create the directory '" + directory.string() + "'.");
      }
    }
  }

protected:

  void create_player_directories(const std::experimental::filesystem::path& directory, const Players& players) {
    for (const Player& player : players) {
      std::experimental::filesystem::create_directory(player_directory(directory, player));
      if (!std::experimental::filesystem::exists(player_directory(directory, player)) || !std::experimental::filesystem::is_directory(player_directory(directory, player))) {
        error("Could not create the directory '" + player_directory(directory, player).string() + "'.");
      }
    }
  }

  void write_player_summary_files(const std::experimental::filesystem::path& directory, const Games& games, const Players& players) noexcept {
    for (const Player& player : players) {
      ResultsPlayerSummaryFileWriter{player_directory(directory, player) / std::experimental::filesystem::path{"README.md"}, games, player};
    }
    message("Wrote the player summary files.");
  }

  void write_player_data_table_files(const std::experimental::filesystem::path& directory, const Players& players) noexcept {
    for (const Player& player : players) {
      if (!player[GameCategory::AnyNumberOfPlayers].empty()) {
        DataTableFileWriter{
          player_data_table_file_path(directory, player, GameCategory::AnyNumberOfPlayers),
          player_table(player, GameCategory::AnyNumberOfPlayers)
        };
      }
      if (!player[GameCategory::ThreeToFourPlayers].empty()) {
        DataTableFileWriter{
          player_data_table_file_path(directory, player, GameCategory::ThreeToFourPlayers),
          player_table(player, GameCategory::ThreeToFourPlayers)
        };
      }
      if (!player[GameCategory::FiveToSixPlayers].empty()) {
        DataTableFileWriter{
          player_data_table_file_path(directory, player, GameCategory::FiveToSixPlayers),
          player_table(player, GameCategory::FiveToSixPlayers)
        };
      }
      if (!player[GameCategory::SevenToEightPlayers].empty()) {
        DataTableFileWriter{
          player_data_table_file_path(directory, player, GameCategory::SevenToEightPlayers),
          player_table(player, GameCategory::SevenToEightPlayers)
        };
      }
    }
    message("Wrote the data table files for each player.");
  }

  void write_player_gnuplot_files(const std::experimental::filesystem::path& directory, const Players& players) noexcept {
    for (const Player& player : players) {
      std::map<GameCategory, std::experimental::filesystem::path> data_paths;
      for (const GameCategory game_category : enumerations<GameCategory>) {
        if (!player[game_category].empty()) {
          data_paths.insert({game_category, player_data_table_file_path(directory, player, game_category)});
        }
      }
      if (!data_paths.empty()) {
        GnuplotPlayerAveragePointsVsGameNumberFileWriter{gnuplot_player_average_points_vs_game_number_file_path(directory, player), data_paths};
        GnuplotPlayerAveragePointsVsDateFileWriter{gnuplot_player_average_points_vs_date_file_path(directory, player), data_paths};
      }
      write_gnuplot_place_percentage_files(directory, player, GameCategory::AnyNumberOfPlayers);
      write_gnuplot_place_percentage_files(directory, player, GameCategory::ThreeToFourPlayers);
      write_gnuplot_place_percentage_files(directory, player, GameCategory::FiveToSixPlayers);
      write_gnuplot_place_percentage_files(directory, player, GameCategory::SevenToEightPlayers);
    }
    message("Wrote the Gnuplot files for each player.");
  }

  void write_gnuplot_place_percentage_files(const std::experimental::filesystem::path& directory, const Player& player, const GameCategory game_category) noexcept {
    if (!player[game_category].empty()) {
      GnuplotPlayerPlacePercentageVsGameNumberFileWriter{
        gnuplot_player_place_percentage_vs_game_number_file_path(directory, player, game_category),
        player_data_table_file_path(directory, player, game_category),
        game_category
      };
      GnuplotPlayerPlacePercentageVsDateFileWriter{
        gnuplot_player_place_percentage_vs_date_file_path(directory, player, game_category),
        player_data_table_file_path(directory, player, game_category),
        game_category
      };
    }
  }

  void generate_player_plots(const std::experimental::filesystem::path& directory, const Players& players) const {
    message("Generating the plots for each player...");
    for (const Player& player : players) {
      if (!player[GameCategory::AnyNumberOfPlayers].empty()) {
        run_command("gnuplot " + gnuplot_player_average_points_vs_game_number_file_path(directory, player).string());
        run_command("gnuplot " + gnuplot_player_average_points_vs_date_file_path(directory, player).string());
      }
      for (const GameCategory game_category : enumerations<GameCategory>) {
        if (!player[game_category].empty()) {
          run_command("gnuplot " + gnuplot_player_place_percentage_vs_game_number_file_path(directory, player, game_category).string());
          run_command("gnuplot " + gnuplot_player_place_percentage_vs_date_file_path(directory, player, game_category).string());
        }
      }
    }
    message("Generated the plots for each player.");
  }

  Table player_table(const Player& player, const GameCategory game_category) const noexcept {
    Column global_number{"Global"};
    Column local_number{"Local"};
    Column date{"Date"};
    Column average_points_per_game{"Points"};
    Column first_place_percentage{"1stPlace"};
    Column second_place_percentage{"2ndPlace"};
    Column third_place_percentage{"3rdPlace"};
    if (!player[game_category].empty()) {
      for (const PlayerProperties& properties : player[game_category]) {
        global_number.add_row(properties.global_game_number());
        local_number.add_row(properties.local_game_number());
        date.add_row(properties.date());
        average_points_per_game.add_row(properties.average_points_per_game(), 7);
        first_place_percentage.add_row(properties.place_percentage({1}), 5);
        second_place_percentage.add_row(properties.place_percentage({2}), 5);
        third_place_percentage.add_row(properties.place_percentage({3}), 5);
      }
    }
    return {{global_number, local_number, date, average_points_per_game, first_place_percentage, second_place_percentage, third_place_percentage}};
  }

  std::string common_file_name(const GameCategory game_category) const noexcept {
    return replace_character(replace_character(lowercase(label(game_category)), ' ', '_'), '-', '_');
  }

  std::experimental::filesystem::path player_directory(const std::experimental::filesystem::path& directory, const Player& player) const noexcept {
    return directory / player.name().value();
  }

  std::experimental::filesystem::path player_data_table_file_path(
    const std::experimental::filesystem::path& directory,
    const Player& player,
    const GameCategory game_category
  ) const noexcept {
    return {player_directory(directory, player) / std::experimental::filesystem::path{"data_" + common_file_name(game_category) + ".dat"}};
  }

  std::experimental::filesystem::path gnuplot_player_average_points_vs_game_number_file_path(
    const std::experimental::filesystem::path& directory,
    const Player& player
  ) const noexcept {
    return {player_directory(directory, player) / std::experimental::filesystem::path{"average_points_vs_game_number.gnuplot"}};
  }

  std::experimental::filesystem::path gnuplot_player_average_points_vs_date_file_path(
    const std::experimental::filesystem::path& directory,
    const Player& player
  ) const noexcept {
    return {player_directory(directory, player) / std::experimental::filesystem::path{"average_points_vs_date.gnuplot"}};
  }

  std::experimental::filesystem::path gnuplot_player_place_percentage_vs_game_number_file_path(
    const std::experimental::filesystem::path& directory,
    const Player& player,
    const GameCategory game_category
  ) const noexcept {
    return {player_directory(directory, player) / std::experimental::filesystem::path{"place_percentage_" + common_file_name(game_category) + "_vs_game_number.gnuplot"}};
  }

  std::experimental::filesystem::path gnuplot_player_place_percentage_vs_date_file_path(
    const std::experimental::filesystem::path& directory,
    const Player& player,
    const GameCategory game_category
  ) const noexcept {
    return {player_directory(directory, player) / std::experimental::filesystem::path{"place_percentage_" + common_file_name(game_category) + "_vs_date.gnuplot"}};
  }

  void run_command(const std::string& command) const {
    const int outcome{std::system(command.c_str())};
    if (outcome != 0) {
      error("Could not run the command: " + command);
    }
  }

};

} // namespace CatanLeaderboardGenerator
