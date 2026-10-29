# create

## createmore

Create more file at once

- You can also use it to create one file, but it is better
to used it, when you need to create more files.
- The directory-path must exist.
- It does not create subdirectories.

## createdirf

Create directories and his subdirectories, if they don't already exist and the file with the help of the filepath.

## Warning 

The code was only tested in Windows and Linux. It is only Windows and Linux.

## Setup

- gcc create_file.c createmore.c -o cmore
- gcc create_file.c createdirf.c -o  cdf
- Add to system-variable depending of your operating system.

## How to use it

- Open your terminal
> cmore filepath1 filepath2
or
> cdf filepath1 filepath2
** filepath1 could be C:\User\dummy\text.txt (for Windows) or \home\dummy\Documents\text.txt (for Linux) **

