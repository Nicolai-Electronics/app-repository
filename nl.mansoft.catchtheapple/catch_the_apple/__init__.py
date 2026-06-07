import buttons
import display
import machine
import mch22
import random
import time

from .apple import apple
from .basket import basket

basket_move = 'stop'
gameover = 0
score = 0
speed = 0

def display_next(apple_x, apple_y, basket_x):
    display.drawFill(0xFFFFFF)
    display.drawPng(apple_x, apple_y, apple)
    display.drawPng(basket_x, display.height() - 70, basket)
    display.drawText(10, 10, str(score), 0x000000, "permanentmarker36")
    display.flush()

def game():
    apple_x = random.randint(0, display.width() - 50)
    apple_y = 0
    basket_x = int(display.width() / 2) - 35
    global gameover
    global score
    global speed
    while True:
        if gameover == 0:
            display_next(apple_x, apple_y, basket_x)
            apple_y += 10 + speed
            if apple_y > 400:
                if apple_x + 10 > basket_x and apple_x + 20 < basket_x + 70:
                    score += 1
                    speed += 1
                else:
                    gameover = 1
                apple_x = random.randint(0, display.width() - 50)
                apple_y = 0
            if basket_move == 'left':
                basket_x -= 25
                basket_x = max(basket_x, 0)
            if basket_move == 'right':
                basket_x += 25
                if basket_x > display.width() - 70:
                    basket_x = display.width() - 70
        else:
            text_x = int(display.width() / 2) - 120
            text_y = int(display.height() / 2) - 30
            display.drawText(text_x, text_y, "Game Over!", 0xD41E1E, "permanentmarker36")
            display.drawText(text_x + 20, text_y + 40, "Press A to start", 0x000000, "permanentmarker22")
            display.flush()
        #machine.lightsleep(500)
        time.sleep(0.1)

def basket_left(pressed):
    global basket_move
    if pressed:
        basket_move = 'left'
    else:
        basket_move = 'stop'

def basket_right(pressed):
    global basket_move
    if pressed:
        basket_move = 'right'
    else:
        basket_move = 'stop'

def startover(pressed):
    global gameover
    global score
    global speed
    if pressed:
        gameover = 0
        score = 0
        speed = 0

def exit_app(pressed):
    if pressed:
        mch22.exit_python()


buttons.attach(buttons.BTN_LEFT, basket_left)
buttons.attach(buttons.BTN_RIGHT, basket_right)
buttons.attach(buttons.BTN_HOME, exit_app)
buttons.attach(buttons.BTN_A, startover)
buttons.attach(buttons.BTN_B, exit_app)

game()
