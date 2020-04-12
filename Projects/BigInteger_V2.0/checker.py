#!/usr/bin/python
# -*- coding: utf-8 -*-


def fun(x):
    with open(r"/home/kamil/Experiments/ForWorkOnLinux/Projects/BigInteger_V2.0/out_of_cpp.txt", "r") as inFile:
        a = inFile.readlines()
        if x == '+':
            print(int(a[0]), x, int(a[1]), " = ", int(a[0]) + int(a[1]))
            print("program answer : ", a[2])
            print(int(a[0]) + int(a[1]) == int(a[2]))
        elif x == '-':
            print(int(a[0]), x, int(a[1]), " = ", int(a[0]) - int(a[1]))
            print("program answer : ", a[2])
            print(int(a[0]) - int(a[1]) == int(a[2]))
        elif x == "*":
            print(int(a[0]), x, int(a[1]), " = ", int(a[0]) * int(a[1]))
            print("program answer : ", a[2])
            print(int(a[0]) * int(a[1]) == int(a[2]))
        elif x == "/":
            print(int(a[0]), x, int(a[1]), " = ", int(a[0]) // int(a[1]))
            print("program answer : ", a[2])
            print(int(a[0]) // int(a[1]) == int(a[2]))
        elif x == "%":
            print(int(a[0]), x, int(a[1]), " = ", int(a[0]) % int(a[1]))
            print("program answer : ", a[2])
            print(int(a[0]) % int(a[1]) == int(a[2]))


fun("/")
