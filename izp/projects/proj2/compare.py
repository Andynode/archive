#!/usr/bin/python3
import os
import sys
import re


def compare(file1, file2):
    if not os.path.isfile(file1) or not os.path.isfile(file2):
        print("The files have to exist")
        sys.exit(1)
    with open(file1, "r") as infile:
        first_content = infile.read().splitlines()
    with open(file2, "r") as infile:
        second_content = infile.read().splitlines()
    if len(first_content) != len(second_content):
        print("The files have to equal in length, check whether some inputs were skipped")
        sys.exit(1)

    diffs = 0
    for i in range(len(first_content) // 4):
        result_one = first_content[4 * i: 4 * (i + 1)]
        result_two = second_content[4 * i: 4 * (i + 1)]
        if result_one[0] != result_two[0]:
            print("The inputs have to equal")
            sys.exit(1)
        accuracy = float(result_one[0].split(' ')[-1])
        pattern = re.compile(r"(Up|Ip)=(?P<result>[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?) (A|V)")
        for j in (1, 2):
            match_one = pattern.match(result_one[j])
            match_two = pattern.match(result_two[j])
            if not match_one or not match_two:
                print("The used regex didn't match, sorry :(")
                continue
            value_one = float(match_one.group('result'))
            value_two = float(match_two.group('result'))
            if abs(value_one - value_two) > 2 * accuracy:
                diffs += 1
                print("Found difference exceeding accuracy on line {}".format(4 * i + j + 1))
                print("{}: {}, {}: {}\n".format(file1, result_one[j], file2, result_two[j]))

    print("Number of differences: {}".format(diffs))


if __name__ == '__main__':
    if len(sys.argv) < 2:
            print("Usage: {} file1 file2".format(sys.argv[0]))
            sys.exit(1)
    compare(sys.argv[1], sys.argv[2])


