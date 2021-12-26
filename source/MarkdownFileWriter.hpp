#pragma once

#include "Table.hpp"
#include "TextFileWriter.hpp"

namespace CatanLeaderboardGenerator {

class MarkdownFileWriter : public TextFileWriter {

public:

  MarkdownFileWriter(const std::experimental::filesystem::path& path, const std::string& title) noexcept : TextFileWriter(path) {
    this->title(title);
  }

protected:

  void title(const std::string& text) noexcept {
    if (!text.empty()) {
      line("# " + text);
      blank_line();
    }
  }

  void list_link(const std::string title) noexcept {
    line("- [" + title + "](#" + lowercase(replace_character(title, ' ', '-')) + ")");
  }

  void nested_list_link(const std::string title) noexcept {
    line("    - [" + title + "](#" + lowercase(replace_character(title, ' ', '-')) + ")");
  }

  void link_back_to_section(const std::string title) noexcept {
    blank_line();
    line("[(Back to " + title + ")](#" + lowercase(replace_character(title, ' ', '-')) + ")");
  }

  void link_back_to_top() noexcept {
    blank_line();
    line("[(Back to Top)](#)");
  }

  void link_back_to_main_page() noexcept {
    blank_line();
    line("[(Back to Main Page)](../#)");
  }

  void section(const std::string& title) noexcept {
    blank_line();
    line("## " + title);
    blank_line();
  }

  void subsection(const std::string& title) noexcept {
    blank_line();
    line("### " + title);
    blank_line();
  }

  void subsubsection(const std::string& title) noexcept {
    blank_line();
    line("#### " + title);
    blank_line();
  }

  void table(const Table& table) noexcept {
    line(table.print_as_markdown());
  }

};

} // namespace CatanLeaderboardGenerator
