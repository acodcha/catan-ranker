#pragma once

#include "Base.hpp"

namespace CatanLeaderboard {

class Place {

public:

  constexpr Place() noexcept {}

  constexpr Place(const uint_least8_t value) noexcept : value_(value) {}

  constexpr uint_least8_t value() const noexcept {
    return value_;
  }

  std::string print() const noexcept {
    if (value_ == 1) {
      return "1st";
    } else if (value_ == 2) {
      return "2nd";
    } else if (value_ == 3) {
      return "3rd";
    } else {
      return std::to_string(value_) + "th";
    }
  }

  constexpr bool operator==(const Place other) const noexcept {
    return value_ == other.value_;
  }

  constexpr bool operator!=(const Place other) const noexcept {
    return value_ != other.value_;
  }

  constexpr bool operator<(const Place other) const noexcept {
    return value_ < other.value_;
  }

  constexpr bool operator<=(const Place other) const noexcept {
    return value_ <= other.value_;
  }

  constexpr bool operator>(const Place other) const noexcept {
    return value_ > other.value_;
  }

  constexpr bool operator>=(const Place other) const noexcept {
    return value_ >= other.value_;
  }

  /// \brief Prefix ++ operator.
  Place& operator++() {
    ++value_;
    return *this;
  }

  /// \brief Postfix ++ operator.
  Place operator++(int) {
    Place result(*this);
    ++(*this);
    return result;
  }

  /// \brief Prefix -- operator.
  Place& operator--() {
    --value_;
    return *this;
  }

  /// \brief Postfix -- operator.
  Place operator--(int) {
    Place result(*this);
    --(*this);
    return result;
  }

  struct sort_ascending {
    bool operator()(const Place& place_1, const Place& place_2) const noexcept {
      return place_1.value() < place_2.value();
    }
  };

protected:

  uint_least8_t value_{0};

};

} // namespace CatanLeaderboard

namespace std {

  template <> struct hash<CatanLeaderboard::Place> {

    size_t operator()(const CatanLeaderboard::Place& place) const {
      return hash<uint_least8_t>()(place.value());
    }

  };

} // namespace std
