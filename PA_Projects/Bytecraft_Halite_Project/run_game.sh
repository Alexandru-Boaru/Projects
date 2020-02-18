#!/bin/sh

SEED=42
WIDTH=32
HEIGHT=32

case "$1" in

1)  SEED=42
    WIDTH=32
    HEIGHT=32
    ;;

2)  SEED=1673031865
    WIDTH=32
    HEIGHT=32
    ;;

3)  SEED=1773807367
    WIDTH=40
    HEIGHT=40
    ;;

4)  SEED=1942373999
    WIDTH=48
    HEIGHT=48
    ;;

5)  SEED=142342898
    WIDTH=56
    HEIGHT=56
    ;;

*) echo "USAGE: ./run_game.sh nr_test"
    exit
    ;;

esac


set -e

javac MyBot.java
./halite --replay-directory replays/ -vvv --width $WIDTH --height $HEIGHT -s $SEED "java MyBot"
