#!/bin/sh
set -e
cd "${0%/*}"
./clear.sh
../build/bin/catan-ranker --games games.txt --leaderboard leaderboard
