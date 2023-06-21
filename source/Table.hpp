#pragma once

#include "Column.hpp"

namespace CatanRanker {

/// \brief General-purpose table for printing out. All values are stored
/// internally as strings.
class Table {
public:
  Table() noexcept {}

  Table(const std::vector<Column>& columns) noexcept : columns_(columns) {}

  std::string print_as_data() const noexcept {
    std::stringstream stream;
    stream << print_header_as_data();
    const std::size_t number_of_rows_{number_of_rows()};
    for (std::size_t row_index = 0; row_index < number_of_rows_; ++row_index) {
      stream << std::endl << print_row_as_data(row_index);
    }
    return stream.str();
  }

  std::string print_as_markdown() const noexcept {
    std::stringstream stream;
    stream << print_header_as_markdown();
    const std::size_t number_of_rows_{number_of_rows()};
    for (std::size_t row_index = 0; row_index < number_of_rows_; ++row_index) {
      stream << std::endl << print_row_as_markdown(row_index);
    }
    return stream.str();
  }

  struct const_iterator : public std::vector<Column>::const_iterator {
    const_iterator(const std::vector<Column>::const_iterator i) noexcept
      : std::vector<Column>::const_iterator(i) {}
  };

  std::size_t number_of_columns() const noexcept { return columns_.size(); }

  std::size_t number_of_rows() const noexcept {
    std::size_t maximum{0};
    for (const Column& column : columns_) {
      if (maximum < column.number_of_rows()) {
        maximum = column.number_of_rows();
      }
    }
    return maximum;
  }

  const_iterator cbegin() const noexcept {
    return const_iterator(columns_.cbegin());
  }

  const_iterator begin() const noexcept { return cbegin(); }

  const_iterator cend() const noexcept {
    return const_iterator(columns_.cend());
  }

  const_iterator end() const noexcept { return cend(); }

private:
  std::vector<Column> columns_;

  std::string print_header_as_data() const noexcept {
    std::string text{"#"};
    for (const Column& column : columns_) {
      text += column.header() + " ";
    }
    return text;
  }

  std::string print_row_as_data(const std::size_t index) const noexcept {
    std::string text;
    for (const Column& column : columns_) {
      if (index < column.number_of_rows()) {
        text += column[index] + " ";
      } else {
        text += " ";
      }
    }
    return text;
  }

  std::string print_header_as_markdown() const noexcept {
    std::stringstream stream;
    stream << "|";
    for (const Column& column : columns_) {
      stream << " "
             << pad_to_length(column.header_bold(), column.width_markdown())
             << " |";
    }
    stream << std::endl << "|";
    for (const Column& column : columns_) {
      stream
          << " "
          << pad_to_length(column.alignment_markdown(), column.width_markdown())
          << " |";
    }
    return stream.str();
  }

  std::string print_row_as_markdown(const std::size_t index) const noexcept {
    std::string text{"|"};
    for (const Column& column : columns_) {
      if (index < column.number_of_rows()) {
        text +=
            " " + pad_to_length(column[index], column.width_markdown()) + " |";
      } else {
        text += " " + pad_to_length({}, column.width_markdown()) + " |";
      }
    }
    return text;
  }
};

}  // namespace CatanRanker
