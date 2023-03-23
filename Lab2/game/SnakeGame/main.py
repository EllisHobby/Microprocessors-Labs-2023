# Simple Snake Game in Python 3 for Beginners
# By @TokyoEdTech
# modified by Yan Luo
# modified by Ellis Hobby
#
# you need to install a few python3 packages
#   pip3 install pyserial

import turtle
import time
import random

# custom serial class - see <serial_control.py>
import serial_control as ser

FONT_SIZE = 20

delay = 0.1

# Score
score = 0
high_score = 0
ppa = 10

# Set up the screen
wn = turtle.Screen()
wn.title("Snake Game by @TokyoEdTech (mod by YL)")
wn.bgcolor("green")
wn.setup(width=600, height=600)
wn.tracer(0) # Turns off the screen updates

# Snake head
head = turtle.Turtle()
head.speed(0)
head.shape("square")
head.color("black")
head.penup()
head.goto(0,0)
head.direction = "stop"

# Snake food
food = turtle.Turtle()
food.speed(0)
food.shape("circle")
food.color("red")
food.penup()
food.goto(0,100)
food.color("red")

segments = []

# Pen
pen = turtle.Turtle()
pen.speed(0)
pen.shape("square")
pen.color("white")
pen.penup()
pen.hideturtle()
pen.goto(0, 260)
pen.write("Score: {}  High Score: {}  P/A: {}".format(score, high_score, ppa), align="center", font=("Courier", FONT_SIZE, "normal")) 


# Functions
def go_up():
    if head.direction != "down":
        head.direction = "up"

def go_down():
    if head.direction != "up":
        head.direction = "down"

def go_left():
    if head.direction != "right":
        head.direction = "left"

def go_right():
    if head.direction != "left":
        head.direction = "right"

def move():
    if head.direction == "up":
        y = head.ycor()
        head.sety(y + 20)

    if head.direction == "down":
        y = head.ycor()
        head.sety(y - 20)

    if head.direction == "left":
        x = head.xcor()
        head.setx(x - 20)

    if head.direction == "right":
        x = head.xcor()
        head.setx(x + 20)

# Keyboard bindings
wn.listen()
wn.onkey(go_up, "w")
wn.onkey(go_down, "s")
wn.onkey(go_left, "a")
wn.onkey(go_right, "d")


#/////////////////////////////////////////////////////

# init class and search for Arduino device
controller = ser.SerialController()
controller.find_device()

# if device found check connection
if controller.dev:
    controller.check_connection()
else:
    print("Cannot Find Controller")
    exit()

# used for indexing direction from serial data
dir_list = [go_up, go_down, go_right, go_left]

#/////////////////////////////////////////////////////


# Main game loop
while True:
    wn.update()


    #/////////////////////////////////////////////////////

    # read data as dict with keys: dir, shake
    control_data = controller.get_control_dict()
    dir = control_data['dir']
    shake = control_data['shake']

    # use dir index to call approprate movement method
    dir_list[dir]()

    # if shake detected, double points and make apple gold
    if (shake and (food.color()[0] != 'gold')):
        food.color('gold')
        ppa = ppa * 2
        pen.clear()
        pen.write("Score: {}  High Score: {}  P/A: {}".format(score, high_score, ppa), align="center", font=("Courier", FONT_SIZE, "normal")) 

    #/////////////////////////////////////////////////////
    


    # Check for a collision with the border
    if head.xcor()>290 or head.xcor()<-290 or head.ycor()>290 or head.ycor()<-290:
        time.sleep(1)
        head.goto(0,0)
        head.direction = "stop"

        # Hide the segments
        for segment in segments:
            segment.goto(1000, 1000)
        
        # Clear the segments list
        segments.clear()

        # Reset the score
        score = 0

        # Reset the delay
        delay = 0.1

        pen.clear()
        pen.write("Score: {}  High Score: {}  P/A: {}".format(score, high_score, ppa), align="center", font=("Courier", FONT_SIZE, "normal")) 


    # Check for a collision with the food
    if head.distance(food) < 20:

        #/////////////////////////////////////////////////////

        # send signal to start buzzer
        controller.start_buzzer()

        # if gold apple eaten, revert points, let controller know
        if (food.color()[0] == 'gold'):
            controller.reset_shake()
            food.color('red')
            ppa = ppa / 2

        #/////////////////////////////////////////////////////

        # Move the food to a random spot
        x = random.randint(-290, 290)
        y = random.randint(-290, 290)
        food.goto(x,y)

        # Add a segment
        new_segment = turtle.Turtle()
        new_segment.speed(0)
        new_segment.shape("square")
        new_segment.color("grey")
        new_segment.penup()
        segments.append(new_segment)

        # Shorten the delay
        delay -= 0.001

        # Increase the score
        score += ppa

        if score > high_score:
            high_score = score

        
        pen.clear()
        pen.write("Score: {}  High Score: {}  P/A: {}".format(score, high_score, ppa), align="center", font=("Courier", FONT_SIZE, "normal")) 

    # Move the end segments first in reverse order
    for index in range(len(segments)-1, 0, -1):
        x = segments[index-1].xcor()
        y = segments[index-1].ycor()
        segments[index].goto(x, y)

    # Move segment 0 to where the head is
    if len(segments) > 0:
        x = head.xcor()
        y = head.ycor()
        segments[0].goto(x,y)

    move()    

    # Check for head collision with the body segments
    for segment in segments:
        if segment.distance(head) < 20:
            time.sleep(1)
            head.goto(0,0)
            head.direction = "stop"
        
            # Hide the segments
            for segment in segments:
                segment.goto(1000, 1000)
        
            # Clear the segments list
            segments.clear()

            # Reset the score
            score = 0

            # Reset the delay
            delay = 0.1
        
            # Update the score display
            pen.clear()
            pen.write("Score: {}  High Score: {}  P/A: {}".format(score, high_score, ppa), align="center", font=("Courier", FONT_SIZE, "normal")) 

    time.sleep(delay)

wn.mainloop()
