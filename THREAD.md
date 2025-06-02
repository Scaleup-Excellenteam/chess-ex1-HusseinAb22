# Multithreaded Chess AI

A C++ chess game that uses multithreading to speed up move calculations. The AI evaluates possible moves in parallel using a thread pool to find the best move faster.

## Features

- **Thread Pool**: Configurable number of worker threads for parallel move evaluation
- **Thread-Safe Operations**: Uses mutexes to safely share data between threads
- **Performance Benchmarking**: Automatic game mode to measure speedup with different thread counts
- **Minimax Algorithm**: Uses minimax with alpha-beta pruning for move evaluation

## How It Works

1. **Generate Moves**: Find all legal moves for the current player
2. **Distribute Work**: Send each move evaluation to the thread pool
3. **Parallel Evaluation**: Worker threads evaluate moves using minimax algorithm
4. **Collect Results**: Thread-safe priority queue stores the best moves

## Building and Running

### Prerequisites
- C++17 compatible compiler
- CMake

### Build Steps
```bash
mkdir build
cd build
cmake ..
make
```

### Run
```bash
./chess_game
```

The program will ask for:
- Search depth (how many moves ahead to look)
- Game mode (automatic for benchmarks, manual for playing)

## Performance

The multithreaded version shows significant speedup compared to single-threaded execution:
- Best performance typically achieved with threads equal to CPU cores
- Diminishing returns beyond physical core count due to overhead
- Thread-safe operations minimize but don't eliminate synchronization costs

## Technical Details

- **Synchronization**: `std::mutex` and `std::lock_guard` protect shared resources
- **Thread Management**: Custom thread pool using `std::thread` and `std::condition_variable`
- **Task Distribution**: Each possible move becomes a separate task for parallel evaluation
- **Result Aggregation**: Thread-safe priority queue collects evaluated moves