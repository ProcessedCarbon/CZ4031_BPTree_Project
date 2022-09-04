# CZ4031_BPTree_Project

## Setup

- Clone this repository using `git clone <REPO_URL>`
- `cd` to repository and open it using `code .`
- Ensure that you have a C++ compiler. (clang++/g++ for MAC, not sure for Windows.)
- Ensure all C++ files are included in compilation by:

  VSCode Instructions:
  Include the following `args` in `tasks.json` to ensure all C++ files are compiled on build:

  ```
  "tasks": [
    {
      "args": [
        "-g",
        "-Wall",
        "-Wextra",
        "-Wpedantic",
        "${fileDirname}/*.cpp",
        "-o",
        "${fileDirname}/${fileBasenameNoExtension}",
        "-std=c++11"
      ],
    }
  ]
  ```
- Select `run build task` and select your C++ Compiler.
- After compilation open a new terminal, `cd` to `src` folder using the command `cd src`
- Run code using `./main`.

## Development Notes

1. There should be some warnings, but for now it can be ignored, it should not affect the compilation.
2. If the compilation is successful, that should be the following user input prompt: <br> `Select Block Size (Enter 1 or 2): ` <br> 

## List of contributors: 

| Name      |  Github Profile  |
| ------------- |:-----:|
| Joshua     | [suenalaba](https://github.com/suenalaba) |
| Gregory      | [ProcessedCarbon](https://github.com/ProcessedCarbon) |
| Ryan      | [raiseu](https://github.com/raiseu) |
| Joei    | [jyzy-here](https://github.com/jyzy-here) |
| Chee Yang    | [raiseu](https://github.com/sleepy-pan-da) |