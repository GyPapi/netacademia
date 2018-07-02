
import time
import random
print("randomPerline code starting")

filehandle = open("randoms.txt", 'w')
random.seed(3)
asd = 1
for x in range(0, 10):
    time.sleep(0.3)
    y = random.randint(0, 100)
    print("loop number: " + str(x) + " random number = " + str(y))
    filehandle.write("Random number, index: " + str(x) + " randomNumber: " + str(y) + " \n\n")


filehandle.close

print("end")