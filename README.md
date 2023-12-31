# AOC 2023
Attempts for AOC 2023. Code is done first in CPP for practice, and then done in Python.

## Day 1
* Learning to compile cpp programs locally using `C++ 20`
* Discoving `find()` and `rfind()` are a thing
* Familiarising myself with pointers and aliases, vectors and maps
* Identifying characters that are digits and working with characters as integers

## Day 2
* Learning to split a string by a separator in cpp

## Day 3
* Finding an efficient way to solve the problem

## Day 4
* Misunderstanding part 2 for an hour :<

## Day 5
* Learning to work with bigger numbers by using `unsigned int` and `unsigned long` to prevent overflow
* Coming up with a strategy for part 2 and deciding to reverse engineer the ranges of seeds that correspond to ranges of locations, sorted by ascending location

## Day 6
* Learning to work with even larger numbers by using `long long` and `double` to prevent overflow

## Day 7
* Deciding to try out enums in cpp for the first time to represent the hand types

## Day 8
* Figuring out that I could use LCM for part 2

## Day 9
* Fairly simple day today! ^-^

## Day 10
* Implementing BFS in cpp
* Using structs to represent the tiles

## Day 11
* Being off-by-one and suffering for it :<

## Day 12
* Implementing memoisation in cpp
* Getting stuck for a bit on the non-brute-force method for finding the number of possible combinations for a specific sequence because of stupid mistakes
* More integer overflow
* Got interrupted on this one due to a plane flight

## Day 13
* Will be overseas for the remaining days on holiday and doing these at night or on trains, let's see how it goes!
* Relatively easy day today

## Day 14
* Finding the point at which the dish state repeats: a lot like getting stuck on these types of puzzles in old-school RPGs

## Day 15
* Doing this one while really tired, but it wasn't too bad (except for reading comprehension on part 2)
* Forgot to reset the box number for a bit though

## Day 16
* More bfs

## Day 17
* Implementing Dijkstra's in cpp 
* Using the cpp priority queue
* Using a struct as the key for a map
* Failing on the memoisation for a bit

## Day 18
* Optimising my solution for part 1 so it could run for part 2 by using a priority queue
* There's an easier mathematical solution T_T

## Day 19
* Tried to do it in a more OOP manner by using structs to represent the parts
* Had a bit of a problem with part 2 initially as I wasn't aware the ranges for each individual part do not overlap

## Day 20
* Did part 1 in an OOP manner with polymorphism by representing each module type as a child class of a module parent class
* Tried brute forcing part 2 for a bit before learning that I had to use LCM

## Day 21
* Did part 1 with bfs that memoised based on both coordinates and path length
* For part 2, I optimised the bfs to memoise only based on coordinates since any point reached with an even number of steps remaining will be reachable by the end

## Day 22
* Mostly just brute forced part 1, though I ran into a minor issue where I missed out some of the cases where a block can support another
* Part 2 uses a traversal of the tower

## Day 23
* Part 1 used BFS and noted the direction a tile was entered from to prevent loops
* For part 2, all junctions were found and DFS between the entrance to the exit through these junctions were done

## Day 24
* Part 1 just involved formulating a simple two unknown, two equation simultaneous equation and solving it
* For part 2, I learned how to use Z3 with cpp to solve the more complicated simultaneous equations

## Day 25
* Learned how to implement Karger's (Had learned the algo in class before but never used it practically)

![end](img/end.png)

I took an additional four days after Christmas to complete AOC'23, but I managed to finish all parts before my flight back home :D<br/>
I didn't have enough time to keep up doing both Python and cpp versions, so for most days I ended up doing only the cpp version since I'm trying to learn how to use cpp.<br/>
I learned a lot about cpp syntax and things specific to the language, so this was a great learning experience and it was fairly fun as well, even if I got stumped on a lot of the later days and had to consult the subreddit for hints.

Hopefully I'll get to do AOC'24 too!
