#pragma once

#include "GnuplotFileWriter.hpp"

namespace CatanLeaderboardGenerator {

class GnuplotAveragePointsFileWriter : public GnuplotFileWriter {

public:

  GnuplotAveragePointsFileWriter(const std::experimental::filesystem::path& path, const std::map<GameCategory, std::experimental::filesystem::path>& data_paths) noexcept : GnuplotFileWriter(path) {
    line("set terminal pngcairo size 800,600 enhanced font \"Verdana,10\"");
    line("set grid xtics ytics mxtics mytics");
    line("set key right bottom inside");
    line("set xlabel \"Date\"");
    line("set ylabel \"Average Points per Game\"");
    line("set xdata time");
    line("set timefmt \"%Y-%m-%d\"");
    line("set format x \"%Y-%m-%d\"");
    line("set xtics rotate by 90 right");
    line("set mxtics 1");
    line("set yrange [2:11]");
    line("set ytics 1.0");
    line("set mytics 5");
    line("set output \"" + png_file_path().string() + "\"");
    line("plot \\");
    const std::map<GameCategory, std::experimental::filesystem::path>::const_iterator found_7_8{data_paths.find(GameCategory::SevenToEightPlayers)};
    if (found_7_8 != data_paths.cend()) {
      line("  \"" + found_7_8->second.string() + "\" u 2:3 w lp lw 2 pt 7 ps 1 lt rgb \"orange\" t \"7-8 Player Games\" , \\");
    }
    const std::map<GameCategory, std::experimental::filesystem::path>::const_iterator found_5_6{data_paths.find(GameCategory::FiveToSixPlayers)};
    if (found_5_6 != data_paths.cend()) {
      line("  \"" + found_5_6->second.string() + "\" u 2:3 w lp lw 2 pt 7 ps 1 lt rgb \"green\" t \"5-6 Player Games\" , \\");
    }
    const std::map<GameCategory, std::experimental::filesystem::path>::const_iterator found_3_4{data_paths.find(GameCategory::ThreeToFourPlayers)};
    if (found_3_4 != data_paths.cend()) {
      line("  \"" + found_3_4->second.string() + "\" u 2:3 w lp lw 2 pt 7 ps 1 lt rgb \"red\" t \"3-4 Player Games\" , \\");
    }
    const std::map<GameCategory, std::experimental::filesystem::path>::const_iterator found_all{data_paths.find(GameCategory::AnyNumberOfPlayers)};
    if (found_all != data_paths.cend()) {
      line("  \"" + found_all->second.string() + "\" u 2:3 w lp lw 2 pt 7 ps 1 lt rgb \"blue\" t \"All Games\" , \\");
    }
  }

};

} // namespace CatanLeaderboardGenerator
