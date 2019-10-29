import math

data = None

def calculate_force(G, particle1, particle2):
	diff_vector_x = particle2[1] - particle1[1]
	diff_vector_y = particle2[2] - particle1[2]

	distance = math.sqrt(math.pow(diff_vector_x, 2) + math.pow(diff_vector_y, 2))

	coefficient = float(G * particle1[0] * particle2[0]) / math.pow(distance, 3)

	return [coefficient * diff_vector_x, coefficient * diff_vector_y]

def new_move():
	global data

	if data is None:
		data = get_data()

		data[0] = float(data[0])
		data[1] = float(data[1])

		for particle in data[2:]:
			particle[0] = float(particle[0])
			particle[1] = float(particle[1])
			particle[2] = float(particle[2])
			particle[3] = float(particle[3])
			particle[4] = float(particle[4])

	G = data[0]
	delta_t = data[1]

	deltas = []

	for i in range(2, len(data)):
		particle1 = data[i]

		delta_x = particle1[3] * delta_t
		delta_y = particle1[4] * delta_t

		deltas.append([delta_x, delta_y])

		particle1[1] += delta_x
		particle1[2] += delta_y

	for i in range(2, len(data)):
		particle1 = data[i]

		force = [0, 0]
		for j in range(2, len(data)):
			if i == j:
				continue

			particle2 = data[j]

			new_force = calculate_force(G, particle1, particle2)

			force[0] += new_force[0]
			force[1] += new_force[1]

		acceleration = [force[0]/particle1[0], force[1]/particle1[0]]

		particle1[3] += acceleration[0] * delta_t
		particle1[4] += acceleration[1] * delta_t

	return deltas
