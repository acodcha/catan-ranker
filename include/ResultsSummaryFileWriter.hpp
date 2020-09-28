#pragma once

#include "MarkdownFileWriter.hpp"
#include "Players.hpp"

namespace CatanLeaderboardGenerator {

class ResultsSummaryFileWriter : public MarkdownFileWriter {

public:

  ResultsSummaryFileWriter(const std::experimental::filesystem::path& path, const Players& players) noexcept : MarkdownFileWriter(path, "Results") {
    line("Last updated " + current_local_date_and_time() + " local time (" + current_utc_date_and_time() + ").");
    players_table(GameCategory::AnyNumberOfPlayers, players);
    players_table(GameCategory::ThreeToFourPlayers, players);
    players_table(GameCategory::FiveToSixPlayers, players);
    players_table(GameCategory::SevenToEightPlayers, players);
    blank_line();
    message("Wrote the summary file to: " + path_.string());
  }

protected:

  void players_table(const GameCategory game_category, const Players& players) noexcept {
    section(label(game_category));
    Column player_name{"Player", Column::Alignment::Left};
    Column number_of_games{"Games", Column::Alignment::Center};
    Column average_points_per_game{"Points", Column::Alignment::Center};
    Column first_place_percentage{"1st Place", Column::Alignment::Center};
    Column second_place_percentage{"2nd Place", Column::Alignment::Center};
    Column third_place_percentage{"3rd Place", Column::Alignment::Center};
    for (const Player& player : players) {
      if (!player[game_category].empty()) {
        player_name.add_row(player.name().value());
        number_of_games.add_row(player[game_category].back().game_number());
        average_points_per_game.add_row(player[game_category].back().average_points_per_game(), 2);
        first_place_percentage.add_row(player[game_category].back().place_percentage({1}), 0);
        second_place_percentage.add_row(player[game_category].back().place_percentage({2}), 0);
        third_place_percentage.add_row(player[game_category].back().place_percentage({3}), 0);
      }
    }
    table({{player_name, number_of_games, average_points_per_game, first_place_percentage, second_place_percentage, third_place_percentage}});
  }

};

} // namespace CatanLeaderboardGenerator
