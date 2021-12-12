#pragma once

#include "EloRating.hpp"
#include "GnuplotFileWriter.hpp"

namespace CatanLeaderboardGenerator {

class PlayerEloRatingGnuplotFileWriter : public GnuplotFileWriter {

public:

  PlayerEloRatingGnuplotFileWriter(const std::experimental::filesystem::path& path, const EloRating& lowest, const EloRating& highest) noexcept : GnuplotFileWriter(path) {
    const uint_least64_t increment{100};
    const uint_least64_t y_minimum{std::min((uint_least64_t)(EloRatingStartingValue - increment), nearest_lower_nice_number(lowest.value(), increment))};
    const uint_least64_t y_maximum{std::max((uint_least64_t)(EloRatingStartingValue + increment), nearest_higher_nice_number(highest.value(), increment))};
    line("set title \"\"");
    line("set grid xtics ytics mxtics mytics");
    line("set key horizontal center top outside");
    line("set ylabel \"Rating\"");
    line("set yrange [" + std::to_string(y_minimum) + ":" + std::to_string(y_maximum) + "]");
    line("set ytics mirror out");
    line("set mytics 10");
    line("set y2label \"\"");
    line("set y2range [" + std::to_string(y_minimum) + ":" + std::to_string(y_maximum) + "]");
    line("set y2tics mirror in");
    line("set my2tics 10");
  }

protected:

  virtual uint_least8_t x_column() const noexcept = 0;

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
    const uint_least8_t line_width
  ) noexcept {
    const std::map<GameCategory, std::experimental::filesystem::path>::const_iterator found{data.find(game_category)};
    if (found != data.cend()) {
      line("  \"" + found->second.string() + "\" u " + std::to_string(x_column()) + ":7 w lp lw " + std::to_string(line_width) + " pt 7 ps 1 lt rgb \"#" + color + "\" t \"" + label(game_category) + "\" , \\");
    }
  }

};

class PlayerEloRatingVsGameNumberGnuplotFileWriter : public PlayerEloRatingGnuplotFileWriter {

public:

  PlayerEloRatingVsGameNumberGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const std::map<GameCategory, std::experimental::filesystem::path>& data,
    const EloRating& lowest,
    const EloRating& highest
  ) noexcept : PlayerEloRatingGnuplotFileWriter(path, lowest, highest) {
    line("set xlabel \"Game Number\"");
    line("set xtics nomirror out");
    line("set mxtics 1");
    line("plot \\");
    plot(data);
  }

protected:

  uint_least8_t x_column() const noexcept {
    return 3;
  }

};

class PlayerEloRatingVsDateGnuplotFileWriter : public PlayerEloRatingGnuplotFileWriter {

public:

  PlayerEloRatingVsDateGnuplotFileWriter(
    const std::experimental::filesystem::path& path,
    const std::map<GameCategory, std::experimental::filesystem::path>& data,
    const EloRating& lowest,
    const EloRating& highest
  ) noexcept : PlayerEloRatingGnuplotFileWriter(path, lowest, highest) {
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

  uint_least8_t x_column() const noexcept {
    return 5;
  }

};

} // namespace CatanLeaderboardGenerator
