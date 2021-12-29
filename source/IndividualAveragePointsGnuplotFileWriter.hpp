#pragma once

#include "GnuplotFileWriter.hpp"

namespace CatanRanker {

class IndividualAveragePointsGnuplotFileWriter : public GnuplotFileWriter {

public:

  IndividualAveragePointsGnuplotFileWriter(const std::experimental::filesystem::path& path) noexcept : GnuplotFileWriter(path) {
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

  void plot(const std::map<GameCategory, std::experimental::filesystem::path>& data) noexcept {
    plot(data, GameCategory::AnyNumberOfPlayers,  Color::BlueDarkWater, 4);
    plot(data, GameCategory::ThreeToFourPlayers,  Color::OrangeBrick, 2);
    plot(data, GameCategory::FiveToSixPlayers,    Color::GreenDarkLumber, 2);
    plot(data, GameCategory::SevenToEightPlayers, Color::GreenLightWool, 2);
  }

  void plot(
    const std::map<GameCategory, std::experimental::filesystem::path>& data,
    const GameCategory game_category,
    const std::string& color,
    const int8_t line_width
  ) noexcept {
    const std::map<GameCategory, std::experimental::filesystem::path>::const_iterator found{data.find(game_category)};
    if (found != data.cend()) {
      line("  \"" + found->second.string() + "\" u " + std::to_string(x_column()) + ":8 w lp lw " + std::to_string(line_width) + " pt 0 ps 1 lt rgb \"#" + color + "\" t \"" + label(game_category) + "\" , \\");
    }
  }

};

class IndividualAveragePointsVsGameNumberGnuplotFileWriter : public IndividualAveragePointsGnuplotFileWriter {

public:

  IndividualAveragePointsVsGameNumberGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const std::map<GameCategory, std::experimental::filesystem::path>& data
  ) noexcept : IndividualAveragePointsGnuplotFileWriter(path) {
    line("set xlabel \"Game Number\"");
    line("set xtics nomirror out");
    line("set mxtics 1");
    line("plot \\");
    plot(data);
  }

protected:

  int8_t x_column() const noexcept {
    return 3;
  }

};

class IndividualAveragePointsVsDateGnuplotFileWriter : public IndividualAveragePointsGnuplotFileWriter {

public:

  IndividualAveragePointsVsDateGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const std::map<GameCategory, std::experimental::filesystem::path>& data
  ) noexcept : IndividualAveragePointsGnuplotFileWriter(path) {
    line("set timefmt \"%Y-%m-%d\"");
    line("set xlabel \"Date\"");
    line("set xdata time");
    line("set format x \"%Y-%m-%d\"");
    line("set xtics nomirror out rotate by 45 right");
    line("set mxtics 1");
    line("plot \\");
    plot(data);
  }

protected:

  int8_t x_column() const noexcept {
    return 5;
  }

};

} // namespace CatanRanker
