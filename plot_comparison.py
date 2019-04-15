
"""
print("g++ -fopenmp madd.cpp -std=c++14 -pthread")
for n in range(10, 100, 1):
	print("./a.out " + str(n))

"""

f = open("stat_mul.txt", "r")
N = []
d_nont = []
d_t = []
for line in f:
	line = line.split(" ")
	N.append(int(line[0]))
	d_nont.append(int(line[1]))
	d_t.append(int(line[2]))


import matplotlib.pyplot as plt
fig = plt.figure("Threading Statistics")
plt.plot(N, d_nont, "r", label = "single-threaded")
plt.plot(N, d_t, "b", label = "multi-threaded")
plt.title("Threaded vs non-threaded matrix addition")
plt.legend(loc='upper left')
plt.xlabel("Square matrix size")
plt.ylabel("Time Taken (ms)")
plt.show()

