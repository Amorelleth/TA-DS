import matplotlib.pyplot as plt

xlist = [
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8
]

t0 = 0.326138

ylist_odd_even = [
    t0 / 0.326138 / 1,
    t0 / 0.241619 / 2,
    t0 / 0.216712 / 3,
    t0 / 0.209032 / 4,
    t0 / 0.265615 / 5,
    t0 / 0.282047 / 6,
    t0 / 0.305178 / 7,
    t0 / 0.329393 / 8,
]

t1 = 1.40722

ylist_shell = [
    t1 / 1.40722 / 1,
    t1 / 0.569239 / 2,
    t1 / 0.448718 / 3,
    t1 / 0.32555 / 4,
    t1 / 0.401578 / 5,
    t1 / 0.365375 / 6,
    t1 / 0.322897 / 7,
    t1 / 0.309429 / 8,
]

# plt.plot(xlist, ylist_odd_even, label='odd-even')
plt.plot(xlist, ylist_shell, label='shell')

plt.xlabel('number of processors')
plt.ylabel('efficiency')

plt.xlim([1, 8])
plt.title("E(p)")
plt.legend()

plt.show()
