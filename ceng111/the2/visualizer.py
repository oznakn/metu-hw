# Rectangle Visualizer by Ozan Akin, for now :))

import random
import time
import sys

from turtle import *
from the2 import is_firmus

SIZE = 1000

def init(turtle):
	turtle.pencolor(0, 255, 0)

	turtle.up()
	turtle.setx(-SIZE/2)
	turtle.sety(0)
	turtle.down()
	turtle.setx(SIZE/2)

	turtle.up()
	turtle.sety(SIZE/2)
	turtle.setx(0)
	turtle.down()
	turtle.sety(-SIZE/2)

	turtle.up()

	turtle.pencolor(0, 0, 0)


def draw_middle(turtle, x):
	turtle.pencolor(255, 0, 0)

	turtle.setx((SIZE/200) * x)
	turtle.sety(SIZE/2)
	turtle.down()
	turtle.sety(-SIZE/2)

	turtle.pencolor(0, 0, 0)
	turtle.up()

def draw_rectangle(turtle, values, color_code = 0):
	x_diff = abs(values[0] - values[2]) * (SIZE/200)
	y_diff = abs(values[1] - values[3]) * (SIZE/200)

	turtle.up()

	turtle.setx(min(values[0], values[2]) * (SIZE/200))
	turtle.sety(min(values[1], values[3]) * (SIZE/200))

	turtle.down()

	turtle.pencolor(0, 0, color_code)

	turtle.forward(x_diff)
	turtle.left(90)
	turtle.forward(y_diff)
	turtle.left(90)
	turtle.forward(x_diff)
	turtle.left(90)
	turtle.forward(y_diff)
	turtle.left(90)

	turtle.up()

	if color_code == 0:
		draw_middle(turtle, (values[0]+ values[2])/2.0)
		

screen = Screen()
screen.setup(SIZE, SIZE)
screen.colormode(255)

turtle = Turtle()
turtle.hideturtle()
turtle.speed(8)
init(turtle)

if __name__ == '__main__':
	if len(sys.argv) > 2:
		values1 = eval(sys.argv[1])
		values2 = eval(sys.argv[2])

		if min(values1[1], values1[3]) > min(values2[1], values2[3]):
			values1, values2 = values2, values1

		draw_rectangle(turtle, values1)
		draw_rectangle(turtle, values2)

		turtle.setx(-SIZE/5.0)
		turtle.sety(-SIZE/5.0)

		result = is_firmus(values1, values2)

		turtle.write(str(result), move=True, align='center')

		if result[0] == 'ADDENDUM':
			draw_rectangle(turtle, result[1], 200)

	screen.exitonclick()
