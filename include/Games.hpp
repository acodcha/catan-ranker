#pragma once

#include "Game.hpp"

namespace CatanLeaderboard {

class Games {

public:

  Games(const std::vector<Game>& data) noexcept : data_(data) {}

  constexpr const std::vector<Game>& data() const noexcept {
    return data_;
  }

  std::string print() const noexcept {
    if (data_.empty()) {
      return "The games file is empty.";
    }
    std::stringstream stream;
    stream << "There are " << data_.size() << " games:" << std::endl;
    uint_least64_t index{0};
    for (const Game& game : data_) {
      stream << "- " << std::to_string(index + 1) << ": " << game.print();
      if (index + 1 < data_.size()) {
        stream << std::endl;
      }
      ++index;
    }
    return stream.str();
  }

  void message() const noexcept {
    CatanLeaderboard::message(print());
  }

protected:

  std::vector<Game> data_;

};

} // namespace CatanLeaderboard
