# Gemini Assistant Project Guide

Welcome! This file is your central place to configure and guide my actions within this project. By defining your project's specifics here, you help me understand its structure, conventions, and requirements. This ensures my assistance is accurate, consistent, and tailored to your needs.

I will read this file as a source of truth for project-specific commands and preferences.

---

## 1. How to Customize My Behavior

To customize my behavior, simply edit the relevant sections in this document. I will refer to this guide when I need to perform actions like building, testing, or formatting code.

### File and Directory Exclusions (`.geminiignore`)

To prevent me from reading or modifying specific files and directories, you can create a `.geminiignore` file in the root of your project (the same directory as this `GEMINI.md` file).

The `.geminiignore` file uses the same syntax as a `.gitignore` file. I will automatically detect and respect its rules.

**Example `.geminiignore` file:**

```
# Ignore all compiled executables
*.exe
*.out

# Ignore specific directories
/build/
/bin/

# Ignore sensitive files
credentials.json
```

---

## 2. Project Configuration

Please fill out the sections below with the specific commands and settings for this project.

### C++ Project Settings

Since this appears to be a C++ project, please provide the following details.

*   **C++ Standard**: `(e.g., C++11, C++14, C++17, C++20)`
*   **Compiler**: `(e.g., g++, clang++, MSVC)`
*   **Compiler Flags**: `(e.g., -Wall -O2 -g)`

### Build & Execution Commands

*   **Build Command**:
    *   *Provide the exact shell command to compile your project.*
    *   **Example**: `g++ -std=c++17 -o my_program main.cpp`
    *   **Your Command**:

*   **Run Command**:
    *   *Provide the exact shell command to execute your compiled program.*
    *   **Example**: `./my_program`
    *   **Your Command**:

### Testing Commands

*   **Test Command**:
    *   *Provide the exact shell command to run your test suite.*
    *   **Example**: `make test` or `g++ -std=c++17 tests.cpp -o tests && ./tests`
    *   **Your Command**:

### Linter & Formatter Commands

I will automatically try to use common formatters if I find their configuration files. I see a `.clang-format` file in your project and will use it to format C++ code.

*   **Lint Command**:
    *   *Provide the command to check your code for style issues.*
    *   **Example**: `cpplint --filter=-build/header_guard *.cpp`
    *   **Your Command**:

*   **Format Command**:
    *   *Provide the command to automatically format your code.*
    *   **Example**: `clang-format -i **/*.cpp`
    *   **Your Command**:

---

## 3. Coding Style & Conventions

Use this section to note down any specific coding styles or conventions you want me to follow.

*   **Naming Convention**: `(e.g., snake_case for variables, PascalCase for classes)`
*   **Commenting Style**: `(e.g., Use // for single-line and /* */ for multi-line comments. Add comments to explain complex logic.)`
*   **Header Guards**: `(e.g., #pragma once)`
*   **Other Notes**: `(Add any other preferences here)`
