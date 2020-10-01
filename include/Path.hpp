#pragma once

#include "Place.hpp"
#include "PlayerName.hpp"

namespace CatanLeaderboardGenerator {

namespace Path {

std::string common_file_name(const GameCategory game_category) noexcept {
  return replace_character(replace_character(lowercase(label(game_category)), ' ', '_'), '-', '_');
}

std::experimental::filesystem::path global_plots_directory(const std::experimental::filesystem::path& directory) noexcept {
  return directory / std::experimental::filesystem::path{"_plots"};
}

std::experimental::filesystem::path player_directory(const std::experimental::filesystem::path& directory, const PlayerName& player_name) noexcept {
  return directory / std::experimental::filesystem::path{player_name.value()};
}

std::experimental::filesystem::path player_data_directory(const std::experimental::filesystem::path& directory, const PlayerName& player_name) noexcept {
  return player_directory(directory, player_name) / std::experimental::filesystem::path{"data"};
}

std::experimental::filesystem::path player_plots_directory(const std::experimental::filesystem::path& directory, const PlayerName& player_name) noexcept {
  return player_directory(directory, player_name) / std::experimental::filesystem::path{"plots"};
}

std::experimental::filesystem::path player_data_table_file_path(
  const std::experimental::filesystem::path& directory,
  const PlayerName& player_name,
  const GameCategory game_category
) noexcept {
  return {player_data_directory(directory, player_name) / std::experimental::filesystem::path{common_file_name(game_category) + ".dat"}};
}

std::experimental::filesystem::path gnuplot_global_average_points_vs_game_number_file_path(
  const std::experimental::filesystem::path& directory,
  const GameCategory game_category
) noexcept {
  return global_plots_directory(directory) / std::experimental::filesystem::path{"average_points_vs_game_number_" + common_file_name(game_category) + ".gnuplot"};
}

std::experimental::filesystem::path gnuplot_global_average_points_vs_date_file_path(
  const std::experimental::filesystem::path& directory,
  const GameCategory game_category
) noexcept {
  return global_plots_directory(directory) / std::experimental::filesystem::path{"average_points_vs_date_" + common_file_name(game_category) + ".gnuplot"};
}

std::experimental::filesystem::path gnuplot_global_place_percentage_vs_game_number_file_path(
  const std::experimental::filesystem::path& directory,
  const GameCategory game_category,
  const Place& place
) noexcept {
  return global_plots_directory(directory) / std::experimental::filesystem::path{place.print() + "_place_percentage_vs_game_number_" + common_file_name(game_category) + ".gnuplot"};
}

std::experimental::filesystem::path gnuplot_global_place_percentage_vs_date_file_path(
  const std::experimental::filesystem::path& directory,
  const GameCategory game_category,
  const Place& place
) noexcept {
  return global_plots_directory(directory) / std::experimental::filesystem::path{place.print() + "_place_percentage_vs_date_" + common_file_name(game_category) + ".gnuplot"};
}

std::experimental::filesystem::path gnuplot_player_average_points_vs_game_number_file_path(
  const std::experimental::filesystem::path& directory,
  const PlayerName& player_name
) noexcept {
  return {player_plots_directory(directory, player_name) / std::experimental::filesystem::path{"average_points_vs_game_number.gnuplot"}};
}

std::experimental::filesystem::path gnuplot_player_average_points_vs_date_file_path(
  const std::experimental::filesystem::path& directory,
  const PlayerName& player_name
) noexcept {
  return {player_plots_directory(directory, player_name) / std::experimental::filesystem::path{"average_points_vs_date.gnuplot"}};
}

std::experimental::filesystem::path gnuplot_player_place_percentage_vs_game_number_file_path(
  const std::experimental::filesystem::path& directory,
  const PlayerName& player_name,
  const GameCategory game_category
) noexcept {
  return {player_plots_directory(directory, player_name) / std::experimental::filesystem::path{"place_percentage_vs_game_number_" + common_file_name(game_category) + ".gnuplot"}};
}

std::experimental::filesystem::path gnuplot_player_place_percentage_vs_date_file_path(
  const std::experimental::filesystem::path& directory,
  const PlayerName& player_name,
  const GameCategory game_category
) noexcept {
  return {player_plots_directory(directory, player_name) / std::experimental::filesystem::path{"place_percentage_vs_date_" + common_file_name(game_category) + ".gnuplot"}};
}

} // namespace Path

} // namespace CatanLeaderboardGenerator
