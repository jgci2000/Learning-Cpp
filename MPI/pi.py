import numpy as np

def f(x, x_max, num_steps): 
    step = 1 / N
    pi = 0
    
    while x_max >= x:
        pi += 4 / (1 + x ** 2) * step
        x += step
    
    return pi

a = 0
b = 1
N = 100000

pi = f(a, b, N)

print(pi)
