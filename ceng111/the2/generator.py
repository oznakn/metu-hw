import random
import time
import sys

from the2 import is_firmus

def rand():
	return random.uniform(-101, 101)

def generate():
	values1 = [rand(), rand(), rand(), rand()]
	values2 = [rand(), rand(), rand(), rand()]

	result = is_firmus(values1, values2)

	return [values1, values2, result]

if __name__ == '__main__':

	for i in range(int(sys.argv[1])):
		[values1, values2, result] = generate()

		if result[0] == 'ADDENDUM' and (100 < result[0][0] < -100 or 100 < result[0][1] < -100 or 100 < result[0][2] < -100 or 100 < result[0][3] < -100):
			continue

		print str(values1) + ';' + str(values2) + ';' +  str(result)
