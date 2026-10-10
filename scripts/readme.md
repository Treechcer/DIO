# Scripts

Here are small scripts meant for compilation and other stuff

## Documentation

### compile.py

Small python script to compile whole project and then runs it.
Has small file "CompileRunParams.txt" which is read when it opens the file so you can have some input in it (for example which file - when that will be supported). Generates STD for DIO from "data/std.dio" file (it's "baked" into the binary).

#### Params

##### --debug

This after compilation runs it in gdb for debug.

##### --release

This builds the release binary for current platform. Uses data from data/release.json.

#### --version

Overrides version data from data/release.json

#### --tests

This disables the running of tests

#### --timeTest

This disables the running of timeTest.
