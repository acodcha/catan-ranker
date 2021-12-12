#pragma once

#include "Include.hpp"

namespace CatanLeaderboardGenerator {

/// \brief Namespace containing colors from the Catan board game.
namespace Color {

const std::string BlackNumberFont{"004405"};

const std::string BlueDarkWater{"0866A5"};

const std::string BlueLightWater{"7FCFF5"};

const std::string BrownDarkPort{"5B2A2A"};

const std::string BrownLightDesert{"D9D295"};

const std::string GreenDarkLumber{"1F9E34"};

const std::string GreenLightWool{"A3B801"};

const std::string GreyOre{"B5BBB7"};

const std::string OrangeBrick{"EA772F"};

const std::string Pink{"FF7FED"};

const std::string PurpleDevelopmentCard{"9066BA"};

const std::string RedNumberFont{"CC0000"};

const std::string TanCoast{"E0B568"};

const std::string YellowWheat{"FEC53E"};

} // namespace Color

const std::vector<std::string> ColorSequence{
  Color::OrangeBrick,
  Color::GreenDarkLumber,
  Color::YellowWheat,
  Color::GreenLightWool,
  Color::GreyOre,
  Color::BlueDarkWater,
  Color::BrownLightDesert,
  Color::PurpleDevelopmentCard,
  Color::BlueLightWater,
  Color::TanCoast,
  Color::BrownDarkPort,
  Color::RedNumberFont,
  Color::BlackNumberFont,
  Color::Pink
};

std::string color(const uint64_t counter) noexcept {
  const std::size_t color_index{counter % ColorSequence.size()};
  return ColorSequence[color_index];
}

const std::vector<uint8_t> GnuplotPointTypeSequence{7, 5, 9, 13, 11, 2};

uint8_t gnuplot_point_type(const uint64_t counter) noexcept {
  const std::size_t gnuplot_point_type_index{static_cast<uint64_t>(std::floor(counter / ColorSequence.size())) % GnuplotPointTypeSequence.size()};
  return GnuplotPointTypeSequence[gnuplot_point_type_index];
}

} // namespace CatanLeaderboardGenerator
