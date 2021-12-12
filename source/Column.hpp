#pragma once

#include "EloRating.hpp"
#include "Percentage.hpp"

namespace CatanLeaderboardGenerator {

/// \brief General-purpose column of a table for printing out. All data is stored internally as strings.
class Column {

public:

  enum class Alignment : uint8_t {
    Left,
    Center,
    Right
  };

  Column(const std::string& header, const Alignment alignment = Alignment::Left) noexcept : header_(header), alignment_(alignment) {}

  void add_row(const int64_t value) noexcept {
    rows_.push_back(std::to_string(value));
  }

  void add_row(const double value, const uint8_t significant_digits) noexcept {
    rows_.push_back(real_number_to_string(value, significant_digits));
  }

  void add_row(const Percentage& value, const uint8_t decimals) noexcept {
    rows_.push_back(value.print(decimals));
  }

  void add_row(const Date& value) noexcept {
    rows_.push_back(value.print());
  }

  void add_row(const EloRating& value) noexcept {
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

  const uint64_t width_markdown() const noexcept {
    uint64_t maximum{std::max(header_bold().size(), alignment_markdown().size())};
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

  Alignment alignment_{Alignment::Left};

};

} // namespace CatanLeaderboardGenerator
