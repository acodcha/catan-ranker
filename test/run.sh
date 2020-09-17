#!/bin/sh
set -e
cd "${0%/*}"
../build/bin/catan_leaderboard --games ../Games.txt --results ../results/

