#pragma once

#include "GnuplotFileWriter.hpp"
#include "Players.hpp"

namespace CatanRanker {

class GlobalAveragePointsGnuplotFileWriter : public GnuplotFileWriter {

public:

  GlobalAveragePointsGnuplotFileWriter(const std::experimental::filesystem::path& path) noexcept : GnuplotFileWriter(path) {
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

  virtual int8_t x_column() const noexcept = 0;

  void plot(const Players& players, const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data) noexcept {
    for (const std::pair<PlayerName, std::experimental::filesystem::path>& datum : data) {
      line("  \"" + datum.second.string() + "\" u " + std::to_string(x_column()) + ":8 w lp lw 2 pt " + std::to_string(players.find(datum.first).gnuplot_point_type()) + " ps 1 lt rgb \"#" + players.find(datum.first).color() + "\" t \"" + datum.first.value() + "\" , \\");
    }
  }

};

class GlobalAveragePointsVsGameNumberGnuplotFileWriter : public GlobalAveragePointsGnuplotFileWriter {

public:

  GlobalAveragePointsVsGameNumberGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const Players& players,
    const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data
  ) noexcept : GlobalAveragePointsGnuplotFileWriter(path) {
    line("set xlabel \"Game Number\"");
    line("set xtics nomirror out");
    line("set mxtics 1");
    line("plot \\");
    plot(players, data);
  }

protected:

  int8_t x_column() const noexcept {
    return 2;
  }

};

class GlobalAveragePointsVsDateGnuplotFileWriter : public GlobalAveragePointsGnuplotFileWriter {

public:

  GlobalAveragePointsVsDateGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const Players& players,
    const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data
  ) noexcept : GlobalAveragePointsGnuplotFileWriter(path) {
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

  int8_t x_column() const noexcept {
    return 5;
  }

};

} // namespace CatanRanker
