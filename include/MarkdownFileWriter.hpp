#pragma once

#include "Table.hpp"
#include "TextFileWriter.hpp"

namespace CatanLeaderboard {

class MarkdownFileWriter : public TextFileWriter {

public:

  MarkdownFileWriter(const std::experimental::filesystem::path& path, const std::string& title) noexcept : TextFileWriter(path) {
    this->title(title);
  }

protected:

  void title(const std::string& text) noexcept {
    line("# " + text);
  }

  void section(const std::string& title) noexcept {
    blank_line();
    line("## " + title);
  }

  void subsection(const std::string& title) noexcept {
    blank_line();
    line("### " + title);
  }

  void subsubsection(const std::string& title) noexcept {
    blank_line();
    line("#### " + title);
  }

  void table(const Table& table) noexcept {
    line(table.to_markdown());
  }

};

} // namespace CatanLeaderboard
