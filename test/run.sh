#!/bin/sh
set -e
cd "${0%/*}"
./clear.sh
../build/bin/catan_leaderboard --games ../Games.txt --results ../results

