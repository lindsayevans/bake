# Bake

Yet another build automation tool - partly inspired by Make and NPM

## Features:

- JSON configuration
- Variables
- Build dependencies - only triggers on changed source/transitive dependencies

## TODO:

- Split variable interpolation out into a second pass
- Variables: get from env
- Glob file expansion
- JSONC support (Glaze) - probably need to switch to g++ for C++23 lang support
- Build schema using Glaze from config struct
- Install project dependencies
- Watch mode
- Parallel command execution
- "Dashboard" mode to show command execution progress

## Usage

```sh
bake [options] [...targets]
```

### Options

```
--quiet, -q             Quiet mode - just output the result
--help, -h              Print out usage information
--version, -v           Print version information
--config-file, -c       Path to donfig file (default: bake.json)
--print-config          Print configuration
--print-tree            Print the deoendency tree
--dry-run, -d           Don't execute commands, just print
-D                      Define a variable (e.g. -DFOO=bar -DA=a,B=b)
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
