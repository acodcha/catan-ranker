# Catan Leaderboard
Leaderboard for Catan games with friends.

## Dependencies
The following packages are required:
- **C++17 Compiler:** Any C++17 compiler will do, such as GCC or Clang. On Ubuntu, install GCC with `sudo apt install g++` or Clang with `sudo apt install clang`.
- **CMake:** On Ubuntu, install with `sudo apt install cmake`.

## Configuration and Build
Configure and build the program with CMake:

```
mkdir build
cd build
cmake ..
make
```

## Installation
Install the executable to the system from the build directory with CMake:

```
sudo make install
```

This copies the executable to `/usr/local/bin/catan_leaderboard`. To uninstall the executable, simply delete it.

## Documentation
Building the documentation is optional and requires additional packages:
- **Doxygen:** On Ubuntu, install with `sudo apt install doxygen`.
- **Graphviz:** On Ubuntu, install with `sudo apt install graphviz`.
- **TeX Live:** On Ubuntu, install with `sudo apt install texlive texlive-fonts-extra`.

Documentation is disabled by default but can be generated from the `build` directory with CMake:

```
cmake .. -DBUILD_DOCS=ON
make docs
```

This generates HTML documentation using Doxygen. The documentation is located in `docs/html`. Open the `docs/html/index.html` file in any web browser to view the documentation.

## Testing
Run tests from the `build` directory with CMake:

```
make test
```

## Usage
Run with no arguments or with the `--help` argument to obtain usage information.

Otherwise, for regular use, run with:

```
catan_leaderboard --games <path> --players <path> --results <path>
```

- `--games <path>` specifies the path to the games data file to be read. Required.
- `--players <path>` specifies the path to the players list file to be read. Required.
- `--results <path>` specifies the prefix to the results files to be written. Optional. If omitted, the current working directory is used.

## License
This work is licensed under the MIT License. For more details, see the [LICENSE](LICENSE) file or <https://mit-license.org/>.

## Maintainer
- Alexandre Coderre-Chabot (<https://github.com/acodcha>)
