#pragma once

#include "MarkdownFileWriter.hpp"
#include "Path.hpp"
#include "Players.hpp"

namespace CatanLeaderboardGenerator {

class LeaderboardMainFileWriter : public MarkdownFileWriter {

public:

  LeaderboardMainFileWriter(
    const std::experimental::filesystem::path& base_directory,
    const Games& games,
    const Players& players
  ) noexcept : MarkdownFileWriter(base_directory / Path::LeaderboardFileName, "Catan Leaderboard") {
    line("Last updated " + current_utc_date_and_time() + ".");
    section(section_title_players_table_);
    for (const GameCategory game_category : GameCategories) {
      players_table(players, game_category);
    }
    for (const GameCategory game_category : GameCategories) {
      section(label(game_category));
      elo_rating_plot(base_directory, game_category);
      average_points_plot(base_directory, game_category);
      for (const Place place : PlacesFirstSecondThird) {
        place_percentage_plot(base_directory, game_category, place);
      }
    }
    section(section_title_games_tables_);
    for (const GameCategory game_category : GameCategories) {
      games_table(games, game_category);
    }
    blank_line();
  }

protected:

  const std::string section_title_players_table_{"Players"};

  const std::string section_title_games_tables_{"Game History"};

  void players_table(const Players& players, const GameCategory game_category) noexcept {
    bool is_empty{true};
    for (const Player& player : players) {
      if (!player[game_category].empty()) {
        is_empty = false;
        break;
      }
    }
    if (is_empty) {
      return;
    }
    subsection(section_title_players_table_ + ": " + label(game_category));
    Column name{"Player", Column::Alignment::Left};
    Column number_of_games{"Games", Column::Alignment::Center};
    Column elo_rating{"Rating", Column::Alignment::Center};
    Column average_points_per_game{"Points", Column::Alignment::Center};
    Column first_place{"1st Place", Column::Alignment::Center};
    Column second_place{"2nd Place", Column::Alignment::Center};
    Column third_place{"3rd Place", Column::Alignment::Center};
    Column first_or_second_place{"1st or 2nd Place", Column::Alignment::Center};
    Column first_or_second_or_third_place{"1st, 2nd, or 3rd Place", Column::Alignment::Center};
    for (const Player& player : players) {
      const std::optional<PlayerProperties> latest{player.latest_properties(game_category)};
      if (latest.has_value()) {
        const std::experimental::filesystem::path leaderboard_file_path{player.name().directory_name() / Path::LeaderboardFileName};
        name.add_row("[" + player.name().value() + "](" + leaderboard_file_path.string() + ")");
        number_of_games.add_row(latest.value().player_game_category_game_number());
        elo_rating.add_row(latest.value().elo_rating());
        average_points_per_game.add_row(latest.value().average_points_per_game(), 2);
        const uint_least64_t first_place_count{latest.value().place_count({1})};
        const uint_least64_t second_place_count{latest.value().place_count({2})};
        const uint_least64_t third_place_count{latest.value().place_count({3})};
        const Percentage first_place_percentage{latest.value().place_percentage({1})};
        const Percentage second_place_percentage{latest.value().place_percentage({2})};
        const Percentage third_place_percentage{latest.value().place_percentage({3})};
        first_place.add_row(std::to_string(first_place_count) + " , " + first_place_percentage.print(0));
        second_place.add_row(std::to_string(second_place_count) + " , " + second_place_percentage.print(0));
        third_place.add_row(std::to_string(third_place_count) + " , " + third_place_percentage.print(0));
        first_or_second_place.add_row(std::to_string(first_place_count + second_place_count) + " , " + Percentage{first_place_percentage + second_place_percentage}.print(0));
        first_or_second_or_third_place.add_row(std::to_string(first_place_count + second_place_count + third_place_count) + " , " + Percentage{first_place_percentage + second_place_percentage + third_place_percentage}.print(0));
      }
    }
    const Table data{{name, number_of_games, elo_rating, average_points_per_game, first_place, second_place, third_place, first_or_second_place, first_or_second_or_third_place}};
    table(data);
  }

  void elo_rating_plot(const std::experimental::filesystem::path& base_directory, const GameCategory game_category) noexcept {
    const std::experimental::filesystem::path gnuplot_path{
      Path::MainPlotsDirectoryName / Path::main_elo_rating_vs_game_number_file_name(game_category)
    };
    if (std::experimental::filesystem::exists(base_directory / gnuplot_path)) {
      subsection(label(game_category) + ": Ratings");
      line("![](" + Path::gnuplot_path_to_png_path(gnuplot_path).string() + ")");
    }
  }

  void average_points_plot(const std::experimental::filesystem::path& base_directory, const GameCategory game_category) noexcept {
    const std::experimental::filesystem::path gnuplot_path{
      Path::MainPlotsDirectoryName / Path::main_average_points_vs_game_number_file_name(game_category)
    };
    if (std::experimental::filesystem::exists(base_directory / gnuplot_path)) {
      subsection(label(game_category) + ": Average Points per Game");
      line("![](" + Path::gnuplot_path_to_png_path(gnuplot_path).string() + ")");
    }
  }

  void place_percentage_plot(const std::experimental::filesystem::path& base_directory, const GameCategory game_category, const Place& place) noexcept {
    const std::experimental::filesystem::path gnuplot_path{
      Path::MainPlotsDirectoryName / Path::main_place_percentage_vs_game_number_file_name(game_category, place)
    };
    if (std::experimental::filesystem::exists(base_directory / gnuplot_path)) {
      subsection(label(game_category) + ": " + section_title_place_percentage_plots(place));
      line("![](" + Path::gnuplot_path_to_png_path(gnuplot_path).string() + ")");
    }
  }

  void games_table(const Games& games, const GameCategory game_category) noexcept {
    bool is_empty{true};
    for (const Game& game : games) {
      if (game_category == GameCategory::AnyNumberOfPlayers || game_category == game.category()) {
        is_empty = false;
        break;
      }
    }
    if (is_empty) {
      return;
    }
    subsection(section_title_games_tables_ + ": " + label(game_category));
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

  std::string section_title_place_percentage_plots(const Place place) const noexcept {
    return place.print() + " Places";
  }

};

} // namespace CatanLeaderboardGenerator
