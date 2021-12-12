#include "Instructions.hpp"
#include "Leaderboard.hpp"

int main(int argc, char *argv[]) {
  const CatanLeaderboardGenerator::Instructions instructions(argc, argv);
  const CatanLeaderboardGenerator::TextFileReader file{instructions.games_file()};
  const CatanLeaderboardGenerator::Games games{file};
  const CatanLeaderboardGenerator::Players players{games};
  const CatanLeaderboardGenerator::Leaderboard leaderboard{instructions.leaderboard_directory(), games, players};
  CatanLeaderboardGenerator::message("End of " + CatanLeaderboardGenerator::Program::Title + ".");
  return EXIT_SUCCESS;
}
