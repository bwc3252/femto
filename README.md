```
  __               _
 / _|             | |       
| |_ ___ _ __ ___ | |_ ___  
|  _/ _ \ '_ ` _ \| __/ _ \
| ||  __/ | | | | | || (_) |
|_| \___|_| |_| |_|\__\___/
```

# femto

A simple text editor written in C.

This project is purely for fun and is not intended to be used by anyone for anything.

## Installation

To compile femto you will need git, GNU make, and gcc. First, clone the repository:

```
$ git clone https://github.com/bwc3252/femto.git
$ cd femto
```

Then compile:

```
$ make
```

If you want to run the editor from anywhere on your system, you should add the binary to your `PATH`.
For example, if you cloned the repository into your home directory, add the following to your `.bashrc`:

```
export PATH=$PATH:~/femto/build/
```

## Usage

Launch the editor with the name of a file to edit:

```
$ femto file.txt
```

You can begin typing right away. To save, enter `CTRL-s`, and to quit enter `CTRL-x`.
