#pragma once

#include "Place.hpp"
#include "PlayerName.hpp"

namespace CatanLeaderboardGenerator {

namespace Path {

const std::experimental::filesystem::path MainPlotsDirectoryName{"_plots"};

const std::experimental::filesystem::path PlayerDataDirectoryName{"data"};

const std::experimental::filesystem::path PlayerPlotsDirectoryName{"plots"};

const std::experimental::filesystem::path LeaderboardFileName{"README.md"};

const std::experimental::filesystem::path PlayerAveragePointsVsGameNumberFileName{"average_points_vs_game_number.gnuplot"};

const std::experimental::filesystem::path PlayerAveragePointsVsDateFileName{"average_points_vs_date.gnuplot"};

std::experimental::filesystem::path gnuplot_path_to_png_path(const std::experimental::filesystem::path& gnuplot_path) noexcept {
  std::experimental::filesystem::path png_path{gnuplot_path};
  png_path.replace_extension(".png");
  return png_path;
}

std::experimental::filesystem::path game_category_file_name(const GameCategory game_category) noexcept {
  return {replace_character(replace_character(lowercase(label(game_category)), ' ', '_'), '-', '_')};
}

std::experimental::filesystem::path player_data_file_name(const GameCategory game_category) noexcept {
  return {game_category_file_name(game_category).string() + ".dat"};
}

std::experimental::filesystem::path main_average_points_vs_game_number_file_name(const GameCategory game_category) noexcept {
  return {game_category_file_name(game_category).string() + "_average_points_vs_game_number.gnuplot"};
}

std::experimental::filesystem::path main_average_points_vs_date_file_name(const GameCategory game_category) noexcept {
  return {game_category_file_name(game_category).string() + "_average_points_vs_date.gnuplot"};
}

std::experimental::filesystem::path main_place_percentage_vs_game_number_file_name(const GameCategory game_category, const Place& place) noexcept {
  return {game_category_file_name(game_category).string() + "_" + place.print() + "_place_percentage_vs_game_number.gnuplot"};
}

std::experimental::filesystem::path main_place_percentage_vs_date_file_name(const GameCategory game_category, const Place& place) noexcept {
  return {game_category_file_name(game_category).string() + "_" + place.print() + "_place_percentage_vs_date.gnuplot"};
}

std::experimental::filesystem::path player_place_percentage_vs_game_number_file_name(const GameCategory game_category) noexcept {
  return {"place_percentage_vs_game_number_" + game_category_file_name(game_category).string() + ".gnuplot"};
}

std::experimental::filesystem::path player_place_percentage_vs_date_file_name(const GameCategory game_category) noexcept {
  return {"place_percentage_vs_date_" + game_category_file_name(game_category).string() + ".gnuplot"};
}

} // namespace Path

} // namespace CatanLeaderboardGenerator
