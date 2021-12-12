#pragma once

#include "GnuplotFileWriter.hpp"

namespace CatanLeaderboardGenerator {

class PlayerPlacePercentageGnuplotFileWriter : public GnuplotFileWriter {

public:

  PlayerPlacePercentageGnuplotFileWriter(const std::experimental::filesystem::path& path, const GameCategory game_category) noexcept : GnuplotFileWriter(path) {
    line("set title \"\"");
    line("set grid xtics ytics mxtics mytics");
    line("set key horizontal center top outside");
    line("set ylabel \"" + label(game_category) + " Place %\"");
    line("set yrange [0:100]");
    line("set ytics mirror out 10.0");
    line("set mytics 10");
    line("set y2label \"\"");
    line("set y2range [0:100]");
    line("set y2tics mirror in 10.0");
    line("set my2tics 10");
  }

protected:

  virtual int8_t x_column() const noexcept = 0;

  void plot(const std::experimental::filesystem::path& data) noexcept {
    line("  \"" + data.string() + "\" u " + std::to_string(x_column()) + ":9 w lp lw 4 pt 7 ps 1 lt rgb \"#" + Color::YellowWheat + "\" t \"1st Place\" , \\");
    line("  \"" + data.string() + "\" u " + std::to_string(x_column()) + ":10 w lp lw 3 pt 7 ps 1 lt rgb \"#" + Color::GreyOre + "\" t \"2nd Place\" , \\");
    line("  \"" + data.string() + "\" u " + std::to_string(x_column()) + ":11 w lp lw 2 pt 7 ps 1 lt rgb \"#" + Color::BrownDarkPort + "\" t \"3rd Place\" , \\");
  }

};

class PlayerPlacePercentageVsGameNumberGnuplotFileWriter : public PlayerPlacePercentageGnuplotFileWriter {

public:

  PlayerPlacePercentageVsGameNumberGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const std::experimental::filesystem::path& data_path,
    const GameCategory game_category
  ) noexcept : PlayerPlacePercentageGnuplotFileWriter(path, game_category) {
    line("set xlabel \"Game Number\"");
    line("set xtics nomirror out");
    line("set mxtics 1");
    line("plot \\");
    plot(data_path);
  }

protected:

  int8_t x_column() const noexcept {
    return 4;
  }

};

class PlayerPlacePercentageVsDateGnuplotFileWriter : public PlayerPlacePercentageGnuplotFileWriter {

public:

  PlayerPlacePercentageVsDateGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const std::experimental::filesystem::path& data_path,
    const GameCategory game_category
  ) noexcept : PlayerPlacePercentageGnuplotFileWriter(path, game_category) {
    line("set timefmt \"%Y-%m-%d\"");
    line("set xlabel \"Date\"");
    line("set xdata time");
    line("set format x \"%Y-%m-%d\"");
    line("set xtics nomirror out rotate by 45 right");
    line("set mxtics 1");
    line("plot \\");
    plot(data_path);
  }

protected:

  int8_t x_column() const noexcept {
    return 5;
  }

};

} // namespace CatanLeaderboardGenerator
