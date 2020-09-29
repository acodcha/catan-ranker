#pragma once

#include "GnuplotFileWriter.hpp"

namespace CatanLeaderboardGenerator {

class GnuplotPlacePercentageFileWriter : public GnuplotFileWriter {

public:

  GnuplotPlacePercentageFileWriter(const std::experimental::filesystem::path& path, const std::experimental::filesystem::path& data_path, const GameCategory game_category) noexcept : GnuplotFileWriter(path) {
    line("set terminal pngcairo size 800,600 enhanced font \"Verdana,10\"");
    line("set title \"\"");
    line("set grid xtics ytics mxtics mytics");
    line("set key horizontal center top outside");
    line("set timefmt \"%Y-%m-%d\"");
    line("set xlabel \"Date\"");
    line("set xdata time");
    line("set format x \"%Y-%m-%d\"");
    line("set xtics nomirror out rotate by 45 right");
    line("set mxtics 1");
    line("set ylabel \"" + label(game_category) + " Place %\"");
    line("set yrange [0:100]");
    line("set ytics mirror out 10.0");
    line("set mytics 5");
    line("set y2label \"\"");
    line("set y2range [0:100]");
    line("set y2tics mirror in 10.0");
    line("set my2tics 5");
    line("set output \"" + png_file_path().string() + "\"");
    line("plot \\");
    line("  \"" + data_path.string() + "\" u 2:($4+$5+$6) w filledcurves x1 fs pattern 2 lt rgb \"black\" t \"1st Place\" , \\");
    line("  \"" + data_path.string() + "\" u 2:($5+$6) w filledcurves x1 fs pattern 1 lt rgb \"black\" t \"2nd Place\" , \\");
    line("  \"" + data_path.string() + "\" u 2:6 w filledcurves x1 fs pattern 4 lt rgb \"black\" t \"3rd Place\" , \\");
  }

};

} // namespace CatanLeaderboardGenerator
