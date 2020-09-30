#pragma once

#include "Game.hpp"
#include "TextFileReader.hpp"

namespace CatanLeaderboardGenerator {

class Games {

public:

  Games(const TextFileReader& file) noexcept {
    message("Reading the games file...");
    for (const std::string& line : file) {
      if (!remove_whitespace(line).empty()) {
        data_.emplace_back(line);
      }
    }
    std::sort(data_.begin(), data_.end(), Game::sort());
    message(print());
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

  struct const_iterator : public std::vector<Game>::const_iterator {
    const_iterator(const std::vector<Game>::const_iterator i) noexcept : std::vector<Game>::const_iterator(i) {}
  };

  struct const_reverse_iterator : public std::vector<Game>::const_reverse_iterator {
    const_reverse_iterator(const std::vector<Game>::const_reverse_iterator i) noexcept : std::vector<Game>::const_reverse_iterator(i) {}
  };

  std::size_t size() const noexcept {
    return data_.size();
  }

  const_iterator cbegin() const noexcept {
   return const_iterator(data_.cbegin());
  }

  const_iterator begin() const noexcept {
   return cbegin();
  }

  const_reverse_iterator crbegin() const noexcept {
   return const_reverse_iterator(data_.crbegin());
  }

  const_reverse_iterator rbegin() const noexcept {
   return crbegin();
  }

  const_iterator cend() const noexcept {
   return const_iterator(data_.cend());
  }

  const_iterator end() const noexcept {
   return cend();
  }

  const_reverse_iterator crend() const noexcept {
   return const_reverse_iterator(data_.crend());
  }

  const_reverse_iterator rend() const noexcept {
   return crend();
  }

protected:

  std::vector<Game> data_;

};

} // namespace CatanLeaderboardGenerator
