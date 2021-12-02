# SAT-lightout

## Introduce of lightout game
The game consists of a m by n grid of lights, where m and n are nature numbers. When the game starts, a random number of these lights is switched on. Pressing any of the lights will toggle it and the adjacent lights. The goal of the puzzle is to switch all the lights off, preferably in as few button presses as possible.

## Goal
* With all lights on, find out whether with any m and n it is solvable or not.
* With any random initial conditions of the plate of lights and with any m and n, find out whether it is solvable or not.

## Usage
### command line
```
make
./lightout
```
### Input format
* with all lights on
```
0             // means not random input
<n> <m>       // size of n and m  
```
* input any random intial condition of light
```
//example
1             // means random input
5 5           // <n> <m> size of n and m
0 1 0 0 1
1 0 1 0 1
1 1 1 0 0
0 0 0 0 1
0 1 1 1 0
```

## Output
If "SAT", means it is solvable, and it will show how can we press the light to switch all the light off.  
If "UNSAT", then it is not solvable.  
eg.   
```
// this is the answer of 18*19 with all light on
// 0 means do not need to press that light
// 1 means need to press it
SAT
0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0
1 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 1
1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1
0 0 0 1 1 0 1 1 1 0 1 1 1 0 1 1 0 0 0
0 0 0 1 1 1 1 0 1 1 1 0 1 1 1 1 0 0 0
1 1 0 0 1 0 0 0 0 0 0 0 0 0 1 0 0 1 1
1 1 0 1 1 1 0 1 0 0 0 1 0 1 1 1 0 1 1
0 0 1 1 1 1 1 0 0 1 0 0 1 1 1 1 1 0 0
0 1 1 1 1 1 1 1 0 0 0 1 1 1 1 1 1 1 0
0 1 1 1 1 1 1 1 0 0 0 1 1 1 1 1 1 1 0
0 0 1 1 1 1 1 0 0 1 0 0 1 1 1 1 1 0 0
1 1 0 1 1 1 0 1 0 0 0 1 0 1 1 1 0 1 1
1 1 0 0 1 0 0 0 0 0 0 0 0 0 1 0 0 1 1
0 0 0 1 1 1 1 0 1 1 1 0 1 1 1 1 0 0 0
0 0 0 1 1 0 1 1 1 0 1 1 1 0 1 1 0 0 0
1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1
1 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 1
0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0
```
