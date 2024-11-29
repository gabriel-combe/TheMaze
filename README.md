# TheMaze
Maze Project in UE5 (M1 ATI)

[TestDemo](./Pictures/Test_Demo.gif)

## Context
This project is the final project for the course Unreal Engine Blueprint.
It was suppose to be done in UE Blueprint, but I took the challenge to do it in UE C++.

This was a great experience as it made me learn how to use UE C++, its advantages and its limitations (no more widget in C++). Despite seeming verbose at first, I mostly (still learning all it) understand what all the macros are for and how to use them.

However there is nearly no documentation on UE C++, so I had a rough start, but after some research and practice it became easier and easier to know what do to and use in specific context.
As it is my very first UE C++ project, the code surely contains errors or bad coding practice, if you spot any feel free to tell me, any advices are welcomed!

## Algorithm
For the main maze generation algorithm I used the one by [Captain Luma](https://github.com/CaptainLuma/New-Maze-Generating-Algorithm). I added to it the wall and floor generation, as well as the tracking of dead end in the maze. I also made a small algorithm that swap the items in a dead end at runtime (when the maze evolve one step), as I noticed that only 3 nodes (max) are involved, and that if at the step t-1 the origin node (where the maze start one step evolution) was a dead end, at the step t one of those 3 nodes also should be a dead end. 
I tried my best to optimize those algorithms, but I'm sure we can do better.

## Usage
Just clone or download the repo and start the uproject file and it should work.
If not, try to generate the visual studio project file by right clicking the uproject file and selecting the option to do so. Then try to compile from visual studio.

> There might still be some bug that I forgot, but I will try to fix them

## Meta

[Gabriel Combe-Ounkham](https://github.com/gabriel-combe)