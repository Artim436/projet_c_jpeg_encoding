#! /usr/bin/env python3
import sys
from random import randint

def write_P2(filename, largeur, hauteur):
    f = open(filename, "w")
    print("P2", file=f)
    print(f"{largeur} {hauteur}", file=f)
    for i in range(hauteur):
        for j in range(largeur):
            if j == largeur - 1:
                print(f"{randint(0, 255)}", file=f)
            else:
                print(f"{randint(0, 255)}", file=f, end= " ")

if __name__ == "__main__":
    len_argv = len(sys.argv)
    if len_argv == 4:
        filename = sys.argv[1]
        largeur = int(sys.argv[2])
        hauteur = int(sys.argv[3])
        write_P2(filename, largeur, hauteur)