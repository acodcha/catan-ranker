#pragma once

#include "MarkdownFileWriter.hpp"
#include "Path.hpp"
#include "Player.hpp"

namespace CatanRanker {

class LeaderboardIndividualFileWriter : public MarkdownFileWriter {
public:
  LeaderboardIndividualFileWriter(
      const std::experimental::filesystem::path& base_directory,
      const Games& games, const Player& player) noexcept
    : MarkdownFileWriter(base_directory / player.name().directory_name()
                             / Path::LeaderboardFileName,
                         player.name().value()) {
    line("Last updated " + current_utc_date_and_time() + ".");
    blank_line();
    list_link(section_title_players_table_);
    list_link(section_title_elo_rating_plot_);
    list_link(section_title_average_points_plot_);
    list_link(section_title_place_percentage_plot_);
    list_link(section_title_games_tables_);
    link_back_to_main_page();
    section(section_title_players_table_);
    player_table(player);
    link_back_to_top();
    section(section_title_elo_rating_plot_);
    elo_rating_plot();
    link_back_to_top();
    section(section_title_average_points_plot_);
    average_points_plot();
    link_back_to_top();
    section(section_title_place_percentage_plot_);
    place_percentage_plot(
        base_directory, player, GameCategory::AnyNumberOfPlayers);
    link_back_to_top();
    section(section_title_games_tables_);
    for (const GameCategory game_category : GameCategories) {
      list_link(label(game_category) + " " + section_title_games_tables_);
    }
    link_back_to_top();
    for (const GameCategory game_category : GameCategories) {
      subsection(label(game_category) + " " + section_title_games_tables_);
      games_table(games, player, game_category);
      link_back_to_section(section_title_games_tables_);
    }
    blank_line();
  }

protected:
  const std::string section_title_players_table_{"Summary"};

  const std::string section_title_elo_rating_plot_{"Ratings"};

  const std::string section_title_average_points_plot_{"Average Points"};

  const std::string section_title_place_percentage_plot_{"Win Rates"};

  const std::string section_title_games_tables_{"History"};

  void player_table(const Player& player) noexcept {
    Column category{"Category", Column::Alignment::Center};
    Column number_of_games{"Games", Column::Alignment::Center};
    Column elo_rating{"Current Rating", Column::Alignment::Center};
    Column average_elo_rating{"Avg Rating", Column::Alignment::Center};
    Column average_points_per_game{"Avg Points", Column::Alignment::Center};
    Column first_place{"1st Place", Column::Alignment::Center};
    Column second_place{"2nd Place", Column::Alignment::Center};
    Column third_place{"3rd Place", Column::Alignment::Center};
    for (const GameCategory game_category : GameCategories) {
      const std::optional<PlayerProperties> latest{
          player.latest_properties(game_category)};
      if (latest.has_value()) {
        category.add_row(label(game_category));
        number_of_games.add_row(
            latest.value().player_game_category_game_number());
        elo_rating.add_row(latest.value().elo_rating());
        average_elo_rating.add_row(latest.value().average_elo_rating());
        average_points_per_game.add_row(
            latest.value().average_points_per_game(), 3);
        const int64_t first_place_count{latest.value().place_count({1})};
        const int64_t second_place_count{latest.value().place_count({2})};
        const int64_t third_place_count{latest.value().place_count({3})};
        const Percentage first_place_percentage{
            latest.value().place_percentage({1})};
        const Percentage second_place_percentage{
            latest.value().place_percentage({2})};
        const Percentage third_place_percentage{
            latest.value().place_percentage({3})};
        first_place.add_row(first_place_percentage.print(0) + " ("
                            + std::to_string(first_place_count) + ")");
        second_place.add_row(second_place_percentage.print(0) + " ("
                             + std::to_string(second_place_count) + ")");
        third_place.add_row(third_place_percentage.print(0) + " ("
                            + std::to_string(third_place_count) + ")");
      }
    }
    const Table data{
        {category, number_of_games, elo_rating, average_elo_rating,
         average_points_per_game, first_place, second_place, third_place}
    };
    table(data);
  }

  void elo_rating_plot() noexcept {
    line("![Ratings History Plot]("
         + Path::gnuplot_path_to_png_path(
               Path::PlayerPlotsDirectoryName
               / Path::PlayerEloRatingVsGameNumberFileName)
               .string()
         + ")");
  }

  void average_points_plot() noexcept {
    line("![Average Points History Plot]("
         + Path::gnuplot_path_to_png_path(
               Path::PlayerPlotsDirectoryName
               / Path::PlayerAveragePointsVsGameNumberFileName)
               .string()
         + ")");
  }

  void place_percentage_plot(
      const std::experimental::filesystem::path& base_directory,
      const Player& player, const GameCategory game_category) noexcept {
    const std::experimental::filesystem::path gnuplot_path{
        Path::PlayerPlotsDirectoryName
        / Path::individual_place_percentage_vs_game_number_file_name(
            game_category)};
    if (std::experimental::filesystem::exists(
            base_directory / player.name().directory_name() / gnuplot_path)) {
      line("![Win Rates Plot]("
           + Path::gnuplot_path_to_png_path(gnuplot_path).string() + ")");
    }
  }

  void games_table(const Games& games, const Player& player,
                   const GameCategory game_category) noexcept {
    if (player[game_category].empty()) {
      return;
    }
    Column game_number{"Game", Column::Alignment::Center};
    Column date{"Date", Column::Alignment::Center};
    Column winning_points{"Points", Column::Alignment::Center};
    Column number_of_players{"Players", Column::Alignment::Center};
    Column results{"Results", Column::Alignment::Left};
    std::size_t counter{0};
    for (std::vector<Game>::const_reverse_iterator game = games.crbegin();
         game < games.crend(); ++game) {
      if ((game_category == GameCategory::AnyNumberOfPlayers
           || game_category == game->category())
          && game->participant(player.name())) {
        ++counter;
      }
    }
    for (std::vector<Game>::const_reverse_iterator game = games.crbegin();
         game < games.crend(); ++game) {
      if ((game_category == GameCategory::AnyNumberOfPlayers
           || game_category == game->category())
          && game->participant(player.name())) {
        game_number.add_row(counter);
        date.add_row(game->date());
        winning_points.add_row(game->winning_points());
        number_of_players.add_row(game->number_of_players());
        results.add_row(game->print_results());
        --counter;
      }
    }
    const Table data{
        {game_number, date, winning_points, number_of_players, results}
    };
    table(data);
  }
};

}  // namespace CatanRanker
