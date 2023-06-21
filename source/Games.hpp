#pragma once

#include "Game.hpp"
#include "TextFileReader.hpp"

namespace CatanRanker {

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
    std::map<GameCategory, int64_t> game_category_game_index{
        {GameCategory::AnyNumberOfPlayers,  0},
        {GameCategory::ThreeToFourPlayers,  0},
        {GameCategory::FiveToSixPlayers,    0},
        {GameCategory::SevenToEightPlayers, 0}
    };
    for (Game& game : data_) {
      game.set_indices(
          game_category_game_index[GameCategory::AnyNumberOfPlayers],
          game_category_game_index[game.category()]);
      ++game_category_game_index[GameCategory::AnyNumberOfPlayers];
      ++game_category_game_index[game.category()];
    }
    message(print());
  }

  std::string print() const noexcept {
    if (data_.empty()) {
      return "The games file is empty.";
    }
    std::stringstream stream;
    stream << "There are " << data_.size() << " games:";
    for (const Game& game : data_) {
      stream << std::endl
             << "- " << std::to_string(game.number()) << ": " << game.print();
    }
    return stream.str();
  }

  struct const_iterator : public std::vector<Game>::const_iterator {
    const_iterator(const std::vector<Game>::const_iterator i) noexcept
      : std::vector<Game>::const_iterator(i) {}
  };

  struct const_reverse_iterator
    : public std::vector<Game>::const_reverse_iterator {
    const_reverse_iterator(
        const std::vector<Game>::const_reverse_iterator i) noexcept
      : std::vector<Game>::const_reverse_iterator(i) {}
  };

  std::size_t size() const noexcept { return data_.size(); }

  const_iterator cbegin() const noexcept {
    return const_iterator(data_.cbegin());
  }

  const_iterator begin() const noexcept { return cbegin(); }

  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(data_.crbegin());
  }

  const_reverse_iterator rbegin() const noexcept { return crbegin(); }

  const_iterator cend() const noexcept { return const_iterator(data_.cend()); }

  const_iterator end() const noexcept { return cend(); }

  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(data_.crend());
  }

  const_reverse_iterator rend() const noexcept { return crend(); }

private:
  std::vector<Game> data_;
};

}  // namespace CatanRanker
