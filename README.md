# Magic-Squares-Project
## Background
A magic square, somewhat similar to sudoku, is a matrix of integers in which all rows and columns of the matrix add up to the same number. A traditional magic square (the type that this algorithm focusses on) of a size n * n will contain the digits 1 to n^2, and each row will add up to (n^3 + n) / 2. eg. 

1 | 5 | 9	
--|---|--
6 | 7 | 2	
8 | 3 |	4

This algorithm is designed to take some input integer n, generate all possible magic squares of a size n * n and save them to a text file. I originally envisioned idea of a "magic square generator" back in year 9, around the same time I started learning C++, using what I learnt to implement a crude brute-force approach and lay the groundwork for optimisations. I came back to the project several times throughout year 10 and 11 (usually during the holidays) to make improvements as my knowledge grew, reaching a point in year 11 where I thought I had made it as efficient as I could with expertise at the time.

Having lost the code a while ago in the transition between laptops (thought I was done with the project), I decided to revive the project earlier this year and start from scratch with a deeper knowledge of software design. It is currently a work in progress that I have had to put on hold in order to focus on assignments for a little while, but it has almost reached the level of efficiency that I had achieved in year 11. 

I quite a few plans for further optimisation: algorithmic improvements that will reduce runtime by several orders of magnitude, as well as plans for multithreading and potentially even running the algorithm on the GPU using CUDA. If sufficient efficiency is achieved, I also want to expand the algorithm to generate magic cubes of a size n * n * n, and then eventually d dimensional magic hyper cubes of a size n^d.

## Implementation
(Following explanation is in reference to magic squares of size 4 * 4)

Rather than generating every possible permutation of the matrix's element and checking if each row and column sum to the correct value, this algorithm builds the magic square segment by segment, ensuring each segment's sum is correct before building the next. It currently goes about doing this in somewhat of a "closing shell" approach:

1 | 1 | 1 | 1	
--|---|---|--
2 | 3 | 3 | 3	
2 | 4 |	5 | 5
2 | 4 | 6 | 7

finding a combination of 4 elements from the total set of [1, 16] that sum to 34 for the top row (segment 1), then 3 elements that complete the left-most column's sum (segment 2), then 3 elements to complete the second row's sum (segment 4), and so on and so forth until the entire square is complete and is printed to the text file. 

This process of completing one segment before moving onto the next operates recursively, with each segment tested with every possible combination of available elements (those not used by previous segments) for validity, each valid combination permuted, and each permutation transitioning to the next segment to complete the same combination and permutation process.

//TODO further explanation of changes
