#pragma once

#include "MarkdownFileWriter.hpp"
#include "Path.hpp"
#include "Player.hpp"

namespace CatanLeaderboardGenerator {

class LeaderboardPlayerFileWriter : public MarkdownFileWriter {

public:

  LeaderboardPlayerFileWriter(
    const std::experimental::filesystem::path& file_path,
    const std::experimental::filesystem::path& base_directory,
    const Games& games,
    const Player& player
  ) noexcept : MarkdownFileWriter(file_path, player.name().value()) {
    line("Last updated " + current_utc_date_and_time() + ".");
    blank_line();
    line("[Go back to all players.](../" + Path::LeaderboardFileName.string() + ")");
    section("Overview");
    player_table(player);
    section(section_title_average_points_plot_);
    average_points_plot();
    section(section_title_place_percentage_plot_);
    for (const GameCategory game_category : GameCategories) {
      place_percentage_plot(base_directory, player, game_category);
    }
    section(section_title_games_tables_);
    for (const GameCategory game_category : GameCategories) {
      games_table(games, player, game_category);
    }
    blank_line();
  }

protected:

  const std::string section_title_average_points_plot_{"Average Points per Game"};

  const std::string section_title_place_percentage_plot_{"Places"};

  const std::string section_title_games_tables_{"Game History"};

  void player_table(const Player& player) noexcept {
    Column category{"Category", Column::Alignment::Center};
    Column local_game_number{"Games", Column::Alignment::Center};
    Column average_points_per_game{"Points", Column::Alignment::Center};
    Column first_place_percentage{"1st Place", Column::Alignment::Center};
    Column second_place_percentage{"2nd Place", Column::Alignment::Center};
    Column third_place_percentage{"3rd Place", Column::Alignment::Center};
    for (const GameCategory game_category : GameCategories) {
      category.add_row(label(game_category));
      if (!player[game_category].empty()) {
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

  void average_points_plot() noexcept {
    line("![](" + Path::gnuplot_path_to_png_path(Path::PlayerPlotsDirectoryName / Path::PlayerAveragePointsVsGameNumberFileName).string() + ")");
  }

  void place_percentage_plot(const std::experimental::filesystem::path& base_directory, const Player& player, const GameCategory game_category) noexcept {
    const std::experimental::filesystem::path gnuplot_path{Path::PlayerPlotsDirectoryName / Path::player_place_percentage_vs_game_number_file_name(game_category)};
    if (std::experimental::filesystem::exists(base_directory / player.name().directory_name() / gnuplot_path)) {
      subsection(section_title_place_percentage_plot_ + ": " + label(game_category));
      line("![](" + Path::gnuplot_path_to_png_path(gnuplot_path).string() + ")");
    }
  }

  void games_table(const Games& games, const Player& player, const GameCategory game_category) noexcept {
    if (player[game_category].empty()) {
      return;
    }
    subsection(section_title_games_tables_ + ": " + label(game_category));
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
