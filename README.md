# Catan Leaderboard Generator
![build and test](https://github.com/acodcha/CatanLeaderboardGenerator/workflows/build%20and%20test/badge.svg?branch=master)

Generates a simple leaderboard for Catan games.

## Dependencies
The following packages are required:
- **C++17 Compiler:** Any C++17 compiler will do, such as GCC or Clang. On Ubuntu, install GCC with `sudo apt install g++` or Clang with `sudo apt install clang`.
- **CMake:** On Ubuntu, install with `sudo apt install cmake`.
- **Gnuplot:** On Ubuntu, install with `sudo apt install gnuplot`.

## Configuration and Build
Configure and build the program with:

```
mkdir build
cd build
cmake ..
make
```

## Installation
Once you have configured and built the program, install it from the `build` directory with:

```
sudo make install
```

This installs the program to `/usr/local/bin/catan_leaderboard_generator`. To uninstall the program, simply delete it.

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

## Testing
Once you have configured and built the program, run tests from the `build` directory with:

```
make test
```

## Usage
Run with no arguments or with the `--help` argument to obtain usage information.

Otherwise, for regular use, run with:

```
catan_leaderboard_generator --games <path> --results <path>
```

- `--games <path>` specifies the path to the games file to be read. Required.
- `--results <path>` specifies the path to the directory in which results files will be written. Optional.

## License
This work is licensed under the MIT License. For more details, see the [LICENSE](LICENSE) file or <https://mit-license.org/>.

## Maintainer
- Alexandre Coderre-Chabot (<https://github.com/acodcha>)
