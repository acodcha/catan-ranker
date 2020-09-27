#pragma once

#include "Base.hpp"

namespace CatanLeaderboardGenerator {

/// \brief General-purpose file handler base class.
template <class FileStream> class File {

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

  constexpr File(const std::experimental::filesystem::path& path) : path_(path) {
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

/// \brief General-purpose file reader base class.
class FileReader : public File<std::ifstream> {

protected:

  constexpr FileReader(const std::experimental::filesystem::path& path) noexcept : File(path) {}

};

/// \brief General-purpose file writer base class.
class FileWriter : public File<std::ofstream> {

public:

  ~FileWriter() noexcept {
    set_permissions();
  }

  constexpr const std::experimental::filesystem::perms& permissions() const noexcept {
    return permissions_;
  }

protected:

  constexpr FileWriter(const std::experimental::filesystem::path& path, const std::experimental::filesystem::perms& permissions = {std::experimental::filesystem::perms::owner_read | std::experimental::filesystem::perms::owner_write | std::experimental::filesystem::perms::group_read | std::experimental::filesystem::perms::others_read}) noexcept : File(path), permissions_(permissions) {}

  std::experimental::filesystem::perms permissions_;

  void set_permissions() noexcept {
    if (!path_.empty()) {
      if (std::experimental::filesystem::exists(path_)) {
        std::experimental::filesystem::permissions(path_, permissions_);
      }
    }
  }

};

} // namespace CatanLeaderboardGenerator
