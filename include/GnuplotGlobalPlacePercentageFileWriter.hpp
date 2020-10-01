#pragma once

#include "GnuplotFileWriter.hpp"
#include "Place.hpp"
#include "PlayerName.hpp"

namespace CatanLeaderboardGenerator {

class GnuplotGlobalPlacePercentageFileWriter : public GnuplotFileWriter {

public:

  GnuplotGlobalPlacePercentageFileWriter(
    const std::experimental::filesystem::path& path,
    const GameCategory game_category,
    const Place& place
  ) noexcept : GnuplotFileWriter(path) {
    line("set terminal pngcairo size 800,600 enhanced font \"Verdana,10\"");
    line("set title \"\"");
    line("set grid xtics ytics mxtics mytics");
    line("set key horizontal center top outside");
    line("set ylabel \"" + label(game_category) + " " + place.print() + "Place %\"");
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

  virtual constexpr uint_least8_t x_column() const noexcept = 0;

  void plot(const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data) noexcept {
    uint_least64_t counter{0};
    for (const std::pair<PlayerName, std::experimental::filesystem::path>& datum : data) {
      const uint_least64_t color_index{counter % ColorSequence.size()};
      const uint_least64_t point_type_index{(uint_least64_t)std::floor(counter / ColorSequence.size()) % GnuplotPointTypeSequence.size()};
      line("  \"" + datum.second.string() + "\" u " + std::to_string(x_column()) + ":5 w lp lw 2 pt " + std::to_string(GnuplotPointTypeSequence[point_type_index]) + " ps 1 lt rgb \"" + ColorSequence[color_index] + "\" t \"" + datum.first.value() + "\" , \\");
      ++counter;
    }
  }

};

class GnuplotGlobalPlacePercentageVsGameNumberFileWriter : public GnuplotGlobalPlacePercentageFileWriter {

public:

  GnuplotGlobalPlacePercentageVsGameNumberFileWriter(
    const std::experimental::filesystem::path& path,
    const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data,
    const GameCategory game_category,
    const Place& place
  ) noexcept : GnuplotGlobalPlacePercentageFileWriter(path, game_category, place) {
    line("set xlabel \"Game Number\"");
    line("set xtics nomirror out");
    line("set mxtics 1");
    line("plot \\");
    plot(data);
  }

protected:

  constexpr uint_least8_t x_column() const noexcept {
    return 1;
  }

};

class GnuplotGlobalPlacePercentageVsDateFileWriter : public GnuplotGlobalPlacePercentageFileWriter {

public:

  GnuplotGlobalPlacePercentageVsDateFileWriter(
    const std::experimental::filesystem::path& path,
    const std::map<PlayerName, std::experimental::filesystem::path, PlayerName::sort>& data,
    const GameCategory game_category,
    const Place& place
  ) noexcept : GnuplotGlobalPlacePercentageFileWriter(path, game_category, place) {
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

  constexpr uint_least8_t x_column() const noexcept {
    return 3;
  }

};

} // namespace CatanLeaderboardGenerator
