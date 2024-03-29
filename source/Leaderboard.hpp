#pragma once

#include "DataFileWriter.hpp"
#include "GlobalAveragePointsGnuplotFileWriter.hpp"
#include "GlobalEloRatingGnuplotFileWriter.hpp"
#include "GlobalPlacePercentageGnuplotFileWriter.hpp"
#include "IndividualAveragePointsGnuplotFileWriter.hpp"
#include "IndividualEloRatingGnuplotFileWriter.hpp"
#include "IndividualPlacePercentageGnuplotFileWriter.hpp"
#include "LeaderboardGlobalFileWriter.hpp"
#include "LeaderboardIndividualFileWriter.hpp"

namespace CatanRanker {

/// \brief Class that writes all leaderboard files given games and players data.
class Leaderboard {
public:
  Leaderboard(const std::experimental::filesystem::path& base_directory,
              const Games& games, const Players& players) {
    if (!base_directory.empty()) {
      create_directories(base_directory, players);
      write_data_files(base_directory, players);
      write_global_gnuplot_files(base_directory, players);
      write_player_gnuplot_files(base_directory, players);
      write_global_leaderboard_file(base_directory, games, players);
      write_player_leaderboard_files(base_directory, games, players);
      generate_global_plots(base_directory);
      generate_player_plots(base_directory, players);
    }
  }

private:
  void create_directories(
      const std::experimental::filesystem::path& base_directory,
      const Players& players) {
    create(base_directory);
    create(base_directory / Path::PlayersDirectoryName);
    create(base_directory / Path::MainPlotsDirectoryName);
    for (const Player& player : players) {
      create(base_directory / player.name().directory_name());
      create(base_directory / player.name().directory_name()
             / Path::PlayerDataDirectoryName);
      create(base_directory / player.name().directory_name()
             / Path::PlayerPlotsDirectoryName);
    }
  }

  void write_data_files(
      const std::experimental::filesystem::path& base_directory,
      const Players& players) noexcept {
    for (const Player& player : players) {
      for (const GameCategory game_category : GameCategories) {
        if (!player[game_category].empty()) {
          DataFileWriter{base_directory / player.name().directory_name()
                             / Path::PlayerDataDirectoryName
                             / Path::player_data_file_name(game_category),
                         player_table(player, game_category)};
        }
      }
    }
    message("Wrote the data files.");
  }

  void write_global_gnuplot_files(
      const std::experimental::filesystem::path& base_directory,
      const Players& players) noexcept {
    for (const GameCategory game_category : GameCategories) {
      std::map<PlayerName, std::experimental::filesystem::path,
               PlayerName::sort>
          data_paths;
      for (const Player& player : players) {
        if (!player[game_category].empty() && !player.color().empty()) {
          data_paths.insert({player.name(),
                             base_directory / player.name().directory_name()
                                 / Path::PlayerDataDirectoryName
                                 / Path::player_data_file_name(game_category)});
        }
      }
      if (!data_paths.empty()) {
        GlobalEloRatingVsGameNumberGnuplotFileWriter{
            base_directory / Path::MainPlotsDirectoryName
                / Path::global_elo_rating_vs_game_number_file_name(
                    game_category),
            players, data_paths, game_category};
        GlobalAveragePointsVsGameNumberGnuplotFileWriter{
            base_directory / Path::MainPlotsDirectoryName
                / Path::global_average_points_vs_game_number_file_name(
                    game_category),
            players, data_paths};
        GlobalPlacePercentageVsGameNumberGnuplotFileWriter{
            base_directory / Path::MainPlotsDirectoryName
                / Path::global_place_percentage_vs_game_number_file_name(
                    game_category, {1}),
            players,
            data_paths,
            game_category,
            {1}};
      }
    }
    message("Wrote the global Gnuplot files.");
  }

  void write_player_gnuplot_files(
      const std::experimental::filesystem::path& base_directory,
      const Players& players) noexcept {
    for (const Player& player : players) {
      std::map<GameCategory, std::experimental::filesystem::path> data_paths;
      for (const GameCategory game_category : GameCategories) {
        // Only generate a plot if this player has at least 2 games in this game
        // category.
        if (player[game_category].size() >= 2) {
          data_paths.insert({game_category,
                             base_directory / player.name().directory_name()
                                 / Path::PlayerDataDirectoryName
                                 / Path::player_data_file_name(game_category)});
        }
      }
      if (!data_paths.empty()) {
        IndividualEloRatingVsGameNumberGnuplotFileWriter{
            base_directory / player.name().directory_name()
                / Path::PlayerPlotsDirectoryName
                / Path::PlayerEloRatingVsGameNumberFileName,
            data_paths, player.lowest_elo_rating(),
            player.highest_elo_rating()};
        IndividualAveragePointsVsGameNumberGnuplotFileWriter{
            base_directory / player.name().directory_name()
                / Path::PlayerPlotsDirectoryName
                / Path::PlayerAveragePointsVsGameNumberFileName,
            data_paths};
      }
      if (player[GameCategory::AnyNumberOfPlayers].size() >= 2) {
        IndividualPlacePercentageVsGameNumberGnuplotFileWriter{
            base_directory / player.name().directory_name()
                / Path::PlayerPlotsDirectoryName
                / Path::individual_place_percentage_vs_game_number_file_name(
                    GameCategory::AnyNumberOfPlayers),
            base_directory / player.name().directory_name()
                / Path::PlayerDataDirectoryName
                / Path::player_data_file_name(GameCategory::AnyNumberOfPlayers),
            GameCategory::AnyNumberOfPlayers};
      }
    }
    message("Wrote the individual player Gnuplot files.");
  }

  void write_global_leaderboard_file(
      const std::experimental::filesystem::path& base_directory,
      const Games& games, const Players& players) noexcept {
    LeaderboardGlobalFileWriter{base_directory, games, players};
    message("Wrote the global leaderboard Markdown file.");
  }

  void write_player_leaderboard_files(
      const std::experimental::filesystem::path& base_directory,
      const Games& games, const Players& players) noexcept {
    for (const Player& player : players) {
      LeaderboardIndividualFileWriter{base_directory, games, player};
    }
    message("Wrote the individual player leaderboard Markdown files.");
  }

  void generate_global_plots(
      const std::experimental::filesystem::path& base_directory) const {
    message("Generating the global plots...");
    for (const GameCategory game_category : GameCategories) {
      generate_plot(
          base_directory / Path::MainPlotsDirectoryName
          / Path::global_elo_rating_vs_game_number_file_name(game_category));
      generate_plot(base_directory / Path::MainPlotsDirectoryName
                    / Path::global_average_points_vs_game_number_file_name(
                        game_category));
      for (const Place& place : PlacesFirstSecondThird) {
        generate_plot(base_directory / Path::MainPlotsDirectoryName
                      / Path::global_place_percentage_vs_game_number_file_name(
                          game_category, place));
      }
    }
    message("Generated the global plots.");
  }

  void generate_player_plots(
      const std::experimental::filesystem::path& base_directory,
      const Players& players) const {
    message("Generating the individual player plots...");
    for (const Player& player : players) {
      generate_plot(base_directory / player.name().directory_name()
                    / Path::PlayerPlotsDirectoryName
                    / Path::PlayerAveragePointsVsGameNumberFileName);
      generate_plot(base_directory / player.name().directory_name()
                    / Path::PlayerPlotsDirectoryName
                    / Path::PlayerEloRatingVsGameNumberFileName);
      for (const GameCategory game_category : GameCategories) {
        generate_plot(
            base_directory / player.name().directory_name()
            / Path::PlayerPlotsDirectoryName
            / Path::individual_place_percentage_vs_game_number_file_name(
                game_category));
      }
    }
    message("Generated the individual player plots.");
  }

  /// \brief Generate a plot using Gnuplot. If the path points to a file that
  /// does not exist, no plot is generated.
  void generate_plot(const std::experimental::filesystem::path& path) const {
    if (std::experimental::filesystem::exists(path)) {
      const std::string command{"gnuplot " + path.string()};
      const int outcome{std::system(command.c_str())};
      if (outcome != 0) {
        error("Could not run the command: " + command);
      }
    }
  }

  Table player_table(
      const Player& player, const GameCategory game_category) const noexcept {
    Column game_number{"Game#"};
    Column game_category_game_number{"GameCategory#"};
    Column player_game_number{"PlayerGame#"};
    Column player_game_category_game_number{"PlayerCategoryGame#"};
    Column date{"Date"};
    Column average_elo_rating{"AvgRating"};
    Column elo_rating{"CurrentRating"};
    Column average_points_per_game{"AvgPoints"};
    Column first_place_percentage{"1stPlace%"};
    Column second_place_percentage{"2ndPlace%"};
    Column third_place_percentage{"3rdPlace%"};
    Column first_or_second_place_percentage{"1stOr2ndPlace%"};
    Column first_or_second_or_third_place_percentage{"1stOr2ndOr3rdPlace%"};
    if (!player[game_category].empty()) {
      for (const PlayerProperties& properties : player[game_category]) {
        game_number.add_row(properties.game_number());
        game_category_game_number.add_row(
            properties.game_category_game_number());
        player_game_number.add_row(properties.player_game_number());
        player_game_category_game_number.add_row(
            properties.player_game_category_game_number());
        date.add_row(properties.date());
        average_elo_rating.add_row(properties.average_elo_rating());
        elo_rating.add_row(properties.elo_rating());
        average_points_per_game.add_row(
            properties.average_points_per_game(), 7);
        first_place_percentage.add_row(properties.place_percentage({1}), 5);
        second_place_percentage.add_row(properties.place_percentage({2}), 5);
        third_place_percentage.add_row(properties.place_percentage({3}), 5);
        first_or_second_place_percentage.add_row(
            properties.place_percentage({1}) + properties.place_percentage({2}),
            5);
        first_or_second_or_third_place_percentage.add_row(
            properties.place_percentage({1}) + properties.place_percentage({2})
                + properties.place_percentage({3}),
            5);
      }
    }
    return {
        {game_number, game_category_game_number, player_game_number,
         player_game_category_game_number, date, average_elo_rating, elo_rating,
         average_points_per_game, first_place_percentage,
         second_place_percentage, third_place_percentage,
         first_or_second_place_percentage, first_or_second_or_third_place_percentage}
    };
  }
};

}  // namespace CatanRanker
