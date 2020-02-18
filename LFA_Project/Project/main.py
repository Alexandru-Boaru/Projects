#!/usr/bin/env python
import sys
import pickle
import tema1

from regex import RegEx

from antlr4 import *
from RegexLexer import RegexLexer
from RegexListener import RegexListener
from RegexParser import RegexParser
from EvaluateListener import EvaluateListener
import sys, io

if __name__ == "__main__":
    valid = (len(sys.argv) == 4 and sys.argv[1] in ["RAW", "TDA"]) or \
            (len(sys.argv) == 3 and sys.argv[1] == "PARSE")
    if not valid:
        sys.stderr.write(
            "Usage:\n"
            "\tpython3 main.py RAW <regex-str> <words-file>\n"
            "\tOR\n"
            "\tpython3 main.py TDA <tda-file> <words-file>\n"
            "\tOR\n"
            "\tpython3 main.py PARSE <regex-str>\n"
        )
        sys.exit(1)

    if sys.argv[1] == "TDA":
        tda_file = sys.argv[2]
        with open(tda_file, "rb") as fin:
            parsed_regex = pickle.loads(fin.read())
    else:
        regex_string = sys.argv[2]

        # TODO "regex_string" conține primul argument din linia de comandă,
        # șirul care reprezintă regexul cerut. Apelați funcția de parsare pe el
        # pentru a obține un obiect RegEx pe care să-l stocați în
        # "parsed_regex"
        #
        # Dacă nu doriți să implementați parsarea, puteți ignora această parte.
        stream = InputStream(regex_string)
        lexer = RegexLexer(stream)
        stream = CommonTokenStream(lexer)
        parser = RegexParser(stream)
        tree = parser.r1()
        e = EvaluateListener()
        walker = ParseTreeWalker()
        walker.walk(e, tree)
        parsed_regex = e.stack[0]

        if sys.argv[1] == "PARSE":
            print(str(parsed_regex))
            sys.exit(0)

    # În acest punct, fie că a fost parsat, fie citit direct ca obiect, aveți
    # la dispoziție variabila "parsed_regex" care conține un obiect de tip
    # RegEx. Aduceți-l la forma de Automat Finit Determinist, pe care să puteți
    # rula în continuare.
    with open(sys.argv[3], "r") as fin:
        content = fin.readlines()
    (nfa, dfa) = tema1.regex_to_dfa(parsed_regex);
    for word in content:
        # TODO la fiecare iterație, "word" conținue un singur cuvânt din
        # fișierul de input; verificați apartenența acestuia la limbajul
        # regexului dat și scrieți rezultatul la stdout.
        tema1.check_word(word, dfa);
        pass
