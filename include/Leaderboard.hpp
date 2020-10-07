#pragma once

#include "DataFileWriter.hpp"
#include "LeaderboardMainFileWriter.hpp"
#include "LeaderboardPlayerFileWriter.hpp"
#include "MainAveragePointsGnuplotFileWriter.hpp"
#include "MainEloRatingGnuplotFileWriter.hpp"
#include "MainPlacePercentageGnuplotFileWriter.hpp"
#include "PlayerAveragePointsGnuplotFileWriter.hpp"
#include "PlayerEloRatingGnuplotFileWriter.hpp"
#include "PlayerPlacePercentageGnuplotFileWriter.hpp"

namespace CatanLeaderboardGenerator {

/// \brief Class that writes all leaderboard files given games and players data.
class Leaderboard {

public:

  Leaderboard(const std::experimental::filesystem::path& base_directory, const Games& games, const Players& players) {
    if (!base_directory.empty()) {
      create_directories(base_directory, players);
      write_player_data_files(base_directory, players);
      write_main_gnuplot_files(base_directory, players);
      write_player_gnuplot_files(base_directory, players);
      write_main_leaderboard_file(base_directory, games, players);
      write_player_leaderboard_files(base_directory, games, players);
      generate_main_plots(base_directory);
      generate_player_plots(base_directory, players);
    }
  }

protected:

  void create_directories(const std::experimental::filesystem::path& base_directory, const Players& players) {
    create(base_directory);
    create(base_directory / Path::MainPlotsDirectoryName);
    for (const Player& player : players) {
      create(base_directory / player.name().directory_name());
      create(base_directory / player.name().directory_name() / Path::PlayerDataDirectoryName);
      create(base_directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName);
    }
  }

  void write_player_data_files(const std::experimental::filesystem::path& base_directory, const Players& players) noexcept {
    for (const Player& player : players) {
      for (const GameCategory game_category : GameCategories) {
        if (!player[game_category].empty()) {
          DataFileWriter{
            base_directory / player.name().directory_name() / Path::PlayerDataDirectoryName / Path::player_data_file_name(game_category),
            player_table(player, game_category)
          };
        }
      }
    }
    message("Wrote the data files.");
  }

  void write_main_gnuplot_files(const std::experimental::filesystem::path& base_directory, const Players& players) noexcept {
    for (const GameCategory game_category : GameCategories) {
      std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort> data_paths;
      for (const Player& player : players) {
        if (!player[game_category].empty()) {
          data_paths.insert({
            player.name(),
            base_directory / player.name().directory_name() / Path::PlayerDataDirectoryName / Path::player_data_file_name(game_category)
          });
        }
      }
      if (!data_paths.empty()) {
        MainEloRatingVsGameNumberGnuplotFileWriter{
          base_directory / Path::MainPlotsDirectoryName / Path::main_elo_rating_vs_game_number_file_name(game_category),
          players, data_paths, game_category
        };
        MainAveragePointsVsGameNumberGnuplotFileWriter{
          base_directory / Path::MainPlotsDirectoryName / Path::main_average_points_vs_game_number_file_name(game_category),
          players, data_paths
        };
        for (const Place& place : PlacesFirstSecondThird) {
          MainPlacePercentageVsGameNumberGnuplotFileWriter{
            base_directory / Path::MainPlotsDirectoryName / Path::main_place_percentage_vs_game_number_file_name(game_category, place),
            players, data_paths, game_category, place
          };
        }
      }
    }
    message("Wrote the main Gnuplot files.");
  }

  void write_player_gnuplot_files(const std::experimental::filesystem::path& base_directory, const Players& players) noexcept {
    for (const Player& player : players) {
      std::map<GameCategory, std::experimental::filesystem::path> data_paths;
      for (const GameCategory game_category : GameCategories) {
        if (!player[game_category].empty()) {
          data_paths.insert({
            game_category,
            base_directory / player.name().directory_name() / Path::PlayerDataDirectoryName / Path::player_data_file_name(game_category)
          });
        }
      }
      if (!data_paths.empty()) {
        PlayerEloRatingVsGameNumberGnuplotFileWriter{
          base_directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName / Path::PlayerEloRatingVsGameNumberFileName,
          data_paths, player.peak_elo_rating()
        };
        PlayerAveragePointsVsGameNumberGnuplotFileWriter{
          base_directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName / Path::PlayerAveragePointsVsGameNumberFileName,
          data_paths
        };
      }
      for (const GameCategory game_category : GameCategories) {
        if (!player[game_category].empty()) {
          PlayerPlacePercentageVsGameNumberGnuplotFileWriter{
            base_directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName / Path::player_place_percentage_vs_game_number_file_name(game_category),
            base_directory / player.name().directory_name() / Path::PlayerDataDirectoryName / Path::player_data_file_name(game_category),
            game_category
          };
        }
      }
    }
    message("Wrote the player Gnuplot files.");
  }

  void write_main_leaderboard_file(const std::experimental::filesystem::path& base_directory, const Games& games, const Players& players) noexcept {
    LeaderboardMainFileWriter{base_directory, games, players};
    message("Wrote the main leaderboard Markdown file.");
  }

  void write_player_leaderboard_files(const std::experimental::filesystem::path& base_directory, const Games& games, const Players& players) noexcept {
    for (const Player& player : players) {
      LeaderboardPlayerFileWriter{base_directory, games, player};
    }
    message("Wrote the player leaderboard Markdown files.");
  }

  void generate_main_plots(const std::experimental::filesystem::path& base_directory) const {
    message("Generating the main plots...");
    for (const GameCategory game_category : GameCategories) {
      generate_plot(base_directory / Path::MainPlotsDirectoryName / Path::main_elo_rating_vs_game_number_file_name(game_category));
      generate_plot(base_directory / Path::MainPlotsDirectoryName / Path::main_average_points_vs_game_number_file_name(game_category));
      for (const Place& place : PlacesFirstSecondThird) {
        generate_plot(base_directory / Path::MainPlotsDirectoryName / Path::main_place_percentage_vs_game_number_file_name(game_category, place));
      }
    }
    message("Generated the main plots.");
  }

  void generate_player_plots(const std::experimental::filesystem::path& base_directory, const Players& players) const {
    message("Generating the player plots...");
    for (const Player& player : players) {
      generate_plot(base_directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName / Path::PlayerAveragePointsVsGameNumberFileName);
      generate_plot(base_directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName / Path::PlayerEloRatingVsGameNumberFileName);
      for (const GameCategory game_category : GameCategories) {
        generate_plot(base_directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName / Path::player_place_percentage_vs_game_number_file_name(game_category));
      }
    }
    message("Generated the player plots.");
  }

  /// \brief Generate a plot using Gnuplot. If the path points to a file that does not exist, no plot is generated.
  void generate_plot(const std::experimental::filesystem::path& path) const {
    if (std::experimental::filesystem::exists(path)) {
      const std::string command{"gnuplot " + path.string()};
      const int outcome{std::system(command.c_str())};
      if (outcome != 0) {
        error("Could not run the command: " + command);
      }
    }
  }

  Table player_table(const Player& player, const GameCategory game_category) const noexcept {
    Column game_number{"Game#"};
    Column game_category_game_number{"GameCategory#"};
    Column player_game_number{"PlayerGame#"};
    Column player_game_category_game_number{"PlayerCategoryGame#"};
    Column date{"Date"};
    Column elo_rating{"Rating"};
    Column average_points_per_game{"AvgPoints"};
    Column first_place_percentage{"1stPlace%"};
    Column second_place_percentage{"2ndPlace%"};
    Column third_place_percentage{"3rdPlace%"};
    if (!player[game_category].empty()) {
      for (const PlayerProperties& properties : player[game_category]) {
        game_number.add_row(properties.game_number());
        game_category_game_number.add_row(properties.game_category_game_number());
        player_game_number.add_row(properties.player_game_number());
        player_game_category_game_number.add_row(properties.player_game_category_game_number());
        date.add_row(properties.date());
        elo_rating.add_row(properties.elo_rating());
        average_points_per_game.add_row(properties.average_points_per_game(), 7);
        first_place_percentage.add_row(properties.place_percentage({1}), 5);
        second_place_percentage.add_row(properties.place_percentage({2}), 5);
        third_place_percentage.add_row(properties.place_percentage({3}), 5);
      }
    }
    return {{game_number, game_category_game_number, player_game_number, player_game_category_game_number, date, elo_rating, average_points_per_game, first_place_percentage, second_place_percentage, third_place_percentage}};
  }

};

} // namespace CatanLeaderboardGenerator
