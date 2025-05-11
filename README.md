# Clustering
Implementation of [parallel clustering algorithms](https://arxiv.org/pdf/2307.07848).

## Installation

This project needs a few python libraries to run. Using [python's virtual environment](https://docs.python.org/3/library/venv.html) is recommended.
```bash
pip install matplotlib scikit-learn scikit-learn-extra
```

It is also possible that [`scikit-learn-extra` does not work with `numpy` of too high version](https://github.com/scikit-learn-contrib/scikit-learn-extra/issues/178).
You can then install older `numpy` with:
```bash
pip install -v "numpy<2.0"
```

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
