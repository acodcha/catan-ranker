#pragma once

#include "DataTableFileWriter.hpp"
#include "GnuplotPointsFileWriter.hpp"
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
        write_points_data_table_files(directory, players);
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

  void write_points_data_table_files(const std::experimental::filesystem::path& directory, const Players& players) noexcept {
    for (const Player& player : players) {
      write_points_data_table_file(directory, player, GameCategory::AnyNumberOfPlayers);
      write_points_data_table_file(directory, player, GameCategory::ThreeToFourPlayers);
      write_points_data_table_file(directory, player, GameCategory::FiveToSixPlayers);
      write_points_data_table_file(directory, player, GameCategory::SevenToEightPlayers);
    }
  }

  void write_points_data_table_file(const std::experimental::filesystem::path& directory, const Player& player, const GameCategory game_category) noexcept {
    Column number{"Number", Column::Alignment::Left};
    Column date{"Date", Column::Alignment::Left};
    Column average_points_per_game{"AveragePointsPerGame", Column::Alignment::Left};
    uint_least64_t counter{0};
    if (!player[game_category].empty()) {
      for (const PlayerProperties& properties : player[game_category]) {
        ++counter;
        number.add_row(counter);
        date.add_row(properties.date());
        average_points_per_game.add_row(properties.average_points_per_game(), 7);
      }
    }
    const std::string file_name{"Points" + remove_whitespace(label(game_category))};
    const std::experimental::filesystem::path data_file_name{file_name + ".dat"};
    const std::experimental::filesystem::path gnuplot_file_name{file_name + ".gnuplot"};
    DataTableFileWriter{directory / player.name().value() / data_file_name, {{number, date, average_points_per_game}}};
    GnuplotPointsFileWriter(directory / player.name().value() / gnuplot_file_name, label(game_category) + ": Average Points per Game");
  }

};

} // namespace CatanLeaderboardGenerator
