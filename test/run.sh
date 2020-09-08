#!/bin/sh
cd "${0%/*}"
../build/bin/catan_leaderboard --games ../data/games.txt --players ../data/players.txt --results ../results/

