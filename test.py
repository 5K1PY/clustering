#!/usr/bin/env python3
import os
from subprocess import Popen, PIPE
import time

Z = "2"

BUILD_DIR = "build"
DATA_DIR = "data"
GENERATOR = f"data_gen_z{Z}"
JUDGE = f"solution_cost_z{Z}"
SOLUTIONS = [f"mettu_plaxton_z{Z}"] + [f"facility_set_z{Z}"]*6
SOLUTION_ARGS = [
    [],
    ["grid_hashing", "3c6da5d7"],
    ["grid_hashing", "5b17b966"],
    ["grid_hashing", "9a7aa40"],
    ["face_hashing", "3c6da5d7"],
    ["face_hashing", "5b17b966"],
    ["face_hashing", "9a7aa40"],
]

SIZES = [100, 1000, int(1e5), int(1e6)]
DIMENSIONS = [2, 5, 10]
COST = 10

def gen(size: int, dimension: int, cost: float) -> str:
    filepath = os.path.join(DATA_DIR, f"gen_n{size}_d{dimension}.in")
    process = Popen(
        [os.path.join(BUILD_DIR, GENERATOR)],
        stdin=PIPE,
        stdout=open(filepath, "w")
    )

    process.communicate(f"{size} {dimension} {cost}\n".encode())
    assert process.returncode == 0

    return filepath


def solve(input_path: str, solution: str, args: list[str]) -> tuple[str, float]:
    output_path = input_path.removesuffix(".in") + f".{solution}.out"
    start_time = time.time()
    process = Popen(
        [os.path.join(BUILD_DIR, solution), *args],
        stdin=open(input_path),
        stdout=open(output_path, "w")
    )
    rc = process.wait()
    total_time = time.time() - start_time

    assert rc == 0

    return output_path, total_time


def judge(input_path: str, output_path: str) -> float:
    process = Popen(
        [os.path.join(BUILD_DIR, JUDGE)],
        stdin=open(input_path),
        stdout=PIPE,
        env={"SOLUTION" : output_path}
    )
    return float(process.communicate()[0].decode().strip())



os.makedirs(DATA_DIR, exist_ok=True)
for size in SIZES:
    for dimension in DIMENSIONS:
        inp = gen(size, dimension, COST)
        for solution, args in zip(SOLUTIONS, SOLUTION_ARGS, strict=True):
            print(f"{inp:20} {solution:20} {' '.join(args):21}", end="  ", flush=True)
            out, sol_time = solve(inp, solution, args)
            print(f"{judge(inp, out):.4f}  {sol_time:.2f}s")
        print("-"*50)
