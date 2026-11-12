# create

This project should implement two programs cnf and cdf, which create more files at once.

## Structure of the project

The directory *common* is for what not specific for **linux** or **windows**, while the following directories *linux*
is specific to **linux** and *windows* is specific **windows**. This project have now directory for tests, which
are called *tests*. The main.h are just there as placeholder.


## cnf

Create more file at once

- You can also use it to create a single file, but it is best suited for creating multiple files simultaneously.
- **Note:** The directory path must already exist. This tool does not automatically create missing subdirectories.

## cdf

Create the file/files and its directory, its parent-directories, if they don't already exist and the file with the help of the filepath.

## Warning 

The code was only tested in **Windows** and **Linux**. It is currently only guaranteed to work on these two operating systems.

## Installation

1. Prerequisites `gcc` and `gnu make` must be installed
2. How to install:
```make install```
3. Add the path to system-variable depending of your operating system.

## How to uninstall
1. delete manually the bin directory
2. Remove the path manually from the system-variable of your operating system.

## How to use it

- Open your terminal

```cfmore filepath1 filepath2```

or

```cdf filepath1 filepath2```

*Example:*
- `cnf C:\User\dummy\text.txt C:\User\dummy\Documents\logdir\logfile.log (for Windows)`
- `cnf \home\dummy\Documents\text.txt \home\dummy\Downloads\hobbys\music.txt (for Linux)`

- `cdf C:\User\dummy\newdir1\text.txt C:\User\dummy\Documents\newlogdir1\logfile.log (for Windows)`
- `cdf \home\dummy\Documents\User\dummy\newdir1\text\text.txt \home\dummy\Downloads\newhobbys\music.txt (for Linux)`

