# create

Two small command-line tools for creating many files at once.

| Tool  | What it does                                                        |
| ----- | ------------------------------------------------------------------- |
| `cnf` | **c**reate **n**ew **f**iles in directories that already exist      |
| `cdf` | **c**reate **d**irectories and **f**iles, including missing parents |

> **Status:** work in progress. Linux only. Both tools are almost feature-complete but not yet fully tested (see [Project status](#project-status)).

## Quick start

```sh
# Three files in one directory
cnf --sd ~/Documents/new-project --files Makefile main.h main.c

# Files in two different directories
cnf --dd ~/Documents --files log.txt man.tex --dd ~/Downloads/private --files index.html main.js main.css

# Same, but missing directories are created for you
cdf --dd ~/Documents --files log.txt --dd ~/new/nested/dir --files index.html
```

## Building

<!-- TODO: add real build/install instructions. The current Makefile only builds the tests. -->

```sh
git clone https://github.com/erardlucien/create.git
cd create
make   # currently builds the test suite only
```

## Usage

```
cnf <mode> <path(s)> --files <name(s)> [<mode> <path(s)> --files <name(s)>] ...
cdf <mode> <path(s)> --files <name(s)> [<mode> <path(s)> --files <name(s)>] ...
```

### Options (both tools)

| Option   | Meaning                                                                    |
| -------- | -------------------------------------------------------------------------- |
| `--help` | Show usage information                                                     |
| `--sd`   | **s**ame **d**irectory: create all following files in one directory        |
| `--dd`   | **d**ifferent **d**irectories: each `--dd` block has its own file list     |
| `--df`   | **d**ifferent **f**ilepaths: give each file as a full path                 |

`~` is expanded to the current user's home directory.

**Note on `--sd`:** it is meant to be used **once** per command and can take one or more directories. Anything after a second `--sd` is ignored by design:

```sh
cdf --sd ../dir00 ../dir01 --files a.txt b.md --sd ~/Docs --files index.html
# "--sd ~/Docs --files index.html" is ignored
```

If you need different file lists for different directories, use `--dd`.

### `cnf`

Target directories **must already exist**. `cnf` never creates directories.

```sh
cnf --df /home/dummy/Documents/text.txt /home/dummy/Downloads/private/man.tex
```

### `cdf`

Like `cnf`, but missing parent directories are created automatically.
In addition, `cdf` supports:

| Option   | Meaning                         |
| -------- | ------------------------------- |
| `--pd`   | parent directory                |
| `--subd` | subdirectory (relative to `--pd`) |

`--pd` and `--subd` must always be used **together**. They exist to work around
shell wildcard expansion (see [Limitations](#limitations)).

```sh
cdf --pd ~ --subd docs/lectures/ --files intro.txt chap01.txt
```

## Limitations

- You need write permission for every target path.
- Hidden files (names starting with `.`) cannot be created.
- Wildcards are expanded by your shell before the program sees them, so their effects cannot be controlled. Without shell expansion, a wildcard character is treated as an ordinary (and invalid) character.

## Project layout

```
common/   OS-independent code
linux/    Linux-specific code
tests/    Tests
Makefile  Test build (for now)
```

## Project status

- `cnf` and `cdf` are almost fully implemented for Linux.
- Tested so far: `validate_dirpath()`, `validate_filename()`.
- Next: `extract_directory_path()`, then the functions in `files.h` and `create.h`.
- Some requirements are not yet met, so treat the tools as experimental.

## License

<!-- TODO: add a license (e.g. MIT) and a LICENSE file. -->
