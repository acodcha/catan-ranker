#pragma once

#include "File.hpp"

namespace CatanLeaderboardGenerator {

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
