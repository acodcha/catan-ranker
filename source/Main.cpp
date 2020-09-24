#include "../include/Instructions.hpp"
#include "../include/Players.hpp"

int main(int argc, char *argv[]) {
  const CatanLeaderboard::Instructions instructions(argc, argv);
  const CatanLeaderboard::TextFileReader file{instructions.games_file()};
  const CatanLeaderboard::Games games{file};
  const CatanLeaderboard::Players players{games};
  CatanLeaderboard::message("End of " + CatanLeaderboard::Program::Title + ".");
  return EXIT_SUCCESS;
}
