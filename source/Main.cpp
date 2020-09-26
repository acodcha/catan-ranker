#include "../include/Instructions.hpp"
#include "../include/Results.hpp"

int main(int argc, char *argv[]) {
  const CatanLeaderboard::Instructions instructions(argc, argv);
  const CatanLeaderboard::TextFileReader file{instructions.games_file()};
  const CatanLeaderboard::Games games{file};
  const CatanLeaderboard::Players players{games};
  const CatanLeaderboard::Results results{instructions.results_prefix(), games, players};
  CatanLeaderboard::message("End of " + CatanLeaderboard::Program::Title + ".");
  return EXIT_SUCCESS;
}
