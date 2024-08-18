#!/usr/bin/env python3
import os
from subprocess import Popen, PIPE

BUILD_DIR = "build"
DATA_DIR = "data"
GENERATOR = "data_gen"
JUDGE = "solution_cost"
SOLUTIONS = ["mettu_plaxton", "facility_set"]

SIZES = [50]
DIMENSIONS = [2, 10]
COST = 1

def gen(size, dimension, cost) -> str:
    filepath = os.path.join(DATA_DIR, f"gen_n{size}_d{dimension}.in")
    process = Popen(
        [os.path.join(BUILD_DIR, GENERATOR)],
        stdin=PIPE,
        stdout=open(filepath, "w")
    )

    process.communicate(f"{size} {dimension} {cost}\n".encode())
    assert process.returncode == 0

    return filepath


def solve(input_path, solution) -> str:
    output_path = input_path.removesuffix(".in") + f".{solution}.out"
    process = Popen(
        [os.path.join(BUILD_DIR, solution)],
        stdin=open(input_path),
        stdout=open(output_path, "w")
    )
    rc = process.wait()
    assert rc == 0

    return output_path


def judge(input_path: str, output_path: str) -> float:
    process = Popen(
        [os.path.join(BUILD_DIR, JUDGE)],
        stdin=open(input_path),
        stdout=PIPE,
        env={"SOLUTION" : output_path}
    )
    return process.communicate()[0].decode().strip()



os.makedirs(DATA_DIR, exist_ok=True)
for size in SIZES:
    for dimension in DIMENSIONS:
        inp = gen(size, dimension, COST)
        for solution in SOLUTIONS:
            print(f"{inp:<20} {solution:15}", end=" ")
            out = solve(inp, solution)
            print(judge(inp, out))
