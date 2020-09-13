#include "../include/Game.hpp"
#include "../include/Instructions.hpp"

int main(int argc, char *argv[]) {
  const CatanLeaderboard::Instructions instructions(argc, argv);
  CatanLeaderboard::message("End of " + CatanLeaderboard::Program::Title + ".");
  return EXIT_SUCCESS;
}
