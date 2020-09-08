#pragma once

#include "File.hpp"

namespace CatanLeaderboard {

/// \brief General-purpose file reader base class.
class FileReader : public File<std::ifstream> {

protected:

  constexpr FileReader(const std::experimental::filesystem::path& path) noexcept : File(path) {}

};

} // namespace CatanLeaderboard
