# create

This project should implement two programs cnf and cdf, which create more files at once.

## Structure of the project

The directory *common* is for what not specific for **linux** or **windows**, while the following directories *linux*
is specific to **linux** and *windows* is specific **windows**. This project have now directory for tests, which
are called *tests*. The main.h are just there as placeholder. **cnf** and **cdf** will have 4 options:
- --help
- --df (different filepath)
- --dd (different directories) Here different files will be created in different directories

*Example(on linux):*

`cnf --dd /home/dummy/Documents/:log.txt:man.tex ~/Downloads/private/:index.html:main.js:main.css`

will create in the directory /home/dummy/Documents log.txt, man.tex and then in the directory
/home/dummy/Downloads/private index.html, main.js and main.css.(~ stand for the actual home-directory for the actual user in our example is /home/dummy here)
- --sd(same directory)

*Example(on linux)*:

`cnf --sd ~/Documents/new-project Makefile main.h main.c`

In the directory /home/dummy/Documents/new-project will be created Makefile, main.h and main.c.

## cnf

Create more file at once

- You can also use it to create a single file, but it is best suited for creating multiple files simultaneously.
- **Note:** The directory path must already exist. This tool does not automatically create missing subdirectories.

*Example:*

- `cnf --df /home/dummy/Documents/text.txt /home/dummy/Downloads/private/man.tex (on linux)`

## cdf

Create the file/files and its directory, its parent-directories, if they don't already exist and the file with the help of the filepath.

