#!/usr/bin/python3
import os
import sys

if len(sys.argv) < 2:
	print("Usage: {} program_name".format(sys.argv[0]))
	sys.exit(1)

prog_name = sys.argv[1]

arg_list = [
["0", "0.01", "0.1", "0.2", "0.35", "0.5", "0.7", "1", "5", "10", "100", "1e3", "1e6"],
["1", "10", "100", "1e3", "1e4", "22e3", "1e5", "1e6"],
["1e-9"]
]

def loop_over(arg_list, args=[], idx=0):
	if len(arg_list) <= idx:
		print("$ {args}".format(args=' '.join(args)))
		stream = os.popen(prog_name + ' ' + ' '.join(args))
		print(stream.read())
		stream.close()
	else:
		for arg in arg_list[idx]:
			copy = args.copy()
			copy.append(arg)
			loop_over(arg_list, copy, idx+1)

loop_over(arg_list)

