#!/usr/bin/env python3
import argparse
import os
from subprocess import Popen, PIPE
import time

parser = argparse.ArgumentParser(prog='test', description='Script for testing facility set / clustering solution')
parser.add_argument("target", choices=["fl", "cl"])
parser.add_argument("z", type=int, choices=[1, 2])
args = parser.parse_args()

Z = args.z

BUILD_DIR = "build"
DATA_DIR = "data"
GEN_DATA_DIR = "gen"
GENERATOR = f"data_gen_z{Z}"

FACILITY_JUDGE = f"facility_set_cost_z{Z}"
FACILITY_SOLUTIONS = [f"mettu_plaxton_z{Z}"] + [f"facility_set_z{Z}"]*6
FACILITY_SOLUTION_ARGS = [
    [],
    ["grid_hashing", "3c6da5d7"],
    ["grid_hashing", "5b17b966"],
    ["grid_hashing", "9a7aa40"],
    ["face_hashing", "3c6da5d7"],
    ["face_hashing", "5b17b966"],
    ["face_hashing", "9a7aa40"],
]
FACILITY_COST = 1

CLUSTERING_JUDGE = f"clustering_cost_z{Z}"
CLUSTERING_SOLUTIONS = [f"scikit_z{Z}"] + [f"clustering_z{Z}"]*6
CLUSTERING_SOLUTION_ARGS = [
    [],
    ["grid_hashing", "3c6da5d7"],
    ["grid_hashing", "5b17b966"],
    ["grid_hashing", "9a7aa40"],
    ["face_hashing", "3c6da5d7"],
    ["face_hashing", "5b17b966"],
    ["face_hashing", "9a7aa40"],
]
CLUSTER_COUNT = 10

SIZES = [100, 1000, int(1e4), int(1e5)]
DIMENSIONS = [2, 5, 10]

def gen(size: int, dimension: int, k_or_cost: float) -> str:
    filepath = os.path.join(DATA_DIR, GEN_DATA_DIR, f"gen_n{size}_d{dimension}.in")
    process = Popen(
        [os.path.join(BUILD_DIR, GENERATOR)],
        stdin=PIPE,
        stdout=open(filepath, "w")
    )

    process.communicate(f"{size} {dimension} {k_or_cost}\n".encode())
    assert process.returncode == 0

    return filepath

def gen_iris() -> str:
    IRIS_DIR = "iris"
    with open(os.path.join(IRIS_DIR, "iris.data")) as f:
        plants = f.read().strip().split("\n")
    plants = map(lambda p: p.split(","), plants)
    plants = list(filter(lambda p: p, plants))

    inp = os.path.join(DATA_DIR, IRIS_DIR, "iris.in")
    os.makedirs(os.path.dirname(inp), exist_ok=True)
    with open(inp, "w") as f:
        f.write(f"{len(plants)} {len(plants[0])-1} 3\n")
        for plant in plants:
            f.write(" ".join(plant[:-1]) + "\n")

    return inp


def solve(input_path: str, solution: str, args: list[str]) -> tuple[str, float]:
    output_path = input_path.removesuffix(".in") + f".{solution}.{'.'.join(args)}.out"
    start_time = time.time()
    process = Popen(
        [os.path.join(BUILD_DIR, solution), *args],
        stdin=open(input_path),
        stdout=open(output_path, "w")
    )
    rc = process.wait()
    total_time = time.time() - start_time # TODO: Do something better

    assert rc == 0

    return output_path, total_time


def judge(judge: str, input_path: str, output_path: str) -> float:
    process = Popen(
        [os.path.join(BUILD_DIR, judge)],
        stdin=open(input_path),
        stdout=PIPE,
        env={"SOLUTION" : output_path}
    )
    return float(process.communicate()[0].decode().strip())


def test_facility_location(inp: str):
    for solution, args in zip(FACILITY_SOLUTIONS, FACILITY_SOLUTION_ARGS, strict=True):
        print(f"{os.path.basename(inp):20} {solution:20} {' '.join(args):21}", end="  ", flush=True)
        out, sol_time = solve(inp, solution, args)
        result = f"{judge(FACILITY_JUDGE, inp, out):.4f}"
        print(f"{result:>10}  {sol_time:.2f}s")
    print("-"*50)


def test_clustering(inp: str):
    for solution, args in zip(CLUSTERING_SOLUTIONS, CLUSTERING_SOLUTION_ARGS, strict=True):
        print(f"{os.path.basename(inp):20} {solution:20} {' '.join(args):21}", end="  ", flush=True)
        out, sol_time = solve(inp, solution, args)
        with open(out) as f:
            centers = len(list(filter(lambda r: r.strip(), f.readlines())))
        result = f"{judge(CLUSTERING_JUDGE, inp, out):.4f}"
        print(f"{centers:>3}  {result:>10}  {sol_time:.2f}s")
    print("-"*50)

def test(target: str, inp: str):
    if target == "fl":
        test_facility_location(inp)
    elif target == "cl":
        test_clustering(inp)


if __name__ == "__main__":
    os.makedirs(os.path.join(DATA_DIR, GEN_DATA_DIR), exist_ok=True)
    if args.target == "cl":
        for static_gen in [gen_iris]:
            test(args.target, static_gen())

    for size in SIZES:
        for dimension in DIMENSIONS:
            if args.target == "fl":
                inp = gen(size, dimension, FACILITY_COST)
            elif args.target == "cl":
                inp = gen(size, dimension, CLUSTER_COUNT)
            test(args.target, inp)
