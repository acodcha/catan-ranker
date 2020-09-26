#pragma once

#include "Base.hpp"

namespace CatanLeaderboard {

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

  Column(const std::string& header, const std::vector<int_least64_t>& rows, const Alignment alignment) noexcept : header_(header), alignment_(alignment) {
    for (const int_least64_t row : rows) {
      add_row(row);
    }
  }

  template <RealNumber Type> Column(const std::string& header, const std::vector<double>& rows, const Alignment alignment, const uint_least8_t significant_digits = 7) noexcept : header_(header), alignment_(alignment) {
    for (const double row : rows) {
      add_row<Type>(row, significant_digits);
    }
  }

  Column(const std::string& header, const std::vector<std::string>& rows, const Alignment alignment) noexcept : header_(header), alignment_(alignment) {
    for (const std::string row : rows) {
      add_row(row);
    }
  }

  void add_row(const int_least64_t value) noexcept {
    rows_.push_back(std::to_string(value));
  }

  template <RealNumber Type> void add_row(const double value, const uint_least8_t significant_digits = 7) noexcept;

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

template <> void Column::add_row<RealNumber::FloatingPoint>(const double value, const uint_least8_t significant_digits) noexcept {
  rows_.push_back(real_number_to_string(value, significant_digits));
}

template <> void Column::add_row<RealNumber::Percentage>(const double value, const uint_least8_t significant_digits) noexcept {
  rows_.push_back(real_number_to_percentage_string(value));
}

/// \brief General-purpose table. All values are stored internally as strings.
class Table {

public:

  Table() noexcept {}

  Table(const std::vector<Column>& columns) noexcept : columns_(columns) {}

  std::string to_markdown() const noexcept {
    std::stringstream stream;
    stream << markdown_header();
    const std::size_t number_of_rows_{number_of_rows()};
    for (std::size_t row_index = 0; row_index < number_of_rows_; ++row_index) {
      stream << std::endl << markdown_row(row_index);
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

  std::string markdown_header() const noexcept {
    std::stringstream stream;
    stream << "|";
    for (const Column& column : columns_) {
      stream << " " + pad_to_length(column.header_bold(), column.width()) << " |";
    }
    stream << std::endl << "|";
    for (const Column& column : columns_) {
      stream << " " + pad_to_length(column.alignment_markdown(), column.width()) << " |";
    }
    return stream.str();
  }

  std::string markdown_row(const std::size_t index) const noexcept {
    std::string row_text{"|"};
    for (const Column& column : columns_) {
      if (index < column.number_of_rows()) {
        row_text += " " + pad_to_length(column[index], column.width()) + " |";
      } else {
        row_text += " " + pad_to_length({}, column.width()) + " |";
      }
    }
    return row_text;
  }

};

} // namespace CatanLeaderboard
