#pragma once

#include "Base.hpp"

namespace CatanLeaderboard {

/// \brief Namespace containing information about the program and its compilation.
namespace Program {

const std::string Title{"Catan Leaderboard"};

const std::string CompilationDateAndTime{std::string{__DATE__} + ", " + std::string{__TIME__}};

const std::string Description{"Leaderboard for Catan games with friends."};

} // namespace Program

/// \brief Namespace listing the program's command-line argument keywords.
namespace Arguments {

const std::string UsageInformation{"--help"};

const std::string GameDataFileKey{"--data"};

const std::string GameDataFilePattern{GameDataFileKey + " <path>"};

const std::string ResultsPrefixKey{"--results"};

const std::string ResultsPrefixPattern{ResultsPrefixKey + " <path>"};

} // namespace Arguments

/// \brief Parser and organizer of the program's command-line arguments.
class Instructions {

public:

  Instructions(int argc, char *argv[]) noexcept : executable_name_(argv[0]) {
    assign_arguments(argc, argv);
    initialize();
    message_header_information();
    message_command();
    message_start_information();
    check();
  }

  constexpr const std::experimental::filesystem::path& game_data_file() const noexcept {
    return game_data_file_;
  }

  constexpr const std::experimental::filesystem::path& results_prefix() const noexcept {
    return results_prefix_;
  }

protected:

  std::string executable_name_;

  std::vector<std::string> arguments_;

  std::experimental::filesystem::path game_data_file_;

  std::experimental::filesystem::path results_prefix_;

  void assign_arguments(int argc, char *argv[]) noexcept {
    if (argc > 1) {
      arguments_.assign(argv + 1, argv + argc);
    } else {
      message_header_information();
      message_usage_information();
      exit(EXIT_SUCCESS);
    }
  }

  void initialize() noexcept {
    for (std::vector<std::string>::const_iterator argument = arguments_.cbegin(); argument < arguments_.cend(); ++argument) {
      if (*argument == Arguments::UsageInformation) {
        message_header_information();
        message_usage_information();
        exit(EXIT_SUCCESS);
      } else if (*argument == Arguments::GameDataFileKey && argument + 1 < arguments_.cend()) {
        game_data_file_ = {*(argument + 1)};
      } else if (*argument == Arguments::ResultsPrefixKey && argument + 1 < arguments_.cend()) {
        results_prefix_ = {*(argument + 1)};
      }
    }
  }

  void message_header_information() const noexcept {
    message(Program::Title);
    message(Program::Description);
    message("Version: " + Program::CompilationDateAndTime);
  }

  void message_usage_information() const noexcept {
    const std::string space{"  "};
    message("Usage:");
    message(space + executable_name_ + " " + Arguments::GameDataFilePattern + " " + Arguments::ResultsPrefixPattern);
    const uint_least64_t length{std::max({
      Arguments::UsageInformation.length(),
      Arguments::GameDataFilePattern.length(),
      Arguments::ResultsPrefixPattern.length()
    })};
    message("Arguments:");
    message(space + pad_to_length(Arguments::UsageInformation, length) + space + "Displays this information and exits.");
    message(space + pad_to_length(Arguments::GameDataFilePattern, length) + space + "Path to the game data file to be read. Required.");
    message(space + pad_to_length(Arguments::ResultsPrefixPattern, length) + space + "Prefix for results files to be written. Optional. If omitted, the current working directory is used.");
    message("");
  }

  std::string command() const noexcept {
    std::string text{executable_name_};
    for (const std::string& argument : arguments_) {
      text.append(" " + argument);
    }
    return text;
  }

  void message_command() const noexcept {
    message("Command: " + command());
  }

  void message_start_information() const noexcept {
    if (!game_data_file_.empty()) {
      message("The game data will be read from: " + game_data_file_.string());
    }
    if (!results_prefix_.empty()) {
      message("The results files will be written to: " + results_prefix_.string());
    } else {
      message("The results files will be written to the current working directory.");
    }
  }

  void check() const {
    if (game_data_file_.empty()) {
      message_usage_information();
      error("The game data file (" + Arguments::GameDataFilePattern + ") is missing.");
    }
  }

};

} // namespace CatanLeaderboard
