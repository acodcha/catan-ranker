#pragma once

#include "DataTableFileWriter.hpp"
#include "GnuplotGlobalAveragePointsFileWriter.hpp"
#include "GnuplotGlobalPlacePercentageFileWriter.hpp"
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
      create_directories(directory, players);
      write_player_data_table_files(directory, players);
      write_global_gnuplot_files(directory, players);
      write_player_gnuplot_files(directory, players);
      ResultsSummaryFileWriter{directory / std::experimental::filesystem::path{"README.md"}, games, players};
      write_player_summary_files(directory, games, players);
      generate_global_plots(directory);
      generate_player_plots(directory, players);
    }
  }

protected:

  void create_directories(const std::experimental::filesystem::path& directory, const Players& players) {
    create(directory);
    create(directory / Path::GlobalPlotsDirectoryName);
    for (const Player& player : players) {
      create(directory / player.name().directory_name());
      create(directory / player.name().directory_name() / Path::PlayerDataDirectoryName);
      create(directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName);
    }
  }

  void write_player_data_table_files(const std::experimental::filesystem::path& directory, const Players& players) noexcept {
    for (const Player& player : players) {
      for (const GameCategory game_category : GameCategories) {
        if (!player[game_category].empty()) {
          DataTableFileWriter{
            directory / player.name().directory_name() / Path::PlayerDataDirectoryName / Path::player_data_table_file_name(game_category),
            player_table(player, game_category)
          };
        }
      }
    }
    message("Wrote the data table files for each player.");
  }

  void write_global_gnuplot_files(const std::experimental::filesystem::path& directory, const Players& players) noexcept {
    for (const GameCategory game_category : GameCategories) {
      std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort> data_paths;
      for (const Player& player : players) {
        if (!player[game_category].empty()) {
          data_paths.insert({
            player.name(),
            directory / player.name().directory_name() / Path::PlayerDataDirectoryName / Path::player_data_table_file_name(game_category)
          });
        }
      }
      if (!data_paths.empty()) {
        GnuplotGlobalAveragePointsVsGameNumberFileWriter{
          directory / Path::GlobalPlotsDirectoryName / Path::gnuplot_global_average_points_vs_game_number_file_name(game_category),
          data_paths
        };
        GnuplotGlobalAveragePointsVsDateFileWriter{
          directory / Path::GlobalPlotsDirectoryName / Path::gnuplot_global_average_points_vs_date_file_name(game_category),
          data_paths
        };
        GnuplotGlobalPlacePercentageVsGameNumberFileWriter{
          directory / Path::GlobalPlotsDirectoryName / Path::gnuplot_global_place_percentage_vs_game_number_file_name(game_category, {1}),
          data_paths, game_category, {1}
        };
        GnuplotGlobalPlacePercentageVsGameNumberFileWriter{
          directory / Path::GlobalPlotsDirectoryName / Path::gnuplot_global_place_percentage_vs_game_number_file_name(game_category, {2}),
          data_paths, game_category, {2}
        };
        GnuplotGlobalPlacePercentageVsGameNumberFileWriter{
          directory / Path::GlobalPlotsDirectoryName / Path::gnuplot_global_place_percentage_vs_game_number_file_name(game_category, {3}),
          data_paths, game_category, {3}
        };
        GnuplotGlobalPlacePercentageVsDateFileWriter{
          directory / Path::GlobalPlotsDirectoryName / Path::gnuplot_global_place_percentage_vs_date_file_name(game_category, {1}),
          data_paths, game_category, {1}
        };
        GnuplotGlobalPlacePercentageVsDateFileWriter{
          directory / Path::GlobalPlotsDirectoryName / Path::gnuplot_global_place_percentage_vs_date_file_name(game_category, {2}),
          data_paths, game_category, {2}
        };
        GnuplotGlobalPlacePercentageVsDateFileWriter{
          directory / Path::GlobalPlotsDirectoryName / Path::gnuplot_global_place_percentage_vs_date_file_name(game_category, {3}),
          data_paths, game_category, {3}
        };
      }
    }
  }

  void write_player_gnuplot_files(const std::experimental::filesystem::path& directory, const Players& players) noexcept {
    for (const Player& player : players) {
      std::map<GameCategory, std::experimental::filesystem::path> data_paths;
      for (const GameCategory game_category : GameCategories) {
        if (!player[game_category].empty()) {
          data_paths.insert({
            game_category,
            directory / player.name().directory_name() / Path::PlayerDataDirectoryName / Path::player_data_table_file_name(game_category)
          });
        }
      }
      if (!data_paths.empty()) {
        GnuplotPlayerAveragePointsVsGameNumberFileWriter{
          directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName / Path::PlayerAveragePointsVsGameNumberFileName,
          data_paths
        };
        GnuplotPlayerAveragePointsVsDateFileWriter{
          directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName / Path::PlayerAveragePointsVsDateFileName,
          data_paths
        };
      }
      write_player_gnuplot_place_percentage_files(directory, player, GameCategory::AnyNumberOfPlayers);
      write_player_gnuplot_place_percentage_files(directory, player, GameCategory::ThreeToFourPlayers);
      write_player_gnuplot_place_percentage_files(directory, player, GameCategory::FiveToSixPlayers);
      write_player_gnuplot_place_percentage_files(directory, player, GameCategory::SevenToEightPlayers);
    }
    message("Wrote the Gnuplot files for each player.");
  }

  void write_player_gnuplot_place_percentage_files(const std::experimental::filesystem::path& directory, const Player& player, const GameCategory game_category) noexcept {
    if (!player[game_category].empty()) {
      GnuplotPlayerPlacePercentageVsGameNumberFileWriter{
        directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName / Path::gnuplot_player_place_percentage_vs_game_number_file_name(game_category),
        directory / player.name().directory_name() / Path::PlayerDataDirectoryName / Path::player_data_table_file_name(game_category),
        game_category
      };
      GnuplotPlayerPlacePercentageVsDateFileWriter{
        directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName / Path::gnuplot_player_place_percentage_vs_date_file_name(game_category),
        directory / player.name().directory_name() / Path::PlayerDataDirectoryName / Path::player_data_table_file_name(game_category),
        game_category
      };
    }
  }

  void write_player_summary_files(const std::experimental::filesystem::path& directory, const Games& games, const Players& players) noexcept {
    for (const Player& player : players) {
      ResultsPlayerSummaryFileWriter{directory / player.name().directory_name() / Path::SummaryFileName, games, player};
    }
    message("Wrote the player summary files.");
  }

  void generate_global_plots(const std::experimental::filesystem::path& directory) const {
    message("Generating the global plots...");
    for (const GameCategory game_category : GameCategories) {
      const std::experimental::filesystem::path average_points_vs_game_number_path{
        directory / Path::GlobalPlotsDirectoryName / Path::gnuplot_global_average_points_vs_game_number_file_name(game_category)
      };
      if (std::experimental::filesystem::exists(average_points_vs_game_number_path)) {
        run_command("gnuplot " + average_points_vs_game_number_path.string());
      }
      const std::experimental::filesystem::path average_points_vs_date_path{
        directory / Path::GlobalPlotsDirectoryName / Path::gnuplot_global_average_points_vs_date_file_name(game_category)
      };
      if (std::experimental::filesystem::exists(average_points_vs_date_path)) {
        run_command("gnuplot " + average_points_vs_date_path.string());
      }
      for (const Place& place : PlacesFirstSecondThird) {
        const std::experimental::filesystem::path place_percentage_vs_game_number_path{
          directory / Path::GlobalPlotsDirectoryName / Path::gnuplot_global_place_percentage_vs_game_number_file_name(game_category, place)
        };
        run_command("gnuplot " + place_percentage_vs_game_number_path.string());
        const std::experimental::filesystem::path place_percentage_vs_date_path{
          directory / Path::GlobalPlotsDirectoryName / Path::gnuplot_global_place_percentage_vs_date_file_name(game_category, place)
        };
        run_command("gnuplot " + place_percentage_vs_date_path.string());
      }
    }
    message("Generated the global plots.");
  }

  void generate_player_plots(const std::experimental::filesystem::path& directory, const Players& players) const {
    message("Generating the plots for each player...");
    for (const Player& player : players) {
      if (!player[GameCategory::AnyNumberOfPlayers].empty()) {
        const std::experimental::filesystem::path average_points_vs_game_number_path{
          directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName / Path::PlayerAveragePointsVsGameNumberFileName
        };
        run_command("gnuplot " + average_points_vs_game_number_path.string());
        const std::experimental::filesystem::path average_points_vs_date_path{
          directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName / Path::PlayerAveragePointsVsDateFileName
        };
        run_command("gnuplot " + average_points_vs_date_path.string());
      }
      for (const GameCategory game_category : GameCategories) {
        if (!player[game_category].empty()) {
          const std::experimental::filesystem::path place_percentage_vs_game_number_path{
            directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName / Path::gnuplot_player_place_percentage_vs_game_number_file_name(game_category)
          };
          run_command("gnuplot " + place_percentage_vs_game_number_path.string());
          const std::experimental::filesystem::path place_percentage_vs_date_path{
            directory / player.name().directory_name() / Path::PlayerPlotsDirectoryName / Path::gnuplot_player_place_percentage_vs_date_file_name(game_category)
          };
          run_command("gnuplot " + place_percentage_vs_date_path.string());
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

  void run_command(const std::string& command) const {
    const int outcome{std::system(command.c_str())};
    if (outcome != 0) {
      error("Could not run the command: " + command);
    }
  }

};

} // namespace CatanLeaderboardGenerator
