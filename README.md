# CZ4031_BPTree_Project

## Setup

- Clone this repository using `git clone <REPO_URL>`
- `cd` to repository and open it using `code .`
- Ensure that you have a C++ compiler. (clang++/g++ for MAC, not sure for Windows.)
- `run build task` and select your C++ Compiler.
- Ensure all C++ files are included in compilation.

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

- After compilation open a new terminal, `cd` to `src` folder using the command `cd src`
- Run code using `./main`.

Development Note:

1. There should be some warnings, but for now it can be ignored, it should not affect the compilation.
2. Successful running of file should print `Select Block Size (Enter 1 or 2): ` in the console as a user input prompt.
