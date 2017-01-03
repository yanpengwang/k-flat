K-FLAT:
===========
A sequence {ai} is called k-flat if for any two consecutive elements ai and ai+1, the following holds true: |ai - ai+1| ≦ k. For a given sequence, find the longest subsequence which is k-flat.
Input: (standard input)
+Line 1 contains two numbers n and k, 1 ≦ (n , k) ≦ 100000, where n denotes a number of elements of a sequence.
+Line 2 contains n positive integers - elements of a given sequence, all of them are not greater than 1000000000.

Output: (standard output)
Your program should print out k-flat subsequence with the maximum length. If there are more than one solution, print the one for which the last element has the smallest index, if there are more than one such solutions, print the one for which the next to last has the smallest index, etc.

Example:

For sample input:

 
6 3

2 9 7 2 5 8

a correct output format:

9 7 5 8

Hint: There are two 3-flat subsequences (k = 3) with four elements: 9 7 5 8 and 2 2 5 8. Both ends up with the same element (8), the next to last are also the same (5), but 7 is earlier (has smaller index) than 2 in the above example.

---------------


With regard to the source code under /k-flat:

flat.c - My original C code for the contest

flat.cpp - later added C Plus Plus implementation

flat.py - later added Python implementation
