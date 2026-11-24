# create

This project implements two programs **cnf** and **cdf**, which allow you to create multiple files at once.

## News

**cnf** still nearly fully implemented for *linux*, but need to be tested, for now only *validate_dirpath()* was tested.
**cdf** is nearly fully implemented for *linux* but need to be tested.

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
In option `--dd` you have parameter now `-p` for parent-directory and `-c` for child-directory. But you still can
use it like this `cdf --dd /home/dummy/new-project --files index.html main.js main.css`. Both `-p` and `-c`
must be use together. This two was implemented to work around Wildcard-effects like in the following
example. There still oder possibility avaible, which need to be tested.

*Example:*

`cdf --dd -p ~ -c docs/lectures/ --files intro.txt chap01.txt --dd -p ../* -c project00 project01 project02 --files Mekefile placeholder.txt`

## [!WARNING]

- Both programs need you to have rights to create anything in the targeted path. Hidden files are not allowed to be created.
- Wildcard-effects can not be stop, so the both programs work just around with them.
