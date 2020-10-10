import random

with open("/tests.txt", "w") as test_file:
    for i in range(50):
        test_file.write(str(random.randint(0, 100)) + " ")
        test_file.write(str(random.randint(0, 100)) + " ")
        M = random.randint(1, 1000000)
        test_file.write(str(M) + " ")
        test_file.write(str(random.randint(0, M - 1)) + " ")
        test_file.write(str(random.randint(0, M - 1)) + "\n")
