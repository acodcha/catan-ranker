#include "../include/Instructions.hpp"
#include "../include/Results.hpp"

int main(int argc, char *argv[]) {
  const CatanLeaderboardGenerator::Instructions instructions(argc, argv);
  const CatanLeaderboardGenerator::TextFileReader file{instructions.games_file()};
  const CatanLeaderboardGenerator::Games games{file};
  const CatanLeaderboardGenerator::Players players{games};
  const CatanLeaderboardGenerator::Results results{instructions.results_directory(), players};
  CatanLeaderboardGenerator::message("End of " + CatanLeaderboardGenerator::Program::Title + ".");
  return EXIT_SUCCESS;
}
