#pragma once

#include "MarkdownFileWriter.hpp"
#include "Player.hpp"

namespace CatanLeaderboardGenerator {

class ResultsPlayerSummaryFileWriter : public MarkdownFileWriter {

public:

  ResultsPlayerSummaryFileWriter(const std::experimental::filesystem::path& path, const Games& games, const Player& player) noexcept : MarkdownFileWriter(path, player.name().value()) {
    line("Last updated " + current_local_date_and_time() + " local time (" + current_utc_date_and_time() + ").");
    player_table(player);
    section(section_title_games_);
    for (const GameCategory game_category : GameCategories) {
      games_table(games, player, game_category);
    }
    blank_line();
  }

protected:

  const std::string section_title_games_{"Game History"};

  void player_table(const Player& player) noexcept {
    section("Player Statistics");
    Column category{"Category", Column::Alignment::Center};
    Column local_game_number{"Games", Column::Alignment::Center};
    Column average_points_per_game{"Points", Column::Alignment::Center};
    Column first_place_percentage{"1st Place", Column::Alignment::Center};
    Column second_place_percentage{"2nd Place", Column::Alignment::Center};
    Column third_place_percentage{"3rd Place", Column::Alignment::Center};
    for (const GameCategory game_category : GameCategories) {
      category.add_row(label(game_category));
      if (player[game_category].empty()) {
        local_game_number.add_row(0);
        average_points_per_game.add_row("–");
        first_place_percentage.add_row("–");
        second_place_percentage.add_row("–");
        third_place_percentage.add_row("–");
      } else {
        local_game_number.add_row(player[game_category].back().local_game_number());
        average_points_per_game.add_row(player[game_category].back().average_points_per_game(), 2);
        first_place_percentage.add_row(player[game_category].back().place_percentage({1}), 0);
        second_place_percentage.add_row(player[game_category].back().place_percentage({2}), 0);
        third_place_percentage.add_row(player[game_category].back().place_percentage({3}), 0);
      }
    }
    const Table data{{category, local_game_number, average_points_per_game, first_place_percentage, second_place_percentage, third_place_percentage}};
    table(data);
  }

  void games_table(const Games& games, const Player& player, const GameCategory game_category) noexcept {
    subsection(section_title_games_ + ": " + label(game_category));
    Column game_number{"Game", Column::Alignment::Center};
    Column date{"Date", Column::Alignment::Center};
    Column number_of_players{"Players", Column::Alignment::Center};
    Column results{"Results", Column::Alignment::Left};
    uint_least64_t counter{0};
    for (std::vector<Game>::const_reverse_iterator game = games.crbegin(); game < games.crend(); ++game) {
      if ((game_category == GameCategory::AnyNumberOfPlayers || game_category == game->category()) && game->participant(player.name())) {
        ++counter;
      }
    }
    for (std::vector<Game>::const_reverse_iterator game = games.crbegin(); game < games.crend(); ++game) {
      if ((game_category == GameCategory::AnyNumberOfPlayers || game_category == game->category()) && game->participant(player.name())) {
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
