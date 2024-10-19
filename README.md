
# Character Distribution Program - Programming Assignment

This repository contains the implementation for **CS/SE 3345 Programming Assignment 2 and 3**, focusing on the **Character Distribution Program**. The assignment involves generating character sequences that mimic input text, using **AVL trees** and **Hash Tables** for character storage and retrieval.

## Project Description

The goal of the program is to generate random text based on character distributions learned from an input text (Shakespeare's *The Merchant of Venice*). Two separate implementations are provided:
1. **AVL Tree Implementation** (in `avl_main.cpp`)
2. **Hash Table Implementation** (in `hash_main.cpp`)

### Features

- **Random Character Generation:** After analyzing the input text, the program stores character sequences and their frequencies. It then generates random text based on these frequencies.
- **Custom Data Structures:** Two data structures are used to store the character data:
  - An **AVL Tree**: A self-balancing binary search tree.
  - A **Hash Table**: Using linear probing to resolve collisions.
- **Efficient Retrieval:** Both implementations support quick insertion and retrieval of characters based on the learned distributions.

### Files

- `avl_main.cpp`: Implements the AVL Tree version of the program.
- `hash_main.cpp`: Implements the Hash Table version of the program.
- `merchant.txt`: Input text file containing the *Merchant of Venice* (not included here but required to run the program).
- `out.txt`: Generated output file containing random text based on the character distributions.

## How to Run

1. **Compile the program**:
   ```bash
   g++ avl_main.cpp -o avl_program -std=c++20
   g++ hash_main.cpp -o hash_program -std=c++20
   ```

2. **Run the AVL Tree Program**:
   ```bash
   ./avl_program
   ```

3. **Run the Hash Table Program**:
   ```bash
   ./hash_program
   ```

### Input

Both programs require:
- **Window size**: A positive integer representing the number of characters to consider when generating sequences.
- **Output length**: A positive integer representing the number of characters to generate in the output text.

### Output

The programs will generate an `out.txt` file containing the random text generated based on the input text's character distributions.

## Additional Information

- The **AVL Tree** ensures efficient O(log n) time complexity for insertion and retrieval, while maintaining balance after every insertion.
- The **Hash Table** uses a custom hash function and linear probing for collision resolution, ensuring efficient O(1) average time complexity for insertion and retrieval.
- Both implementations include detailed comments for clarity and ease of understanding.
