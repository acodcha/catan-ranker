#pragma once

#include "MarkdownFileWriter.hpp"
#include "Players.hpp"

namespace CatanLeaderboard {

class ResultsSummaryFileWriter : public MarkdownFileWriter {

public:

  ResultsSummaryFileWriter(const std::experimental::filesystem::path& prefix, const Games& games, const Players& players) noexcept : MarkdownFileWriter(prefix / std::experimental::filesystem::path{"README.md"}, "Summary") {
    players_table("All Games", GameCategory::AnyNumberOfPlayers, players);
    players_table("3-4 Player Games", GameCategory::ThreeToFourPlayers, players);
    players_table("5-6 Player Games", GameCategory::FiveToSixPlayers, players);
    players_table("7-8 Player Games", GameCategory::SevenToEightPlayers, players);
    blank_line();
    message("Wrote the summary file to: " + path_.string());
  }

protected:

  void players_table(const std::string title, const GameCategory game_category, const Players& players) noexcept {
    section(title);
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
        average_points_per_game.add_row<RealNumber::FloatingPoint>(player[game_category].back().average_points_per_game(), 2);
        first_place_percentage.add_row<RealNumber::Percentage>(player[game_category].back().place_ratio({1}));
        second_place_percentage.add_row<RealNumber::Percentage>(player[game_category].back().place_ratio({2}));
        third_place_percentage.add_row<RealNumber::Percentage>(player[game_category].back().place_ratio({3}));
      }
    }
    table({{player_name, number_of_games, average_points_per_game, first_place_percentage, second_place_percentage, third_place_percentage}});
  }

};

} // namespace CatanLeaderboard
