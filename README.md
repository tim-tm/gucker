# gucker

Gucker is a simple tool for viewing images built with raylib.

## Usage

You need to have a version of raylib installed on your system.

Cloning the repo
```sh
git clone https://github.com/tim-tm/gucker.git
```

Building gucker
```sh
chmod +x build.sh && ./build.sh
```

The gucker executable will be available at build/gucker.
Gucker expects at least one image to be specified via. the command line.

Example:
```sh
./gucker test.png abc.png 2.png
```
