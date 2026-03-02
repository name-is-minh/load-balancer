# Project 3 - Load Balancer

A C++ simulation of a dynamic load balancer that distributes network requests across multiple web servers with automatic scaling and firewall protection.

## Overview

This project simulates a load balancer system that manages incoming network requests and distributes them among a pool of web servers. The system features:

- **Dynamic Server Scaling**: Automatically adds or removes servers based on queue size
- **IP-Based Firewall**: Blocks requests from predefined malicious IP ranges
- **Request Queue Management**: Efficiently handles pending requests using a FIFO queue
- **Comprehensive Logging**: Tracks all system events and generates detailed statistics
- **Colored Console Output**: Visual feedback for different system events

**Note**: 

This project is implemented based on the design specifications provided at: https://people.tamu.edu/~mhnguyen/csce412_project3/

Some modifications have been made to the original design.

## Features

### Core Functionality

- Simulates web servers processing requests with variable processing times
- Random request generation to simulate realistic traffic patterns
- IP address validation and blocking (blocks private IP ranges: 192.168.x.x and 10.x.x.x)
- Real-time monitoring of server availability and request completion

### Auto-Scaling Logic

- **Scale Up**: Adds a new server when `queue_size > 80 × active_servers`
- **Scale Down**: Removes idle servers when `queue_size < 50 × active_servers`
- Evaluates scaling conditions every 30 clock cycles
- Maintains at least one active server at all times

### Statistics Tracking

- Total requests processed
- Requests blocked by firewall
- Servers dynamically added/removed
- Final queue size and active server count
- Complete event log with timestamps

## Building the Project

### Prerequisites

- C++ compiler with C++17 support (g++ recommended)
- Make build system
- Doxygen (optional, for documentation generation)

### Compilation

```bash
make
```

This creates the `loadbalancer` executable.

### Cleaning Build Files

```bash
make clean
```

Removes compiled object files, the executable, and log files.

## Running the Simulation

```bash
./loadbalancer
```

You'll be prompted to enter:

1. **Number of servers**: Initial server count (e.g., 5)
2. **Run time**: Simulation duration in clock cycles (e.g., 10000)

### Example Session

```
Enter number of servers: 5
Enter the time you want to run the load balancer: 10000
Server A got request from 45.123.87.201 | time=45
Server B got request from 98.201.15.73 | time=62
...
Added server F at time=120. Total active servers: 6
Server A finished request at cycle 165
...
--------------------------------------------------
SUMMARY
--------------------------------------------------
Total clock cycles run       : 10000
Requests blocked             : 142
Requests processed           : 4832
Requests remaining in queue  : 23
Servers added dynamically    : 3
Servers removed dynamically  : 1
Final active server count    : 7
```

## Documentation

Generate HTML documentation using Doxygen:

```bash
doxygen Doxyfile
```

View the documentation by opening `html/index.html` in a web browser.

My current documentation can be found here: https://people.tamu.edu/~mhnguyen/csce_412_project3_docs/

## AI Usage Disclosure

**Note**: AI assistance was used in the development of this project for:

- Creating this README.md
- Generating all Doxygen documentation comments
- Creating inline code comments
- Formatting color to terminal outputs
- Implementing small utility functions
