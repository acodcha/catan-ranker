#pragma once

#include "Table.hpp"
#include "TextFileWriter.hpp"

namespace CatanLeaderboardGenerator {

class DataTableFileWriter : public TextFileWriter {

public:

  DataTableFileWriter(const std::experimental::filesystem::path& path, const Table& table) noexcept : TextFileWriter(path) {
    line(table.print_as_data());
    blank_line();
  }

};

} // namespace CatanLeaderboardGenerator
