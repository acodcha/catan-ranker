#pragma once

#include "GnuplotFileWriter.hpp"
#include "Players.hpp"

namespace CatanLeaderboardGenerator {

class MainEloRatingGnuplotFileWriter : public GnuplotFileWriter {

public:

  MainEloRatingGnuplotFileWriter(const std::experimental::filesystem::path& path, const EloRating& lowest, const EloRating& highest) noexcept : GnuplotFileWriter(path) {
    const uint_least64_t increment{100};
    const uint_least64_t y_minimum{std::min((uint_least64_t)(EloRatingStartingValue - increment), nearest_lower_nice_number(lowest.value(), increment))};
    const uint_least64_t y_maximum{std::max((uint_least64_t)(EloRatingStartingValue + increment), nearest_higher_nice_number(highest.value(), increment))};
    line("set terminal pngcairo size 800,600 enhanced font \"Verdana,10\"");
    line("set title \"\"");
    line("set grid xtics ytics mxtics mytics");
    line("set key horizontal center top outside");
    line("set ylabel \"Average Points per Game\"");
    line("set yrange [" + std::to_string(y_minimum) + ":" + std::to_string(y_maximum) + "]");
    line("set ytics mirror out");
    line("set mytics 10");
    line("set y2label \"\"");
    line("set y2range [" + std::to_string(y_minimum) + ":" + std::to_string(y_maximum) + "]");
    line("set y2tics mirror in");
    line("set my2tics 10");
    line("set output \"" + png_file_path().string() + "\"");
  }

protected:

  virtual uint_least8_t x_column() const noexcept = 0;

  void plot(const Players& players, const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data) noexcept {
    uint_least64_t counter{0};
    for (const std::pair<PlayerName, std::experimental::filesystem::path>& datum : data) {
      line("  \"" + datum.second.string() + "\" u " + std::to_string(x_column()) + ":6 w lp lw 2 pt " + std::to_string(players.find(datum.first).gnuplot_point_type()) + " ps 1 lt rgb \"#" + players.find(datum.first).color() + "\" t \"" + datum.first.value() + "\" , \\");
      ++counter;
    }
  }

};

class MainEloRatingVsGameNumberGnuplotFileWriter : public MainEloRatingGnuplotFileWriter {

public:

  MainEloRatingVsGameNumberGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const Players& players,
    const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data,
    const GameCategory game_category
  ) noexcept : MainEloRatingGnuplotFileWriter(path, players.lowest_elo_rating(game_category), players.highest_elo_rating(game_category)) {
    line("set xlabel \"Game Number\"");
    line("set xtics nomirror out");
    line("set mxtics 1");
    line("plot \\");
    plot(players, data);
  }

protected:

  constexpr uint_least8_t x_column() const noexcept {
    return 2;
  }

};

class MainEloRatingVsDateGnuplotFileWriter : public MainEloRatingGnuplotFileWriter {

public:

  MainEloRatingVsDateGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const Players& players,
    const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data,
    const GameCategory game_category
  ) noexcept : MainEloRatingGnuplotFileWriter(path, players.lowest_elo_rating(game_category), players.highest_elo_rating(game_category)) {
    line("set timefmt \"%Y-%m-%d\"");
    line("set xlabel \"Date\"");
    line("set xdata time");
    line("set format x \"%Y-%m-%d\"");
    line("set xtics nomirror out rotate by 45 right");
    line("set mxtics 1");
    line("plot \\");
    plot(players, data);
  }

protected:

  constexpr uint_least8_t x_column() const noexcept {
    return 5;
  }

};

} // namespace CatanLeaderboardGenerator
