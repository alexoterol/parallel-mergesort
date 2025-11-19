# Parallel Mergesort with 14 Cores

## Overview
This project implements a **parallelized version of the Mergesort algorithm** in C, utilizing **POSIX Threads** (pthreads). The aim is to demonstrate how parallelism, by taking advantage of a **14-core processor**, can significantly reduce the execution time for sorting large datasets.

## Objective
The main goal of this project is to **implement a scalable concurrent MergeSort algorithm**. It leverages multi-core processors to reduce execution time through parallelism, where the number of cores available (P) is significantly smaller than the size of the dataset (N). The program reads the data to be sorted from either **standard input** (keyboard) or a **CSV file**.

## Features
- **Concurrency**: Uses 14 cores for parallel execution.
- **Scalability**: The program scales based on the number of cores and dataset size.
- **File I/O Support**: Capable of reading data either from standard input or CSV files.
- **Modular Design**: The project is divided into multiple files for clear separation of logic.

## Setup

### Prerequisites
- A **C compiler** that supports pthreads (e.g., **gcc**).
- **POSIX Threads** library.
- Linux or macOS environment (Windows may require WSL or other tools for pthreads support).

### Installation

1. Clone the repository or download the project files.
2. In the terminal, navigate to the project directory.
3. Run `make` to compile the program.
4. Execute the program using `./concurrent_mergesort <input_file>` or provide input from the keyboard.

### Files and Descriptions
- **`Makefile`**: Automates the compilation and execution process.
- **`concurrent_mergesort`**: Main program that drives the parallel mergesort.
- **`csv_gen.py`**: Python script for generating CSV files of numbers for testing.
- **`datos_10000.csv`**, **`datos_1000000.csv`**: Sample input CSV files with 10,000 and 1,000,000 numbers respectively.
- **`main.c`**: Contains the main program logic to read data, call the mergesort, and display the results.
- **`sort_utils.c`**, **`sort_utils.h`**: Utilities for sorting, including thread management and merge logic.
- **`numbers.csv`**: Another sample input file.
  
## Usage

### Run from a CSV file:
```bash
./concurrent_mergesort datos_10000.csv
```
### Run with user input (keyboard):
```bash
./concurrent_mergesort
# Enter a list of numbers separated by commas, e.g., 45,12,78,3
```
## Expected Output

The program will display messages showing the phases of the merge sort (initial parallel sorting and hierarchical merging).

It will then print the sorted result.

## Performance Considerations

The performance of the parallel merge sort depends on the relation between the dataset size (N) and the overhead of thread management. When N is large (e.g., 1,000,000), the parallel fraction is high, and using all 14 cores is very efficient. However, for small datasets, the overhead of managing threads can overshadow the performance benefits.

## Test Scenarios:

- **CSV with 1,000,000 entries**: High acceleration expected.
- **CSV with 10,000 entries**: Moderate acceleration.
- **Console input with a small set of numbers**: Likely results in no acceleration due to thread creation overhead.

## Results and Analysis

The success of the parallel merge sort is highly dependent on the size of the task and the overhead of thread management. For large datasets, the parallel version significantly reduces execution time, while for smaller datasets, the overhead can negate the benefits.

| Scenario | Source         | Data Size (N) | Expected Acceleration | Main Limiting Factor            |
|----------|----------------|---------------|-----------------------|---------------------------------|
| 1        | Large CSV      | 1,000,000     | Very High             | I/O Time and Final Merge        |
| 2        | Small CSV      | 10,000        | Moderate to High      | Thread Management and Merging   |
| 3        | Console Input  | Small         | None or Deceleration  | Thread Creation Overhead        |

## Contributors
- **Alex David Otero Limones** (Author)

