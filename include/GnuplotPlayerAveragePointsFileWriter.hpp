#pragma once

#include "GnuplotFileWriter.hpp"

namespace CatanLeaderboardGenerator {

class GnuplotPlayerAveragePointsFileWriter : public GnuplotFileWriter {

public:

  GnuplotPlayerAveragePointsFileWriter(const std::experimental::filesystem::path& path, const std::map<GameCategory, std::experimental::filesystem::path>& data_paths) noexcept : GnuplotFileWriter(path) {
    line("set terminal pngcairo size 800,600 enhanced font \"Verdana,10\"");
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
    line("set output \"" + png_file_path().string() + "\"");
  }

protected:

  void plots(const std::map<GameCategory, std::experimental::filesystem::path>& data_paths, const uint_least8_t x_column) noexcept {
    plot(data_paths, GameCategory::AnyNumberOfPlayers, x_column, Color::BlueDarkWater, 10);
    plot(data_paths, GameCategory::ThreeToFourPlayers, x_column, Color::OrangeBrick, 8);
    plot(data_paths, GameCategory::FiveToSixPlayers, x_column, Color::GreenDarkLumber, 6);
    plot(data_paths, GameCategory::SevenToEightPlayers, x_column, Color::GreenLightWool, 4);
  }

  void plot(const std::map<GameCategory, std::experimental::filesystem::path>& data_paths, const GameCategory game_category, const uint_least8_t x_column, const std::string& color, const uint_least8_t line_width) noexcept {
    const std::map<GameCategory, std::experimental::filesystem::path>::const_iterator found_7_8{data_paths.find(game_category)};
    if (found_7_8 != data_paths.cend()) {
      line("  \"" + found_7_8->second.string() + "\" u " + std::to_string(x_column) + ":3 w lp lw " + std::to_string(line_width) + " pt 7 ps 1 lt rgb \"" + color + "\" t \"" + label(game_category) + "\" , \\");
    }
  }

};

class GnuplotPlayerAveragePointsVsGameNumberFileWriter : public GnuplotPlayerAveragePointsFileWriter {

public:

  GnuplotPlayerAveragePointsVsGameNumberFileWriter(const std::experimental::filesystem::path& path, const std::map<GameCategory, std::experimental::filesystem::path>& data_paths) noexcept : GnuplotPlayerAveragePointsFileWriter(path, data_paths) {
    line("set xlabel \"Game Number\"");
    line("set xtics nomirror out");
    line("set mxtics 1");
    line("plot \\");
    plots(data_paths, 1);
  }

};

class GnuplotPlayerAveragePointsVsDateFileWriter : public GnuplotPlayerAveragePointsFileWriter {

public:

  GnuplotPlayerAveragePointsVsDateFileWriter(const std::experimental::filesystem::path& path, const std::map<GameCategory, std::experimental::filesystem::path>& data_paths) noexcept : GnuplotPlayerAveragePointsFileWriter(path, data_paths) {
    line("set timefmt \"%Y-%m-%d\"");
    line("set xlabel \"Date\"");
    line("set xdata time");
    line("set format x \"%Y-%m-%d\"");
    line("set xtics nomirror out rotate by 45 right");
    line("set mxtics 1");
    line("plot \\");
    plots(data_paths, 2);
  }

};

} // namespace CatanLeaderboardGenerator
