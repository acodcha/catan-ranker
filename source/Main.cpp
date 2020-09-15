#include "../include/GamesFileReader.hpp"
#include "../include/Instructions.hpp"

int main(int argc, char *argv[]) {
  const CatanLeaderboard::Instructions instructions(argc, argv);
  const CatanLeaderboard::GamesFileReader reader{instructions.games_file()};
  const CatanLeaderboard::Games games{reader.data()};
  CatanLeaderboard::message("End of " + CatanLeaderboard::Program::Title + ".");
  return EXIT_SUCCESS;
}
