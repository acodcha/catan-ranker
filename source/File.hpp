#pragma once

#include "Base.hpp"

namespace CatanRanker {

/// \brief General-purpose file handler base class.
template<class FileStream> class File {
public:
  ~File() noexcept {
    if (stream_.is_open()) {
      stream_.close();
    }
  }

  constexpr const std::experimental::filesystem::path& path() const noexcept {
    return path_;
  }

protected:
  constexpr File(const std::experimental::filesystem::path& path)
    : path_(path) {
    if (!path_.empty()) {
      stream_.open(path_.string());
      if (!stream_.is_open()) {
        error("Could not open the file: " + path_.string());
      }
    }
  }

  std::experimental::filesystem::path path_;

  FileStream stream_;
};

}  // namespace CatanRanker
