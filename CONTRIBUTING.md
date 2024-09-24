# Contributing to the C Hacktoberfest Challenge

Thanks for participating in this project! Please take a moment to read the guidelines below before making your contribution.

## Code Style

Please follow these coding conventions to keep the codebase consistent:

### 1. Braces Placement

Opening braces `{` should be on the same line as the function or statement header, not on a new line.

**Correct**

```c
int main() {
    // Code here
}
```

**Incorrect**

```c
int main()
{
    // Code here
}
```

### 2. Indentation

- Use 4 spaces for indentation, not tabs.
- This project already includes a `settings.json` file inside the `.vscode` folder. So you shouldn't have to worry about this if you're using VSCode.
  - If you're using another editor, you can copy the contents of the `settings.json` file and paste it into your editor's settings.

### 3. Naming Conventions

Use the following naming conventions across the codebase:

- Variables: Use `snake_case` for variable names.
  - Example: `int total_sum = 0`;
- Constants: Use `ALL_CAPS` with underscores for constants.
  - Example: `#define MAX_BUFFER_SIZE 1024`
- Functions: Use `snake_case` for function names.
  - Example: `void calculate_sum(int num1, int num2);`
- File Names: Use `snake_case` for file names, and keep file extensions in lowercase.
  - Example: `audio_capture.c`
- Struct Names: Use `snake_case` for struct names and `ALL_CAPS` for typedefs.
  - Example:

```c
typedef struct audio_buffer {
    char name[50];
    int score;
} BUFFER;
```

### 4. Comments

- Technical comments about code functionality are allowed (e.g., `// This function calculates X`).
- Avoid comments that give instructions to the next developer (e.g., `// You should add Y`).
- Use `/* Block comments */` for multiline comments and `//` for single-line comments.

### 5. Variable Naming

- Use meaningful variable names (e.g., `total_sum` instead of `ts`).
- In case a function or variable name is too long, an abreviation can be used as long as a proper comment is added to explain it.

## Workflow for Contributions

### 1. Fork the Repository

- Begin by [forking the repository](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/fork-a-repo) to your GitHub account.

### 2. Clone your Fork

```bash
git clone https://github.com/your-username/c-challenge.git # Clone your fork
cd c-challenge # Change directory to the project folder
```

### 3. Create a Branch

- Since this challenge is about creating a program without communication, you should name your new branch after your name or username. This is a bad practice in a real-world scenario, but it's fine for this challenge.

```bash
git checkout -b my-username # Create a new branch
```

### 4. Make Changes

- Follow the rules in the [README.md](./README.md) and in the [CONTRIBUTING.md](./CONTRIBUTING.md) to create your program.

### 5. Commit Changes

After making some changes, commit them to your branch. You can make as many commits as you want.

```bash
git commit -m "Add something"
```

### 6. Add username to the README

- Don't forget to add your username to the [Contributors](./README.md#contributors) section in the README.
- After that commit the changes:

```bash
git commit -m "Update README.md" - m "Add my-username to the contributors list"
```

### 7. Push Changes

- Once you're done making changes, push them to your fork.

```bash
git push origin my-username # Push changes to your fork
```

### 8. Create a Pull Request

- Go to your fork on GitHub and create a pull request to the `main` branch of the original repository. Add a title and a description to your pull request, and submit it.
- Wait for maintainers to review your code and merge it.

### 9. AFter PR is Merged

- you will be credited as a contributor on the project!

## Overview of this document

This guide includes:

- **Braces placement**
- **Indentation**
- **Naming conventions** for variables, constants, functions, file names, and structs
- **Comment guidelines**

It also covers the complete workflow for forking, creating a branch, and submitting a pull request. Let me know if you'd like any more modifications!
