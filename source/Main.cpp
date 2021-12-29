#include "Instructions.hpp"
#include "Leaderboard.hpp"

int main(int argc, char *argv[]) {
  const catan_stratification::Instructions instructions(argc, argv);
  const catan_stratification::TextFileReader file{instructions.games_file()};
  const catan_stratification::Games games{file};
  const catan_stratification::Players players{games};
  const catan_stratification::Leaderboard leaderboard{instructions.leaderboard_directory(), games, players};
  catan_stratification::message("End of " + catan_stratification::Program::Title + ".");
  return EXIT_SUCCESS;
}
