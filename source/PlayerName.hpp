#pragma once

#include "Path.hpp"

namespace catan_stratification {

class PlayerName {

public:

  PlayerName() noexcept {}

  PlayerName(const std::string& value) noexcept : value_(remove_non_alphabetic_characters(value)) {}

  const std::string& value() const noexcept {
    return value_;
  }

  std::experimental::filesystem::path directory_name() const noexcept {
    return Path::PlayersDirectoryName / std::experimental::filesystem::path{value_};
  }

  bool operator==(const PlayerName& other) const noexcept {
    return value_ == other.value_;
  }

  bool operator!=(const PlayerName& other) const noexcept {
    return value_ != other.value_;
  }

  bool operator<(const PlayerName& other) const noexcept {
    return value_ < other.value_;
  }

  bool operator<=(const PlayerName& other) const noexcept {
    return value_ <= other.value_;
  }

  bool operator>(const PlayerName& other) const noexcept {
    return value_ > other.value_;
  }

  bool operator>=(const PlayerName& other) const noexcept {
    return value_ >= other.value_;
  }

  struct sort {
    bool operator()(const PlayerName& player_name_1, const PlayerName& player_name_2) const noexcept {
      return player_name_1.value() < player_name_2.value();
    }
  };

protected:

  std::string value_;

};

} // namespace catan_stratification

namespace std {

  template <> struct hash<catan_stratification::PlayerName> {

    size_t operator()(const catan_stratification::PlayerName& player_name) const {
      return hash<string>()(player_name.value());
    }

  };

} // namespace std
