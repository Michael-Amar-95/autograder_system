# Autograder System

## Overview

This project implements a lightweight **automated execution and grading system** for C programs, inspired by real-world CI pipelines and online judge platforms.

The system is built using **Linux system programming concepts**, including:
- Process creation and management (`fork`, `exec`, `wait`)
- File descriptor manipulation (`dup2`)
- Input/output redirection (stdin / stdout)
- Execution monitoring and timeout handling
- Output comparison and semantic similarity detection

It provides a full pipeline that compiles, runs, and evaluates user-submitted programs in an automated and controlled environment.

---

# System Architecture

The project is divided into two main components:

1. **File Comparison Engine**
2. **Automated Grading Pipeline**

---

# Part 1 – File Comparison Engine

## Purpose

This module compares two files and determines whether they are:

### Identical (return code: `1`)
- Exact character-by-character match

### Similar (return code: `3`)
- Same content after normalization:
  - Case-insensitive comparison
  - Ignoring whitespace differences
  - Ignoring newline variations

### Different (return code: `2`)
- Files differ beyond normalization rules

---

## Usage

```bash
./compare file1.txt file2.txt
echo $?
```

---

# Part 2 – Automated Grading Pipeline

## Purpose

This module automates compilation, execution, and evaluation of multiple C programs based on a configuration file.

It simulates a simplified **judge system** used in programming contests.

---

## Configuration File Format

```
<students_directory>
<input_file>
<expected_output_file>
```

---

## Execution Flow

For each student:

### 1. Source File Discovery
- Search only top-level directory
- Locate `.c` file
- Ignore unrelated files

If missing:
```
NO_C_FILE
```

---

### 2. Compilation Stage

- A child process is created using `fork()`
- Compiler is executed using `exec()`
- Parent waits using `wait()`

If compilation fails:
```
COMPILATION_ERROR
```

---

### 3. Execution Stage

Each program runs in an isolated process:

- `fork()` creates a child process
- `exec()` replaces process image with student program
- `dup2()` redirects:
  - stdin → input file
  - stdout → output file

Execution is time-limited.

If runtime exceeds 5 seconds:
```
TIMEOUT
```

---

### 4. Output Evaluation

Program output is compared to expected output using the File Comparison Engine:

| Result | Meaning | Score |
|--------|--------|-------|
| EXACT | identical output | 100 |
| SIMILAR | normalized match | 75 |
| WRONG | mismatch | 50 |

---

# Final Grading Rules

| Status | Description | Score |
|--------|------------|-------|
| NO_C_FILE | No C file found | 0 |
| COMPILATION_ERROR | Compilation failed | 10 |
| TIMEOUT | Execution exceeded time limit | 20 |
| WRONG | Output mismatch | 50 |
| SIMILAR | Output is similar | 75 |
| EXCELLENT | Output is identical | 100 |

---

# Output Format

The system generates a CSV report:

```
results.csv
```

Format:
```
student_name,score,reason
```

Example:

```
Monica,100,EXCELLENT
Phoebe,0,NO_C_FILE
Rachel,20,TIMEOUT
Ross,10,COMPILATION_ERROR
Joey,50,WRONG
Chandler,75,SIMILAR
```

---

# Key System Concepts

## Process Management
- `fork()` → create isolated process
- `exec()` → run external program
- `wait()` → synchronize execution

## I/O Redirection
- `dup2()` used for:
  - Redirecting stdin from input file
  - Redirecting stdout to output capture file

## Execution Control
- Timeout enforcement
- Process isolation per student program

---

# Features

- Automated compilation pipeline
- Secure process execution model
- Input/output redirection system
- File similarity detection
- Timeout handling
- CSV report generation
- Modular grading logic

---

# Technologies

- C
- Linux System Calls
- UNIX Process Model
- File Descriptor Manipulation
- Low-level I/O
- Systems Programming Concepts

---

# Design Philosophy

The system was designed to simulate real-world infrastructure used in:
- Online judges
- CI/CD pipelines
- Automated testing systems

It emphasizes:
- Isolation
- Deterministic execution
- Reproducibility
- Low-level OS control
