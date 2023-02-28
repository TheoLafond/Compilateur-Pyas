def multiplier_generator(c):
    def multiplier_by_c(x):
        return c*x
    return multiplier_by_c

multiplier_by_5 = multiplier_generator(5)
print(multiplier_by_5(4))
