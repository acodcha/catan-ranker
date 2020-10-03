#pragma once

#include "GnuplotFileWriter.hpp"

namespace CatanLeaderboardGenerator {

class GnuplotPlayerPlacePercentageFileWriter : public GnuplotFileWriter {

public:

  GnuplotPlayerPlacePercentageFileWriter(const std::experimental::filesystem::path& path, const GameCategory game_category) noexcept : GnuplotFileWriter(path) {
    line("set terminal pngcairo size 800,600 enhanced font \"Verdana,10\"");
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
    line("set output \"" + png_file_path().string() + "\"");
  }

protected:

  virtual uint_least8_t x_column() const noexcept = 0;

  void plot(const std::experimental::filesystem::path& data) noexcept {
    line("  \"" + data.string() + "\" u " + std::to_string(x_column()) + ":5 w lp lw 4 pt 7 ps 1 lt rgb \"" + Color::YellowWheat + "\" t \"1st Place\" , \\");
    line("  \"" + data.string() + "\" u " + std::to_string(x_column()) + ":6 w lp lw 3 pt 7 ps 1 lt rgb \"" + Color::GreyOre + "\" t \"2nd Place\" , \\");
    line("  \"" + data.string() + "\" u " + std::to_string(x_column()) + ":7 w lp lw 2 pt 7 ps 1 lt rgb \"" + Color::BrownDarkPort + "\" t \"3rd Place\" , \\");
  }

};

class GnuplotPlayerPlacePercentageVsGameNumberFileWriter : public GnuplotPlayerPlacePercentageFileWriter {

public:

  GnuplotPlayerPlacePercentageVsGameNumberFileWriter(
    const std::experimental::filesystem::path& path,
    const std::experimental::filesystem::path& data_path,
    const GameCategory game_category
  ) noexcept : GnuplotPlayerPlacePercentageFileWriter(path, game_category) {
    line("set xlabel \"Game Number\"");
    line("set xtics nomirror out");
    line("set mxtics 1");
    line("plot \\");
    plot(data_path);
  }

protected:

  constexpr uint_least8_t x_column() const noexcept {
    return 2;
  }

};

class GnuplotPlayerPlacePercentageVsDateFileWriter : public GnuplotPlayerPlacePercentageFileWriter {

public:

  GnuplotPlayerPlacePercentageVsDateFileWriter(
    const std::experimental::filesystem::path& path,
    const std::experimental::filesystem::path& data_path,
    const GameCategory game_category
  ) noexcept : GnuplotPlayerPlacePercentageFileWriter(path, game_category) {
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

  constexpr uint_least8_t x_column() const noexcept {
    return 3;
  }

};

} // namespace CatanLeaderboardGenerator
