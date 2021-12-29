#pragma once

#include "Base.hpp"

namespace CatanRanker {

class Points {

public:

  constexpr Points() noexcept {}

  constexpr Points(const int64_t value) noexcept : value_(value) {}

  constexpr int64_t value() const noexcept {
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

  constexpr Points operator+(const int64_t number) const noexcept {
    return {value_ + number};
  }

  constexpr void operator+=(const Points& other) noexcept {
    value_ += other.value_;
  }

  constexpr void operator+=(const int64_t number) noexcept {
    value_ += number;
  }

  constexpr Points operator-(const Points& other) const noexcept {
    return {value_ - other.value_};
  }

  constexpr Points operator-(const int64_t number) const noexcept {
    return {value_ - number};
  }

  constexpr void operator-=(const Points& other) noexcept {
    value_ -= other.value_;
  }

  constexpr void operator-=(const int64_t number) noexcept {
    value_ -= number;
  }

  constexpr Points operator*(const int64_t number) const noexcept {
    return {value_ * number};
  }

  constexpr void operator*=(const int64_t number) noexcept {
    value_ *= number;
  }

  constexpr Points operator/(const int64_t number) const noexcept {
    return {value_ / number};
  }

  constexpr void operator/=(const int64_t number) noexcept {
    value_ /= number;
  }

  struct sort {
    bool operator()(const Points& points_1, const Points& points_2) const noexcept {
      return points_1.value() > points_2.value();
    }
  };

protected:

  int64_t value_{0};

};

constexpr const Points MinimumPoints{2};

constexpr const Points MaximumPoints{50};

} // namespace CatanRanker

namespace std {

  template <> struct hash<CatanRanker::Points> {

    size_t operator()(const CatanRanker::Points& points) const {
      return hash<int64_t>()(points.value());
    }

  };

} // namespace std
