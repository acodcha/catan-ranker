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
    // Header.
    line("Leaderboard for Catan games with friends! Uses https://github.com/acodcha/CatanLeaderboardGenerator to generate the leaderboard. Last updated " + current_utc_date_and_time() + ".");
    blank_line();
    line("If you wish to participate in updating the leaderboard, contact me and I will add you as a collaborator to this project. As a collaborator, to update the leaderboard with new games, simply edit the `games.txt` file and make a pull request. The build system will automatically generate an updated leaderboard. Once I accept and merge the pull request, the leaderboard will be updated with your changes. See https://github.com/acodcha/CatanLeaderboardGenerator for more information about the `games.txt` file.");
    blank_line();
    for (const GameCategory game_category : GameCategories) {
      list_link(label(game_category));
    }
    list_link("License");
    for (const GameCategory game_category : GameCategories) {
      section(label(game_category));
      list_link(label(game_category) + " " + subsection_title_players_table_);
      list_link(label(game_category) + " " + subsection_title_elo_rating_plot_);
      list_link(label(game_category) + " " + subsection_title_average_points_plot_);
      list_link(label(game_category) + " " + subsection_title_first_place_percentage_plot_);
      list_link(label(game_category) + " " + subsection_title_games_table_);
      link_back_to_top();
      subsection(label(game_category) + " " + subsection_title_players_table_);
      players_table(players, game_category);
      link_back_to_section(label(game_category));
      subsection(label(game_category) + " " + subsection_title_elo_rating_plot_);
      elo_rating_plot(base_directory, game_category);
      link_back_to_section(label(game_category));
      subsection(label(game_category) + " " + subsection_title_average_points_plot_);
      average_points_plot(base_directory, game_category);
      link_back_to_section(label(game_category));
      subsection(label(game_category) + " " + subsection_title_first_place_percentage_plot_);
      place_percentage_plot(base_directory, game_category, {1});
      link_back_to_section(label(game_category));
      subsection(label(game_category) + " " + subsection_title_games_table_);
      games_table(games, game_category);
      link_back_to_section(label(game_category));
    }
    section("License");
    line("This work is maintained by Alexandre Coderre-Chabot (https://github.com/acodcha) and licensed under the MIT License. For more details, see the `LICENSE` file or https://mit-license.org/. This work is based on the Catan board game (also known as Settlers of Catan) originally designed by Klaus Teuber. The contents, copyrights, and trademarks of everything involving Catan are exclusively held by its designers and publishers; I make no claim to any of these in any way.");
    link_back_to_top();
    blank_line();
  }

protected:

  const std::string subsection_title_players_table_{"Summary"};

  const std::string subsection_title_elo_rating_plot_{"Ratings"};

  const std::string subsection_title_average_points_plot_{"Average Points"};

  const std::string subsection_title_first_place_percentage_plot_{"Win Rates"};

  const std::string subsection_title_games_table_{"History"};

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
    Column name{"Player", Column::Alignment::Left};
    Column number_of_games{"Games", Column::Alignment::Center};
    Column elo_rating{"Current Rating", Column::Alignment::Center};
    Column average_elo_rating{"Avg Rating", Column::Alignment::Center};
    Column average_points_per_game{"Avg Points", Column::Alignment::Center};
    Column first_place{"1st Place", Column::Alignment::Center};
    Column second_place{"2nd Place", Column::Alignment::Center};
    Column third_place{"3rd Place", Column::Alignment::Center};
    for (const Player& player : players) {
      const std::optional<PlayerProperties> latest{player.latest_properties(game_category)};
      if (latest.has_value()) {
        name.add_row("[" + player.name().value() + "](" + player.name().directory_name().string() + ")");
        number_of_games.add_row(latest.value().player_game_category_game_number());
        elo_rating.add_row(latest.value().elo_rating());
        average_elo_rating.add_row(latest.value().average_elo_rating());
        average_points_per_game.add_row(latest.value().average_points_per_game(), 3);
        const int64_t first_place_count{latest.value().place_count({1})};
        const int64_t second_place_count{latest.value().place_count({2})};
        const int64_t third_place_count{latest.value().place_count({3})};
        const Percentage first_place_percentage{latest.value().place_percentage({1})};
        const Percentage second_place_percentage{latest.value().place_percentage({2})};
        const Percentage third_place_percentage{latest.value().place_percentage({3})};
        first_place.add_row(first_place_percentage.print(0) + " (" + std::to_string(first_place_count) + ")");
        second_place.add_row(second_place_percentage.print(0) + " (" + std::to_string(second_place_count) + ")");
        third_place.add_row(third_place_percentage.print(0) + " (" + std::to_string(third_place_count) + ")");
      }
    }
    const Table data{{name, number_of_games, elo_rating, average_elo_rating, average_points_per_game, first_place, second_place, third_place}};
    table(data);
  }

  void elo_rating_plot(const std::experimental::filesystem::path& base_directory, const GameCategory game_category) noexcept {
    const std::experimental::filesystem::path gnuplot_path{
      Path::MainPlotsDirectoryName / Path::main_elo_rating_vs_game_number_file_name(game_category)
    };
    if (std::experimental::filesystem::exists(base_directory / gnuplot_path)) {
      line("![Ratings History Plot](" + Path::gnuplot_path_to_png_path(gnuplot_path).string() + ")");
    }
  }

  void average_points_plot(const std::experimental::filesystem::path& base_directory, const GameCategory game_category) noexcept {
    const std::experimental::filesystem::path gnuplot_path{
      Path::MainPlotsDirectoryName / Path::main_average_points_vs_game_number_file_name(game_category)
    };
    if (std::experimental::filesystem::exists(base_directory / gnuplot_path)) {
      line("![Average Points History Plot](" + Path::gnuplot_path_to_png_path(gnuplot_path).string() + ")");
    }
  }

  void place_percentage_plot(const std::experimental::filesystem::path& base_directory, const GameCategory game_category, const Place& place) noexcept {
    const std::experimental::filesystem::path gnuplot_path{
      Path::MainPlotsDirectoryName / Path::main_place_percentage_vs_game_number_file_name(game_category, place)
    };
    if (std::experimental::filesystem::exists(base_directory / gnuplot_path)) {
      line("![" + place.print() + " Place History Plot](" + Path::gnuplot_path_to_png_path(gnuplot_path).string() + ")");
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
    Column game_number{"Game", Column::Alignment::Center};
    Column date{"Date", Column::Alignment::Center};
    Column winning_points{"Points", Column::Alignment::Center};
    Column number_of_players{"Players", Column::Alignment::Center};
    Column results{"Results", Column::Alignment::Left};
    std::size_t counter{0};
    for (std::vector<Game>::const_reverse_iterator game = games.crbegin(); game < games.crend(); ++game) {
      if (game_category == GameCategory::AnyNumberOfPlayers || game_category == game->category()) {
        ++counter;
      }
    }
    for (std::vector<Game>::const_reverse_iterator game = games.crbegin(); game < games.crend(); ++game) {
      if (game_category == GameCategory::AnyNumberOfPlayers || game_category == game->category()) {
        game_number.add_row(counter);
        date.add_row(game->date());
        winning_points.add_row(game->winning_points());
        number_of_players.add_row(game->number_of_players());
        results.add_row(game->print_results());
        --counter;
      }
    }
    const Table data{{game_number, date, winning_points, number_of_players, results}};
    table(data);
  }

};

} // namespace CatanLeaderboardGenerator
