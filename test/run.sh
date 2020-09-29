#!/bin/sh
set -e
cd "${0%/*}"
./clear.sh
../build/bin/catan_leaderboard_generator --games games.txt --results leaderboard

