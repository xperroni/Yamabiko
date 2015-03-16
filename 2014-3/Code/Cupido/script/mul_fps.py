#!/usr/bin/env python

def convert(fps, path):
    for line in open(path):
        row = eval(line)
        print [row[0] * fps, row[1] * fps, row[2]]


def main():
    from sys import argv
    convert(int(argv[1]), argv[2])


if __name__ == '__main__':
    main()
