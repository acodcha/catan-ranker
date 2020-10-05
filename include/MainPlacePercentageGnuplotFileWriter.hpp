#pragma once

#include "GnuplotFileWriter.hpp"
#include "Place.hpp"
#include "PlayerName.hpp"

namespace CatanLeaderboardGenerator {

class MainPlacePercentageGnuplotFileWriter : public GnuplotFileWriter {

public:

  MainPlacePercentageGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const GameCategory game_category,
    const Place& place
  ) noexcept : GnuplotFileWriter(path) {
    line("set terminal pngcairo size 800,600 enhanced font \"Verdana,10\"");
    line("set title \"\"");
    line("set grid xtics ytics mxtics mytics");
    line("set key horizontal center top outside");
    line("set ylabel \"" + label(game_category) + " " + place.print() + " Place %\"");
    line("set yrange [0:100]");
    line("set ytics mirror out 10.0");
    line("set mytics 10");
    line("set y2label \"\"");
    line("set y2range [0:100]");
    line("set y2tics mirror in 10.0");
    line("set my2tics 10");
    line("set output \"" + png_file_path().string() + "\"");
  }

protected:

  virtual uint_least8_t x_column() const noexcept = 0;

  constexpr uint_least8_t y_column(const Place& place) const noexcept {
    return place.value() + 6;
  }

  void plot(const Players& players, const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data, const Place& place) noexcept {
    uint_least64_t counter{0};
    for (const std::pair<PlayerName, std::experimental::filesystem::path>& datum : data) {
      line("  \"" + datum.second.string() + "\" u " + std::to_string(x_column()) + ":" + std::to_string(y_column(place)) + " w lp lw 2 pt " + std::to_string(players.find(datum.first).gnuplot_point_type()) + " ps 1 lt rgb \"#" + players.find(datum.first).color() + "\" t \"" + datum.first.value() + "\" , \\");
      ++counter;
    }
  }

};

class MainPlacePercentageVsGameNumberGnuplotFileWriter : public MainPlacePercentageGnuplotFileWriter {

public:

  MainPlacePercentageVsGameNumberGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const Players& players,
    const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data,
    const GameCategory game_category,
    const Place& place
  ) noexcept : MainPlacePercentageGnuplotFileWriter(path, game_category, place) {
    line("set xlabel \"Game Number\"");
    line("set xtics nomirror out");
    line("set mxtics 1");
    line("plot \\");
    plot(players, data, place);
  }

protected:

  constexpr uint_least8_t x_column() const noexcept {
    return 2;
  }

};

class MainPlacePercentageVsDateGnuplotFileWriter : public MainPlacePercentageGnuplotFileWriter {

public:

  MainPlacePercentageVsDateGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const Players& players,
    const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data,
    const GameCategory game_category,
    const Place& place
  ) noexcept : MainPlacePercentageGnuplotFileWriter(path, game_category, place) {
    line("set timefmt \"%Y-%m-%d\"");
    line("set xlabel \"Date\"");
    line("set xdata time");
    line("set format x \"%Y-%m-%d\"");
    line("set xtics nomirror out rotate by 45 right");
    line("set mxtics 1");
    line("plot \\");
    plot(players, data, place);
  }

protected:

  constexpr uint_least8_t x_column() const noexcept {
    return 5;
  }

};

} // namespace CatanLeaderboardGenerator
