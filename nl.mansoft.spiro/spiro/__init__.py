# Press the green Play button on the right to start the simulation.
# 
# Your code will run, and then you'll get the MicroPython REPL.
# 
# The REPL is an interactive prompt where you can type python commands and see the results immediately.
# Type help() for MicroPython API cheat sheet. To paste code into the REPL press Ctrl+E.
#
# Press the blue pushbutton to interact with the example.

import display
import math

print("Hello, MCH2022 Badge!")

width = display.width()
height = display.height()
size = min(width, height) / 2
AA = 6.47059E-2
BB = 0.31
stap = 2.0 * math.pi / 100.0

def gcd(a, b):
    """Calculate the Greatest Common Divisor of a and b.

    Unless b==0, the result will have the same sign as b (so that when
    b is divided by it, the result comes out positive).
    """
    while b:
        a, b = b, a % b
    return a
    
def spirofig(ring, wiel, gat):
	rr = 3.05 if ring == 96 else 3.3
	factor = 2.0 * math.pi / ring
	startpos = 0
	cs = math.cos(factor * startpos)
	sn = math.sin(factor * startpos)
	ggdringwiel = gcd(ring, wiel)
	ggdringwiel = 12
	wr = wiel / ring
	a = 1.0 - wr
	b = wr - (AA * (gat - 1.0) + BB) / rr
	tmax = wiel / ggdringwiel * math.pi * 2.0 + stap
	t = 0.0
	prev_x = None
	while t < tmax:
		x = a * math.cos(t) + b * math.cos(t - t / wr)
		y = a * math.sin(t) + b * math.sin(t - t / wr)
		plot_x = width / 2 + size * (cs * x + sn * y)
		plot_y = height / 2 + size * (-sn * x + cs * y)
		if prev_x is not None:
			display.drawLine(prev_x, prev_y, plot_x, plot_y, display.BLUE)
		prev_x = plot_x
		prev_y = plot_y
		t += stap

display.drawFill(display.WHITE)
spirofig(96, 60, 1)
spirofig(96, 60, 3)
spirofig(96, 60, 5)
display.flush()

