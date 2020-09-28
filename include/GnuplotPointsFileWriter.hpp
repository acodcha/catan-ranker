#pragma once

#include "GnuplotFileWriter.hpp"

namespace CatanLeaderboardGenerator {

class GnuplotPointsFileWriter : public GnuplotFileWriter {

public:

  GnuplotPointsFileWriter(const std::experimental::filesystem::path& path, const std::string& title) noexcept : GnuplotFileWriter(path, title, "Date", "Average Points per Game") {
    line("set terminal pngcairo size 800,600 enhanced font \"Verdana,10\"");
    line("set grid xtics ytics mxtics mytics");
    line("set xdata time");
    line("set timefmt \"%Y-%m-%d\"");
    line("set format x \"%Y-%m-%d\"");
    line("set xtics rotate by 90 right");
    line("set mxtics 1");
    line("set yrange [2:11]");
    line("set ytics 1.0");
    line("set mytics 5");
    line("set output \"" + png_file_path().string() + "\"");
    line("plot \"" + data_table_file_path().string() + "\" u 2:3 w lp lw 2 pt 7 ps 1 lt rgb \"black\" t \"\"");
  }

};

} // namespace CatanLeaderboardGenerator
