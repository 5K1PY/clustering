# Clustering
Implementation of [parallel clustering algorithms](https://arxiv.org/pdf/2307.07848).

## Usage
First, compile required programs:

```bash
make
```

Then, to test clustering solutions, run the testing script: 
```bash
./test.py {fl,cl} {1,2}
```
Use the first argument to choose whether to test **f**acility **l**ocation or **cl**ustering.

The second argument specifies the cost exponent $z$.
The $z$ power of distance to each point is added to the solution cost.

## Running unit tests
To run unit tests:
```bash
make test
```

## Visualizations

All visualizations can be generated with `make`:
```bash
make visuals
```

Is is also possible to generate specific visualizations. The visualizing script is run:
```bash
./visualizer.py <input> [output]
```
(or use `--help` to get info about even more options)

Additionally, graphs can be separately generated from `results_*.csv` files:
```bash
./graphs.py
```
