#pragma once

#include "MarkdownFileWriter.hpp"
#include "Players.hpp"

namespace CatanLeaderboardGenerator {

class ResultsSummaryFileWriter : public MarkdownFileWriter {

public:

  ResultsSummaryFileWriter(const std::experimental::filesystem::path& path, const Games& games, const Players& players) noexcept : MarkdownFileWriter(path, "Results") {
    line("Last updated " + current_local_date_and_time() + " local time (" + current_utc_date_and_time() + ").");
    section(section_title_players_);
    players_table(players, GameCategory::AnyNumberOfPlayers);
    players_table(players, GameCategory::ThreeToFourPlayers);
    players_table(players, GameCategory::FiveToSixPlayers);
    players_table(players, GameCategory::SevenToEightPlayers);
    section(section_title_games_);
    games_table(games, GameCategory::AnyNumberOfPlayers);
    games_table(games, GameCategory::ThreeToFourPlayers);
    games_table(games, GameCategory::FiveToSixPlayers);
    games_table(games, GameCategory::SevenToEightPlayers);
    blank_line();
    message("Wrote the summary file to: " + path_.string());
  }

protected:

  const std::string section_title_players_{"Player Statistics"};

  const std::string section_title_games_{"Game History"};

  void players_table(const Players& players, const GameCategory game_category) noexcept {
    subsection(section_title_players_ + ": " + label(game_category));
    Column name{"Player", Column::Alignment::Left};
    Column local_number{"Games", Column::Alignment::Center};
    Column average_points_per_game{"Points", Column::Alignment::Center};
    Column first_place_percentage{"1st Place", Column::Alignment::Center};
    Column second_place_percentage{"2nd Place", Column::Alignment::Center};
    Column third_place_percentage{"3rd Place", Column::Alignment::Center};
    for (const Player& player : players) {
      if (!player[game_category].empty()) {
        name.add_row(player.name().value());
        local_number.add_row(player[game_category].back().local_game_number());
        average_points_per_game.add_row(player[game_category].back().average_points_per_game(), 2);
        first_place_percentage.add_row(player[game_category].back().place_percentage({1}), 0);
        second_place_percentage.add_row(player[game_category].back().place_percentage({2}), 0);
        third_place_percentage.add_row(player[game_category].back().place_percentage({3}), 0);
      }
    }
    const Table data{{name, local_number, average_points_per_game, first_place_percentage, second_place_percentage, third_place_percentage}};
    table(data);
  }

  void games_table(const Games& games, const GameCategory game_category) noexcept {
    subsection(section_title_games_ + ": " + label(game_category));
    Column game_number{"Game", Column::Alignment::Center};
    Column date{"Date", Column::Alignment::Center};
    Column number_of_players{"Players", Column::Alignment::Center};
    Column results{"Results", Column::Alignment::Left};
    uint_least64_t counter{0};
    for (std::vector<Game>::const_reverse_iterator game = games.crbegin(); game < games.crend(); ++game) {
      if (game_category == GameCategory::AnyNumberOfPlayers || game_category == game->category()) {
        ++counter;
      }
    }
    for (std::vector<Game>::const_reverse_iterator game = games.crbegin(); game < games.crend(); ++game) {
      if (game_category == GameCategory::AnyNumberOfPlayers || game_category == game->category()) {
        game_number.add_row(counter);
        date.add_row(game->date());
        number_of_players.add_row(game->number_of_players());
        results.add_row(game->print_results());
        --counter;
      }
    }
    const Table data{{game_number, date, number_of_players, results}};
    table(data);
  }

};

} // namespace CatanLeaderboardGenerator
