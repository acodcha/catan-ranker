#pragma once

#include "Base.hpp"

namespace CatanLeaderboard {

class PlayerName {

public:

  PlayerName() noexcept {}

  PlayerName(const std::string& value) noexcept : value_(remove_non_alphabetic_characters(value)) {}

  const std::string& value() const noexcept {
    return value_;
  }

  bool operator==(const PlayerName& other) const noexcept {
    return value_ == other.value_;
  }

  bool operator!=(const PlayerName& other) const noexcept {
    return value_ != other.value_;
  }

  struct sort_alphabetical {
    bool operator()(const PlayerName& player_name_1, const PlayerName& player_name_2) const noexcept {
      return player_name_1.value() < player_name_2.value();
    }
  };

  struct hash {
    std::size_t operator()(const PlayerName& player_name) const {
      return std::hash<std::string>()(player_name.value());
    }
  };

protected:

  std::string value_;

};

} // namespace CatanLeaderboard
