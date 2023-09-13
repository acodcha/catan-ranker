#pragma once

#include "Base.hpp"

namespace CatanRanker {

class Place {
public:
  constexpr Place() noexcept {}

  constexpr Place(const int8_t value) noexcept : value_(value) {}

  constexpr int8_t value() const noexcept {
    return value_;
  }

  /// \brief The outcome of a game between a pair of players is 1, 0.5, or 0 in
  /// the case of a higher place, an equal place, or a lower place than the
  /// opponent, respectively.
  double actual_outcome(const Place& opponent_place) const noexcept {
    // Note: A lower place is better, e.g. 1st place is better than 2nd place.
    if (value_ < opponent_place.value_) {
      return 1.0;
    } else if (value_ == opponent_place.value_) {
      return 0.5;
    } else {
      return 0.0;
    }
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

  constexpr bool operator==(const Place& other) const noexcept {
    return value_ == other.value_;
  }

  constexpr bool operator!=(const Place& other) const noexcept {
    return value_ != other.value_;
  }

  constexpr bool operator<(const Place& other) const noexcept {
    return value_ < other.value_;
  }

  constexpr bool operator<=(const Place& other) const noexcept {
    return value_ <= other.value_;
  }

  constexpr bool operator>(const Place& other) const noexcept {
    return value_ > other.value_;
  }

  constexpr bool operator>=(const Place& other) const noexcept {
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

  struct sort {
    bool operator()(const Place& place_1, const Place& place_2) const noexcept {
      return place_1.value() < place_2.value();
    }
  };

private:
  int8_t value_{0};
};

const std::set<Place> PlacesFirstSecondThird{{1}, {2}, {3}};

}  // namespace CatanRanker

namespace std {

template <>
struct hash<CatanRanker::Place> {
  size_t operator()(const CatanRanker::Place& place) const {
    return hash<int8_t>()(place.value());
  }
};

}  // namespace std
