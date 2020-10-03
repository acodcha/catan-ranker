#pragma once

#include "Base.hpp"

namespace CatanLeaderboardGenerator {

class PlayerName {

public:

  PlayerName() noexcept {}

  PlayerName(const std::string& value) noexcept : value_(remove_non_alphabetic_characters(value)) {}

  const std::string& value() const noexcept {
    return value_;
  }

  std::experimental::filesystem::path directory_name() const noexcept {
    return {value_};
  }

  bool operator==(const PlayerName& other) const noexcept {
    return value_ == other.value_;
  }

  bool operator!=(const PlayerName& other) const noexcept {
    return value_ != other.value_;
  }

  struct sort {
    bool operator()(const PlayerName& player_name_1, const PlayerName& player_name_2) const noexcept {
      return player_name_1.value() < player_name_2.value();
    }
  };

protected:

  std::string value_;

};

} // namespace CatanLeaderboardGenerator

namespace std {

  template <> struct hash<CatanLeaderboardGenerator::PlayerName> {

    size_t operator()(const CatanLeaderboardGenerator::PlayerName& player_name) const {
      return hash<string>()(player_name.value());
    }

  };

} // namespace std
