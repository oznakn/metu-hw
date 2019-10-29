# The2 Tester by Ozan Akin & Ilker Kosaroglu, for now :))
import sys

from the2 import is_firmus

def is_not_same(value1, value2):
	return not (abs(value1 - value2) < 0.001)

filepath = './output'

if len(sys.argv) > 1:
	filepath = sys.argv[1]

with open(filepath) as fp:  
	line = fp.readline()
	count = 0

	tests_not_pass = []

	while line:
		count += 1

		data = line.strip()

		datas = line.split(';')

		values1 = eval(datas[0])
		values2 = eval(datas[1])
		expected_result = eval(datas[2])

		result = is_firmus(values1, values2)

		if expected_result[0] == 'FIRMUS':
			if result[0] != 'FIRMUS' or is_not_same(expected_result[1], result[1]):
				print 'Test %d:' % count, 'False'
				tests_not_pass.append(count)
		
		elif expected_result[0] == 'DAMNARE':
			if result[0] != 'DAMNARE' or is_not_same(expected_result[1], result[1]):
				print 'Test %d:' % count, 'False'
				tests_not_pass.append(count)

		elif expected_result[0] == 'ADDENDUM':
			addendum = result[1]
			expected_addendum = expected_result[1]

			if result[0] != 'ADDENDUM':
				print 'Test %d:' % count, 'False'
				tests_not_pass.append(count)

			elif is_not_same(addendum[0], expected_addendum[0]) and is_not_same(addendum[0], expected_addendum[2]) and is_not_same(addendum[2], expected_addendum[0]) and is_not_same(addendum[2], expected_addendum[2]):
				print 'Test %d:' % count, 'False'
				tests_not_pass.append(count)

			elif is_not_same(addendum[1], expected_addendum[1]) and is_not_same(addendum[1], expected_addendum[3]) and is_not_same(addendum[3], expected_addendum[1]) and is_not_same(addendum[3], expected_addendum[3]):
				print 'Test %d:' % count, 'False'
				tests_not_pass.append(count)

		line = fp.readline()

	if len(tests_not_pass) == 0:
		print '%d tests passed' % count
	else:
		print 'Could not pass from %d tests' % len(tests_not_pass)
