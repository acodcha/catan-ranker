#pragma once

#include "Base.hpp"

namespace CatanLeaderboardGenerator {

class Points {

public:

  constexpr Points() noexcept {}

  constexpr Points(const int_least64_t value) noexcept : value_(value) {}

  constexpr int_least64_t value() const noexcept {
    return value_;
  }

  std::string print() const noexcept {
    return std::to_string(value_);
  }

  constexpr bool operator==(const Points other) const noexcept {
    return value_ == other.value_;
  }

  constexpr bool operator!=(const Points other) const noexcept {
    return value_ != other.value_;
  }

  constexpr bool operator<(const Points other) const noexcept {
    return value_ < other.value_;
  }

  constexpr bool operator<=(const Points other) const noexcept {
    return value_ <= other.value_;
  }

  constexpr bool operator>(const Points other) const noexcept {
    return value_ > other.value_;
  }

  constexpr bool operator>=(const Points other) const noexcept {
    return value_ >= other.value_;
  }

  constexpr Points operator+(const Points& other) const noexcept {
    return {value_ + other.value_};
  }

  constexpr Points operator+(const int_least64_t number) const noexcept {
    return {value_ + number};
  }

  constexpr void operator+=(const Points& other) noexcept {
    value_ += other.value_;
  }

  constexpr void operator+=(const int_least64_t number) noexcept {
    value_ += number;
  }

  constexpr Points operator-(const Points& other) const noexcept {
    return {value_ - other.value_};
  }

  constexpr Points operator-(const int_least64_t number) const noexcept {
    return {value_ - number};
  }

  constexpr void operator-=(const Points& other) noexcept {
    value_ -= other.value_;
  }

  constexpr void operator-=(const int_least64_t number) noexcept {
    value_ -= number;
  }

  constexpr Points operator*(const int_least64_t number) const noexcept {
    return {value_ * number};
  }

  constexpr void operator*=(const int_least64_t number) noexcept {
    value_ *= number;
  }

  constexpr Points operator/(const int_least64_t number) const noexcept {
    return {value_ / number};
  }

  constexpr void operator/=(const int_least64_t number) noexcept {
    value_ /= number;
  }

  struct sort {
    bool operator()(const Points& points_1, const Points& points_2) const noexcept {
      return points_1.value() > points_2.value();
    }
  };

protected:

  int_least64_t value_{0};

};

} // namespace CatanLeaderboardGenerator

namespace std {

  template <> struct hash<CatanLeaderboardGenerator::Points> {

    size_t operator()(const CatanLeaderboardGenerator::Points& points) const {
      return hash<int_least64_t>()(points.value());
    }

  };

} // namespace std
