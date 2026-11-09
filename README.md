# create
  Generate two programs, use as command-line-interface.

## cfmore

Create more file at once

- You can also use it to create a single file, but it is best suited for creating multiple files simultaneously.
- **Note:** The directory path must already exist. This tool does not automatically create missing subdirectories.

## cdf

Create the file/files and its directory, its parent-directories, if they don't already exist and the file with the help of the filepath.

## Warning 

The code was only tested in **Windows** and **Linux**. It is currently only guaranteed to work on these two operating systems.

## Setup

1. Prerequisites `gcc` and `gnu make` must be installed
2. How to install:
```make install```
3. Add the path to system-variable depending of your operating system.

## How to uninstall
1. How to uninstall:
```make uninstall```
2. Remove the path manually from the system-variable of your operating system.

## How to use it

- Open your terminal

```cfmore filepath1 filepath2```

or

```cdf filepath1 filepath2```

*Example of filepath:* `C:\User\dummy\text.txt (for Windows)` or `\home\dummy\Documents\text.txt (for Linux)`


