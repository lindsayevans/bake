# bake

yet another build automation tool

- JSON configuration

## Usage

```sh
bake [options] [...targets]
```

```sh
bake clean default install
bake --dry-run debug default
```

### Options

```
--config-file   -c      Default: bake.json
--dry-run       -d
--tree          -t
```

`bake -d -t -c ./examples/basic.json`:

```
===========================

Target dependency tree:

+ clean
+ install
+ ./out/awesometool     [S]
  + ./out/main.o        [U]
    + ./src/main.c      [U]
  + ./out/foo.o         [S]
    + ./src/foo.c       [S]

===========================

Dry run:

clang -c -Wall -o ./out/foo.o ./out/foo.c
clang -llibcurl -o ./out/awesometool.exe ./out/main.o ./out/foo.o

```

## Build

### Dependencies

[Jansson](https://jansson.readthedocs.io/en/latest/gettingstarted.html#compiling-and-installing-jansson)

### Build & install

```sh
make && make install
```

### Build schema

```sh
npx ts-json-schema-generator --path 'Config.ts' --type 'Config' > schema.json
```

## TODO:

- Build dependency tree
- Output dependency tree
- Check dependency modification (time? hash? configurable?)
  - Hash: store hashes in .bake/hashes.csv:
    file,hash
    src/foo.c,xxx
    out/foo.o,yyy
- Run commands
- Split variable interpolation out into a second pass, add --print-config-original
- Variables: get from env -> config -> params
- JSONC support (Glaze) - probably need to switch to g++ for C++23 lang support
- Build schema using Glaze from config struct
- Install project dependencies
- Watcher
