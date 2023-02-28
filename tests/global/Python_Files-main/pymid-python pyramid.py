# Contributed by endrasww

while True:
    try:
        high = int(input(" Please enter the expected high for the python pyramid! ")) + 1
    except ValueError:
        print("Invalid input, expecting integer value!")
    else:
        break

block = input(" Please choose your pyramid block! Select between X, *, -, and +. The default is X. ") + " "
if block not in ['X ', '* ', '- ', '+ ']:
    print("Invalid input for pyramid block, continue with default block (X).")
    block = 'X '

for i in range(1, high):
    if i % 2:
        print("{:^150}".format(block*i))
    else:
        print("{:^150}".format(block*i))
