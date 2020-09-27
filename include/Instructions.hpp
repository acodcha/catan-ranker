#pragma once

#include "Base.hpp"

namespace CatanLeaderboardGenerator {

/// \brief Namespace listing the program's command-line argument keywords.
namespace Arguments {

const std::string UsageInformation{"--help"};

const std::string GamesFileKey{"--games"};

const std::string GamesFilePattern{GamesFileKey + " <path>"};

const std::string ResultsDirectoryKey{"--results"};

const std::string ResultsDirectoryPattern{ResultsDirectoryKey + " <path>"};

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

  const std::experimental::filesystem::path& games_file() const noexcept {
    return games_file_;
  }

  const std::experimental::filesystem::path& results_directory() const noexcept {
    return results_directory_;
  }

protected:

  std::string executable_name_;

  std::vector<std::string> arguments_;

  std::experimental::filesystem::path games_file_;

  std::experimental::filesystem::path results_directory_;

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
      } else if (*argument == Arguments::GamesFileKey && argument + 1 < arguments_.cend()) {
        games_file_ = {*(argument + 1)};
      } else if (*argument == Arguments::ResultsDirectoryKey && argument + 1 < arguments_.cend()) {
        results_directory_ = {*(argument + 1)};
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
    message(space + executable_name_ + " " + Arguments::GamesFilePattern + " " + Arguments::ResultsDirectoryPattern);
    const uint_least64_t length{std::max({
      Arguments::UsageInformation.length(),
      Arguments::GamesFilePattern.length(),
      Arguments::ResultsDirectoryPattern.length()
    })};
    message("Arguments:");
    message(space + pad_to_length(Arguments::UsageInformation, length) + space + "Displays this information and exits.");
    message(space + pad_to_length(Arguments::GamesFilePattern, length) + space + "Path to the games file to be read. Required.");
    message(space + pad_to_length(Arguments::ResultsDirectoryPattern, length) + space + "Path to the directory in which results files will be written. Optional.");
    message("");
  }

  std::string command() const noexcept {
    std::string text{executable_name_};
    for (const std::string& argument : arguments_) {
      text += " " + argument;
    }
    return text;
  }

  void message_command() const noexcept {
    message("Command: " + command());
  }

  void message_start_information() const noexcept {
    if (!games_file_.empty()) {
      message("The games will be read from: " + games_file_.string());
    }
    if (!results_directory_.empty()) {
      message("The results will be written to: " + results_directory_.string());
    } else {
      warning("The results directory (" + Arguments::ResultsDirectoryPattern + ") is missing. Results files will not be written.");
    }
  }

  void check() const {
    if (games_file_.empty()) {
      message_usage_information();
      error("The games file (" + Arguments::GamesFilePattern + ") is missing.");
    }
  }

};

} // namespace CatanLeaderboardGenerator
