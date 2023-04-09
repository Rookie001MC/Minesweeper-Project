_this thing lived without a proper Readme for 3+ months, now I feel lazy af_

# RookieSweeper on Console

_because RookieNguyen did not know how to code a GUI yet help_

## Introduction

This is the first University project I have ever done, a game of Minesweeper, written fully in C++, for CS161 - Introduction to Computer Science.

This is one of my proudest project I have even done, even though admittedly everyone else who did the project outclassed me since they literally coded it with a proper GUI.

At least I tried to make it as cross-platform as possible.

<sub>i feel sad sometimes</sub>

## Full Report

[right here](./Project%20Report%20-%20CS161.docx)

## Building Instructions (from the report)

### Notes:

This game tried to be as cross platform as possible, while only using Standard Template Libraries, so this can be compiled and run on Windows and Linux (I couldn’t test on MacOS because I do not have any MacOS device).

### On Windows:

- Requirements:

  - Visual Studio 2022 or later with the “Desktop Development with C++” workload

- Double click on the .sln file to open the project

- Click on Build -> Build Solution to build

- The resulting output will be in `release/Windows-x64/RookieSweeper-Windows-x64.exe`.

### On Linux:

- Either use the provided `build-linux.sh` script:

  - Enable script to be executable:

  ```bash
  chmod +x `build-linux.sh
  ```

  - Run the script:

  ```bash
  ./build-linux.sh
  ```

- Or compile manually:

  - Execute command to build the project:

  ```bash
  g++ -o ./release/Linux-x64/RookieSweeper-Linux-x64 ./src/*.cpp -std=c++20
  ```

- In either case, the resulting output will be in `release/Linux-x64/RookieSweeper-Linux-x64`.

## How did it go?

Suffice to say, it was fine enough, not the best, but I'm not entirely good at coding either, so it's kinda basic.

This took me 12 weeks, which is the typical duration of the course, although technically it could have taken me shorter, like, 6 weeks I guess.

The cause: I procrastinated for quite a long time.

There is one problem that I got stuck for so long, is that of the [Flood-fill algorithm](https://en.wikipedia.org/wiki/Flood_fill). I got stuck on it for so long, that I ended up delayed finishing it. It's only when I got the insanely flopped Midterm result that I actually determined to finish it, because I ain't gonna let my final result go bad 🥲.

## What I learned (took from the report)

- Planning and structuring a project.
- (Re)learning C/C++.
- Solving some hard problems.
- A somewhat OK grasp of Recursion, call stack and how not to cause a stack overflow; flood-fill algorithm.
- Google is still your best friend in coding.
  - Though, not really anymore, thanks to ChatGPT and Github Copilot 🥲.
  - That being said, I still think it's better to use Google and StackOverflow.

## What needed to be improved (also from the report)

- **_Algorithms!_**
- Simplify parts of code to avoid heavy nesting.
- Better code and project structure.

## What needed to be fixed

- The **_BIGGEST_** problem now for me, is the lack of motivation doing anything, especially right now, as of writing this Readme (10/04/2023). In fact, I'm completely losing my grip on so many courses, that I'm not only failing General Physics 2, but also Calculus 2 and Introduction to Computer Science 2.
- So far, it's still a massive challenge, but maybe it'll turn out fine, hopefully. It'll just be easy, hopefully. Probably. It may not.
