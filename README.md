# jache

A JSON based build tool, because make is a pain

> Pronounced like the name "Jake"

## Usage

```sh
jache [options] [...targets]
```

```sh
jache clean default install
jache --dry-run debug default
```

### Options

```
--config-file   -c      Default: jache.jsonc
--dry-run       -d
--tree          -t
```

`jache -d -t -c ./examples/basic.jache.jsonc`:

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

- JSONC support (Glaze)
- Build schema using Glaze from config struct
- Build dependency tree
- Output dependency tree
- Check dependency modification (time? hash?)
- Run commands
- Install project dependencies
