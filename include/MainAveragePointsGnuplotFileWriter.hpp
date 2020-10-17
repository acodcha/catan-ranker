#pragma once

#include "GnuplotFileWriter.hpp"
#include "Players.hpp"

namespace CatanLeaderboardGenerator {

class MainAveragePointsGnuplotFileWriter : public GnuplotFileWriter {

public:

  MainAveragePointsGnuplotFileWriter(const std::experimental::filesystem::path& path) noexcept : GnuplotFileWriter(path) {
    line("set title \"\"");
    line("set grid xtics ytics mxtics mytics");
    line("set key horizontal center top outside");
    line("set ylabel \"Average Points per Game\"");
    line("set yrange [2:11]");
    line("set ytics mirror out 1.0");
    line("set mytics 10");
    line("set y2label \"\"");
    line("set y2range [2:11]");
    line("set y2tics mirror in 1.0");
    line("set my2tics 10");
  }

protected:

  virtual uint_least8_t x_column() const noexcept = 0;

  void plot(const Players& players, const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data) noexcept {
    uint_least64_t counter{0};
    for (const std::pair<PlayerName, std::experimental::filesystem::path>& datum : data) {
      line("  \"" + datum.second.string() + "\" u " + std::to_string(x_column()) + ":7 w lp lw 2 pt " + std::to_string(players.find(datum.first).gnuplot_point_type()) + " ps 1 lt rgb \"#" + players.find(datum.first).color() + "\" t \"" + datum.first.value() + "\" , \\");
      ++counter;
    }
  }

};

class MainAveragePointsVsGameNumberGnuplotFileWriter : public MainAveragePointsGnuplotFileWriter {

public:

  MainAveragePointsVsGameNumberGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const Players& players,
    const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data
  ) noexcept : MainAveragePointsGnuplotFileWriter(path) {
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

class MainAveragePointsVsDateGnuplotFileWriter : public MainAveragePointsGnuplotFileWriter {

public:

  MainAveragePointsVsDateGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const Players& players,
    const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data
  ) noexcept : MainAveragePointsGnuplotFileWriter(path) {
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
