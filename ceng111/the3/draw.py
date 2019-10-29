#
# YOU CAN PLAY AROUND WITH "SCALE" AND "DELAY". OTHERWISE,
#    YOU SHOULDN'T UPDATE THIS FILE.
# DO NOT SEND THIS FILE TO US.
#
from math import *
from Tkinter import *
from evaluator import *
DELAY = 10 # that is the default value, can be changed
SCALE = 1.0
from the3 import *


def drawcircle(canv,x,y,rad):
	"""DO NOT MODIFY THIS FUNCTION"""
	return canv.create_oval(SCALE*(x-rad),SCALE*(y-rad),SCALE*(x+rad),SCALE*(y+rad),width=0,fill='red')

def drawcircles(): #inialization of circles
    """DO NOT MODIFY THIS FUNCTION"""
    global My_canvas, My_circles, DELAY
    My_circles=[]
    circlelist = [] # will hold a list of [x,y,r]
    data = get_data()[2:] #drop G value and Delta t
    max_mass = max([m for [m,x,y,vx,vy] in data])
    for [m,x,y,vx,vy] in data:
       circlelist.append([x,y,1+sqrt(m/max_mass)*20])
    for [center_x,center_y,radius] in circlelist:
        My_circles.append(drawcircle(My_canvas,center_x,center_y,radius))
    My_canvas.after(DELAY,callback)

def movecircles():
    """DO NOT MODIFY THIS FUNCTION"""
    global My_circles, My_canvas
    deltalist = new_move() # user provides new_move(): list of [dx,dy]
    for i,[dx,dy] in enumerate(deltalist):
        My_canvas.move(My_circles[i],SCALE*dx,SCALE*dy)

def callback():
    """DO NOT MODIFY THIS FUNCTION"""
    global My_canvas,DELAY
    movecircles()
    My_canvas.after(DELAY,callback)

Master = Tk()

My_canvas = Canvas(Master, width=500, height=500)
My_canvas.pack()
drawcircles()

Master.mainloop()
