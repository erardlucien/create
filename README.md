# create

This project implements two programs **cnf** and **cdf**, which allow you to create multiple files at once.

## News

**cnf** is nearly fully implemented for linux, but need to be tested, for now only *validate_dirpath()* was tested.

## Structure of the project

- `common/`: Contains code that is indepedent of the operating system.
- `linux/`:  Contaions Linux-specific code.
- `windows/`: Contains Windows-specific code.
- `tests/`: Contains the test files for the projects.
- `main.h`: Currently serve as a placeholder.

Both **cnf** and **cdf** support following options:
- `--help`
- `--df` (different filepaths)
- `--dd` (different directories): Alllows creating files in different directories.

*Example(on linux):*

`cnf --dd /home/dummy/Documents/ --files log.txt man.tex --dd ~/Downloads/private/ --files index.html main.js main.css`

This create `log.txt` and  `man.tex` in `/home/dummy/Documents`
and `index.html`, `main.js` `and main.css` in `/home/dummy/Downloads/private`.(`~` expands to the current user's home directory).

- `--sd` (same directory): Creates multiple files within the same directory.

*Example(on linux)*:

`cnf --sd ~/Documents/new-project --files Makefile main.h main.c`

This creates `Makefile`, `main.h` and `main.c`  inside `/home/dummy/Documents/new-projects`.

## cnf

Create multiple files at once

- Can de used for single file, but is optimized for multiple files simultaneously.
- **Note:** Target directories must alredy exist. This tool does not automatically create missing directories.

*Example:*

- `cnf --df /home/dummy/Documents/text.txt /home/dummy/Downloads/private/man.tex (on linux)`

## cdf

Creates files along with their parent directories automatically if they do not exist already.

## [!WARNING]

Both programs need you to have rights to create anything in the targeted path. Hidden files are not allowed to be created.
Wildcard-effects can not be stop, so the both programs work just around with them.
