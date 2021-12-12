#pragma once

#include "Base.hpp"

namespace CatanLeaderboardGenerator {

/// \brief Year-month-day date class.
/// \details C++20 will have a std::chrono::year_month_day class. Until then, use this class.
class Date {

public:

  constexpr Date() noexcept {}

  constexpr Date(const uint_least16_t year, const uint_least8_t month_number, const uint_least8_t day_number) noexcept : year_(year), month_number_(month_number), day_number_(day_number) {}

  /// \brief Constructor from a YYYY-MM-DD string, such as "2020-03-15".
  Date(const std::string& year_month_day) {
    const std::vector<std::string> year_month_day_vector{split(remove_whitespace(year_month_day), '-')};
    const std::string error_message{"The date '" + year_month_day + "' is not a valid date in the YYYY-MM-DD format."};
    if (year_month_day_vector.size() != 3) {
      error(error_message);
    }
    const std::optional<uint_least64_t> year{string_to_natural_number(year_month_day_vector[0])};
    if (year.has_value() && year.value() > 0) {
      year_ = year.value();
    } else {
      error(error_message);
    }
    const std::optional<uint_least64_t> month_number{string_to_natural_number(year_month_day_vector[1])};
    if (month_number.has_value() && month_number.value() >= 1 && month_number.value() <= 12) {
      month_number_ = month_number.value();
    } else {
      error(error_message);
    }
    const std::optional<uint_least64_t> day_number{string_to_natural_number(year_month_day_vector[2])};
    if (day_number.has_value() && day_number.value() >= 1 && day_number.value() <= 31) {
      day_number_ = day_number.value();
    } else {
      error(error_message);
    }
  }

  constexpr uint_least16_t year() const noexcept {
    return year_;
  }

  constexpr uint_least8_t month_number() const noexcept {
    return month_number_;
  }

  constexpr uint_least8_t day_number() const noexcept {
    return day_number_;
  }

  std::string print() const noexcept {
    const std::string month_number{month_number_ < 10 ? "0" + std::to_string(month_number_) : std::to_string(month_number_)};
    const std::string day_number{day_number_ < 10 ? "0" + std::to_string(day_number_) : std::to_string(day_number_)};
    return std::to_string(year_) + "-" + month_number + "-" + day_number;
  }

  bool operator==(const Date& other) const noexcept {
    return year_ == other.year_ && month_number_ == other.month_number_ && day_number_ == other.day_number_;
  }

  bool operator!=(const Date& other) const noexcept {
    return year_ != other.year_ || month_number_ != other.month_number_ || day_number_ != other.day_number_;
  }

  bool operator<(const Date& other) const noexcept {
    if (year_ < other.year_) {
      return true;
    } else if (year_ > other.year_) {
      return false;
    } else {
      if (month_number_ < other.month_number_) {
        return true;
      } else if (month_number_ > other.month_number_) {
        return false;
      } else {
        return day_number_ < other.day_number_;
      }
    }
  }

  bool operator<=(const Date& other) const noexcept {
    if (year_ < other.year_) {
      return true;
    } else if (year_ > other.year_) {
      return false;
    } else {
      if (month_number_ < other.month_number_) {
        return true;
      } else if (month_number_ > other.month_number_) {
        return false;
      } else {
        return day_number_ <= other.day_number_;
      }
    }
  }

  bool operator>(const Date& other) const noexcept {
    if (year_ < other.year_) {
      return false;
    } else if (year_ > other.year_) {
      return true;
    } else {
      if (month_number_ < other.month_number_) {
        return false;
      } else if (month_number_ > other.month_number_) {
        return true;
      } else {
        return day_number_ > other.day_number_;
      }
    }
  }

  bool operator>=(const Date& other) const noexcept {
    if (year_ < other.year_) {
      return false;
    } else if (year_ > other.year_) {
      return true;
    } else {
      if (month_number_ < other.month_number_) {
        return false;
      } else if (month_number_ > other.month_number_) {
        return true;
      } else {
        return day_number_ >= other.day_number_;
      }
    }
  }

protected:

  /// \brief CE year, e.g. 2020.
  uint_least16_t year_{0};

  /// \brief Month number. Ranges from 1 to 12, where 1 is January and 12 is December.
  uint_least8_t month_number_{0};

  /// \brief Day number within a month. 1 is the 1st of the month.
  uint_least8_t day_number_{0};

};

} // namespace CatanLeaderboardGenerator
