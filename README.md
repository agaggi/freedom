# Freedom Board Game

## Compilation

A `makefile` is included, so the application can be compiled with `make`.
Upon compiling, an executable file `freedom` will be created.

To run the file, **one argument is required**, which sets the board difficulty.
The following arguments are valid:

* easy - sets the board dimensions to **8x8**
* normal - sets the board dimensions to **10x10**

### Windows Only

If you wish to compile this program on Windows, you will not be able to use the
`makefile`, so you will need to enter the command yourself. For example, with the
[MinGW](https://sourceforge.net/projects/mingw-w64/) compiler:

```
g++ -std=c++17 -O3 main.cpp src\*.cpp -o freedom
```

Powershell and cmd do not display unicode by default. Before running the program
you will need to enter the following in your command prompt:

```
chcp 65001
```

## About

Information about the game can be found here:
https://boardgamegeek.com/boardgame/100480/freedom

Freedom is a 2-player board game having objective of getting exactly
**4** of your stones in sequence, either horizontally, vertically or
diagonally. If a player does so, they get a point. For example:

```
  0 1 2 3 4 5 6 7
0 ● * * * * * * *
1 * ● * * * * ● *
2 * * ● * * * ● *
3 * * * ● * * ● *
4 * * * * * * ● *
5 ● ● ● ● * * * *
6 * * * * * * * *
7 * * * * * * * *

Last placed: (0, 0)

● Player 1: 3
○ Player 2: 0
```

If a player has **5 or more** stones in a sequence, they **lose a
point**.

### Freedom Mechanic

The term *freedom* refers to a particular state of the board:

> If all cells adjacent to the last opponent's stone are occupied then
> the player gets the right ("freedom") to place his stone on any empty
> cell of the board.

For example:

```
  0 1 2 3 4 5 6 7
0 ○ ● * * * * * *
1 ● ○ * * * * * *
2 * * * * * * * *
3 * * * * * * * *
4 * * * * * * * *
5 * * * * * * * *
6 * * * * * * * *
7 * * * * * * * *

Last placed: (0, 0)
```

**As a result, player 1 can place their next piece where there is an
empty space**.

### A.I.

In this program, you play against A.I. that makes use of the **Minimax**
algorithm, which aims to *minimize* your score and *maximize* its score.
The algorithms also makes use of **Alpha-Beta pruning**, which allows the
algorithm to skip checking unncecessary scenarios.
