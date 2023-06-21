#include "Instructions.hpp"
#include "Leaderboard.hpp"

int main(int argc, char* argv[]) {
  const CatanRanker::Instructions instructions(argc, argv);
  const CatanRanker::TextFileReader file{instructions.games_file()};
  const CatanRanker::Games games{file};
  const CatanRanker::Players players{games};
  const CatanRanker::Leaderboard leaderboard{
      instructions.leaderboard_directory(), games, players};
  CatanRanker::message("End of " + CatanRanker::Program::Title + ".");
  return EXIT_SUCCESS;
}
