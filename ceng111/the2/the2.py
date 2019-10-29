def is_equal(value1, value2):
	return abs(value1 - value2) < 0.001

def is_smaller_equal(value1, value2):
	return is_equal(value1, value2) or value1 < value2

def is_greater(value1, value2):
	return (not is_equal(value1, value2)) and value1 > value2

def is_firmus(rect1, rect2):
	if is_greater(rect1[0], rect1[2]):
		rect1[0], rect1[2] = rect1[2], rect1[0]
	if is_greater(rect1[1], rect1[3]):
		rect1[1], rect1[3] = rect1[3], rect1[1]
	if is_greater(rect2[0], rect2[2]):
		rect2[0], rect2[2] = rect2[2], rect2[0]
	if is_greater(rect2[1], rect2[3]):
		rect2[1], rect2[3] = rect2[3], rect2[1]
	if is_equal(rect2[1], min(rect1[1], rect2[1])):
		rect1, rect2 = rect2, rect1

	x_diff = min(rect1[2], rect2[2]) - max(rect1[0], rect2[0])
	y_diff = min(rect1[3], rect2[3]) - max(rect1[1], rect2[1])

	if x_diff < 0.001:
		x_diff = 0
	if y_diff < 0.001:
		y_diff = 0

	total_area = (abs((rect1[2] - rect1[0]) * (rect1[3] - rect1[1])) + abs((rect2[2] - rect2[0]) * (rect2[3] - rect2[1]))) - (x_diff * y_diff)

	upper_rect_x_center = (rect2[2] + rect2[0])/2.0

	if is_equal(rect1[1], 0) and is_equal(rect1[3], rect2[1]) and is_smaller_equal(rect1[0], upper_rect_x_center) and is_smaller_equal(upper_rect_x_center, rect1[2]):
		return ['FIRMUS', total_area] 

	elif is_equal(rect1[1], 0) and is_equal(rect1[3], rect2[1]) and (not is_greater(rect2[0], rect1[2])) and (not is_greater(rect1[0], rect2[2])):
		if is_greater(rect1[0], upper_rect_x_center):
			new_x2 = rect2[2] + ((rect1[0] - upper_rect_x_center) * 2)
			return ['ADDENDUM', [rect2[2], rect2[1] ,new_x2, rect2[3]]]

		if is_greater(upper_rect_x_center, rect1[2]):
			new_x1 = rect2[0] - ((upper_rect_x_center - rect1[2]) * 2)
			return ['ADDENDUM', [new_x1, rect2[1] ,rect2[0], rect2[3]]]

		return None

	return ['DAMNARE', total_area]
