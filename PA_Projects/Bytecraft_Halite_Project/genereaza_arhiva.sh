#!/bin/sh

python ./create_archive.py --etapa 1 --makefile Makefile --readme README.md --files MyBot.java hlt/* myBot/* $(find ./org/la4j -type f -follow -print) --test "java MyBot"