#!/bin/sh
set -e
cd "${0%/*}"
./clear.sh
../build/bin/catan-stratification --games games.txt --leaderboard leaderboard
