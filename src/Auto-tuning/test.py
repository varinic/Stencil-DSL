
import random


print([2**i for i in range(0,10)])

p5 = [2**i for i in range(0,7)]
p6 = [2**i for i in range(0,7)]
print(p5)
print(p6)
ep4 = [i for i in range(1,10)]
ep5 = [i for i in range(1,7)]
ep6 = [i for i in range(1,7)]
print(ep4)
print(ep5)
print(ep6)
for i in range(100):
    ep4 = random.randint(1,10)
    ep1 = random.randint(1,ep4)
    print('ep4:',ep4)
    print('ep4:',ep1)
