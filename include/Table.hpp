#pragma once

#include "Date.hpp"
#include "Percentage.hpp"

namespace CatanLeaderboardGenerator {

/// \brief General-purpose column for printing out. All data is stored internally as strings.
class Column {

public:

  enum class Alignment : uint_least8_t {
    Left,
    Center,
    Right
  };

  Column() noexcept {}

  Column(const std::string& header, const Alignment alignment) noexcept : header_(header), alignment_(alignment) {}

  void add_row(const int_least64_t value) noexcept {
    rows_.push_back(std::to_string(value));
  }

  void add_row(const double value, const uint_least8_t significant_digits) noexcept {
    rows_.push_back(real_number_to_string(value, significant_digits));
  }

  void add_row(const Percentage& value, const uint_least8_t decimals) noexcept {
    rows_.push_back(value.print(decimals));
  }

  void add_row(const Date& value) noexcept {
    rows_.push_back(value.print());
  }

  void add_row(const std::string& value) noexcept {
    rows_.push_back(value);
  }

  const std::string& header() const noexcept {
    return header_;
  }

  const std::string header_bold() const noexcept {
    return "**" + header_ + "**";
  }

  const Alignment alignment() const noexcept {
    return alignment_;
  }

  const std::string alignment_markdown() const noexcept {
    switch (alignment_) {
      case Alignment::Left:
        return ":---";
        break;
      case Alignment::Center:
        return ":---:";
        break;
      case Alignment::Right:
        return "---:";
        break;
    }
  }

  const uint_least64_t width() const noexcept {
    uint_least64_t maximum{std::max(header_bold().size(), alignment_markdown().size())};
    for (const std::string& row : rows_) {
      if (maximum < row.size()) {
        maximum = row.size();
      }
    }
    return maximum;
  }

  struct const_iterator : public std::vector<std::string>::const_iterator {
    const_iterator(const std::vector<std::string>::const_iterator i) noexcept : std::vector<std::string>::const_iterator(i) {}
  };

  std::size_t number_of_rows() const noexcept {
    return rows_.size();
  }

  const_iterator cbegin() const noexcept {
   return const_iterator(rows_.cbegin());
  }

  const_iterator begin() const noexcept {
   return cbegin();
  }

  const_iterator cend() const noexcept {
   return const_iterator(rows_.cend());
  }

  const_iterator end() const noexcept {
   return cend();
  }

  std::string operator[](const std::size_t index) const noexcept {
    return rows_[index];
  }

protected:

  std::string header_;

  std::vector<std::string> rows_;

  Alignment alignment_{Alignment::Center};

};

/// \brief General-purpose table. All values are stored internally as strings.
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
    const_iterator(const std::vector<Column>::const_iterator i) noexcept : std::vector<Column>::const_iterator(i) {}
  };

  std::size_t number_of_columns() const noexcept {
    return columns_.size();
  }

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

  const_iterator begin() const noexcept {
   return cbegin();
  }

  const_iterator cend() const noexcept {
   return const_iterator(columns_.cend());
  }

  const_iterator end() const noexcept {
   return cend();
  }

protected:

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
      stream << " " << pad_to_length(column.header_bold(), column.width()) << " |";
    }
    stream << std::endl << "|";
    for (const Column& column : columns_) {
      stream << " " << pad_to_length(column.alignment_markdown(), column.width()) << " |";
    }
    return stream.str();
  }

  std::string print_row_as_markdown(const std::size_t index) const noexcept {
    std::string text{"|"};
    for (const Column& column : columns_) {
      if (index < column.number_of_rows()) {
        text += " " + pad_to_length(column[index], column.width()) + " |";
      } else {
        text += " " + pad_to_length({}, column.width()) + " |";
      }
    }
    return text;
  }

};

} // namespace CatanLeaderboardGenerator
