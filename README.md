# create

## cmore

Create more file at once

- You can also use it to create one file, but it is better
to used it, when you need to create more files.
- The directory-path must exist.
- It does not create subdirectories.

## cdf

Create the file/files and its directory, its parent-directories, if they don't already exist and the file with the help of the filepath.

## Warning 

The code was only tested in Windows and Linux. It work only Windows and Linux.

## Setup

1. Compile the tools using `gcc`:
```
gcc create_file.c createmore.c -o cmore
gcc create_file.c createdirf.c -o  cdf
```
2. Add to system-variable depending of your operating system.

## How to use it

- Open your terminal

```cmore filepath1 filepath2```

or

```cdf filepath1 filepath2```

*Èxample:* `C:\User\dummy\text.txt (for Windows)` or `\home\dummy\Documents\text.txt (for Linux)`

