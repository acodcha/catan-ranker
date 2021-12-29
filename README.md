![build and test](https://github.com/acodcha/catan-stratification/workflows/build%20and%20test/badge.svg?branch=main)

Leaderboard generator for the Catan board game originally designed by Klaus Teuber. The leaderboard consists of Markdown files containing tables and plots. See https://github.com/acodcha/catan-leaderboard for an example of a leaderboard that uses this program.

- [Setup](#setup)
- [Usage](#usage)
- [Documentation](#documentation)
- [License](#license)

## Setup
The following packages are required:
- **C++17 Compiler:** Any C++17 compiler will do, such as GCC or Clang. On Ubuntu, install GCC with `sudo apt install g++` or Clang with `sudo apt install clang`.
- **CMake:** On Ubuntu, install with `sudo apt install cmake`.
- **Gnuplot:** On Ubuntu, install with `sudo apt install gnuplot`.

Build the program with:

```
mkdir build
cd build
cmake ..
make
```

This builds the `build/bin/catan-stratification` program.

You can optionally run tests from the `build` directory with:

```
make test
```

You can optionally install the program from the `build` directory with:

```
sudo make install
```

This installs the program to `/usr/local/bin/catan-stratification`. To uninstall the program, simply delete it.

## Usage
Run with no arguments or with the `--help` argument to obtain usage information.

Otherwise, for regular use, run with:

```
catan-stratification --games <path> --leaderboard <path>
```

- `--games <path>` specifies the path to the games file to be read. Required.
- `--leaderboard <path>` specifies the path to the directory in which the leaderboard will be written. Optional. If omitted, no leaderboard is written.

### Games File
The games file is a plain text file with the following format:

```
2020-03-15 : 10 : Alice 10 , Bob 8 , Claire 7 , David 5
2020-03-15 : 13 : Alice 13 , Bob 11 , Claire 10
2020-03-17 : 10 : Claire 10 , Bob 9 , Alice 9 , Francis 8 , Edith 8
2020-03-18 : 10 : David 10 , Claire 9 , Alice 8 , Bob 8 , Edith 7 , Francis 6
2020-03-20 : 10 : Claire 10 * , Alice 11 , Bob 7 , David 7 , Edith 5
2020-03-21 : 14 : Bob 14 , Alice 13 , David 11 , Claire 9
```

- Each line represents a game.
- Each game consists of a date, a winning number of points, and a list of player names and points.
- The date must be in the YYYY-MM-DD format.
- A colon (`:`) is used to separate the date from the winning number of points.
- The winning number of points must always be specified, even for a standard 10-point game.
- A colon (`:`) is used to separate the winning number of points from the list of player names and points.
- Each player name is case-sensitive and must be followed by that player's points at game end.
- A comma (`,`) is used to separate players.
- Except for line breaks, all whitespace is ignored. However, as in the above example, the use of whitespace is recommended for readability.
- The ordering of games is unimportant. The games will be sorted by date during processing.
- The ordering of players in a game is unimportant. The winning player is the first player to reach 10 points during their turn. Players can be tied for 2nd place, 3rd place, and so on depending on their points.
- In 5+ player games, because of the special build phase and because a player can only win during their turn, it is possible for the winning player to be tied for most points or to not have the most points. In such cases, identify the winning player by placing an asterisk (`*`) after their points, as in the above example for the game played on 2020-03-20.

## Documentation
Building the documentation is optional and requires additional packages:
- **Doxygen:** On Ubuntu, install with `sudo apt install doxygen`.
- **Graphviz:** On Ubuntu, install with `sudo apt install graphviz`.
- **TeX Live:** On Ubuntu, install with `sudo apt install texlive texlive-fonts-extra`.

Documentation is disabled by default but can be generated from the `build` directory with:

```
cmake .. -DBUILD_DOCS=ON
make docs
```

This generates HTML documentation using Doxygen. The documentation is located in `docs/html`. Open the `docs/html/index.html` file in any web browser to view the documentation.

## License
This work is maintained by Alexandre Coderre-Chabot (<https://github.com/acodcha>) and licensed under the MIT License. For more details, see the [LICENSE](LICENSE) file or <https://mit-license.org/>. This work is based on the Catan board game (also known as Settlers of Catan) originally designed by Klaus Teuber. The contents, copyrights, and trademarks of everything involving Catan are exclusively held by its designers and publishers; I make no claim to any of these in any way.
