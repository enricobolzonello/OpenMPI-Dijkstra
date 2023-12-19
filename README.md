<br/>
<p align="center">
  <h3 align="center">Parallel Dijkstra with OpenMPI</h3>

  <p align="center">
    Parallel implementation of the famous Dijkstra's algorithm for the Single-Source Shortest Path (SSSP) problem.
    <br/>
    <br/>
  </p>
</p>

![Contributors](https://img.shields.io/github/contributors/enricobolzonello/OpenMPI-Dijkstra?color=dark-green) ![Stargazers](https://img.shields.io/github/stars/enricobolzonello/OpenMPI-Dijkstra?style=social) ![Issues](https://img.shields.io/github/issues/enricobolzonello/OpenMPI-Dijkstra) ![License](https://img.shields.io/github/license/enricobolzonello/OpenMPI-Dijkstra) 

## Table Of Contents

* [About the Project](#about-the-project)
  * [Repository Organisation](#repository-organisation)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Usage](#usage)
* [Authors](#authors)
* [References](#references)

## About The Project

Project for the Parallel Computing course 2022/23, MSc Computer Engineering @ UniPD. 

The aim of the project is to write a parallel version of an algorithm and compare the performance against the sequential version. The _project report_ with full explanation of the code and the results can be found [here](Parallel_Dijkstra_paper.pdf)

### Repository Organisation

The repository is organised as follows:

* `cmake`: this folder contains CMake files for finding Valgrind and Check [reference](https://github.com/vndmtrx/check-cmake-example)
* `datasets`: this folder contains the datasets used for the results in the report
* `libs/star`: this folder contains the implementation for the star datastructure, as described in [[1]](#1) 
* `src`: this folder contains the source code for both sequential and parallel implementatio of Dijkstra's algorithm:
  * `parallel`: contains both star and matrix implementation, currently the star implementation doesn't work correctly
  * `sequential`: contains both working star and matrix implementation
  * `utils`: general utilities both for matrix and star implementations
* `tests`: this folder contains the final paper submitted to CLEF.

## Getting Started

This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.

### Prerequisites

This project requires OpenMPI and CMake in order to run.

### Installation

1. Clone the repo

```sh
git clone https://github.com/enricobolzonello/OpenMPI-Dijkstra.git
```

2. Install OpenMPI. On MacOS it can be done with homebrew:

```sh
brew install open-mpi
```

3. Run CMake

```sh
cmake -S . -B build
```

4. Move on the `build` folder and call make
```sh
cd build && make
```

5. If you want you can run tests with: 
```sh
make test
```

## Usage

The command to run the parallel Dijkstra is: 
```sh
mpirun -np <processors> <dataset file path>
```

## Authors

* **[Bolzonello Enrico](https://github.com/enricobolzonello)** - *MSc Computer Engineering Student @ UniPD*

## References
<a id="1">[1]</a> 
Fischetti, M. (2019). 
Introduction to Mathematical Optimization, 130-132
