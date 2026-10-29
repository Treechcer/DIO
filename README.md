<!-- rules: >
<!-- markdownlint-disable MD037-->
<!-- markdownlint-disable MD033 -->
# Dio (Programming language)

![Version](https://img.shields.io/badge/version-0.1.0-blue.svg?style=flat-square)
![License](https://img.shields.io/badge/license-BSD_3-green?logo=open-source-initiative)
<img src="./scripts/data/logo.png" width=48px style="inline-block" alt="Logo of Dio">

<!-- markdownlint-enable MD033 -->

<!--

Non-HTML images

![Version](https://img.shields.io/badge/version-0.1.0-blue.svg?style=flat-square)
![License](https://img.shields.io/badge/license-BSD_3-green?logo=open-source-initiative)
![Dio](./scripts/data/logo.png)
-->

Dio is small open-source scripting programming language made in C that's supposed to run on most modern computer systems.

## Current OS compatibility

|OS|Status (tested)|Runs as should|notes|
|-|-|-|-|
|Windows|✅|Yes||
|Linux|✅|Yes||
|Android|✅|Yes|Termux needs to be used|
|FreeBSD|✅|Compiled, more testing needed||
|iOS|🟨|Compiled, more testing needed|a-shell needs to be used (with clang), seems to have some issue with python test script but seems that it works|
|Mac (Intel)|🟨|Yes||
|Mac (Apple silicone)|🟨|Compiled, more testing needed||
|WebApp|🟨|Untested|can technically run through WASM (more novelty than actual release) - full support not promised|
|OpenBSD|🟨|Was compiled but test haven't ran correctly (13 passed, 16 failed)||
|NetBSD|❌|Was not tested in any way but is planned to be supported in either releases or as working on that platform if compiled||
|HaikuOS|❌|Was not tested in any way but is planned to be supported in either releases or as working on that platform if compiled||
|ReactOS|❌|Was not tested in any way but is planned to be supported in either releases or as working on that platform if compiled||

## Release Cycle

Release cycle ins't set in any way. Time between any versions can be as long (or as short) as needed for that version. Any code in main branch ('Development' branch) is not meant as stable code and can be unfinished and may not run correctly, **only stable code is in the release**. Version are if format: **Major.Minor.Patch**.

Current platforms in releases (current and future):

- Windows
- Linux
- MacOS (Intel CPU)
- Android
- iOS (Dio version 0.1.1+)
- FreeBSD
- WebApp (Dio version 0.1.1+)

> NOTE: although Apple Silicone **should** work, I can't reliably compile for it, I don't own any Apple silicone machine.

## Compiling and Running

### Stable version

Stable is only version in releases. Currently there is only one release (0.1.0).

### Compiling

To compile it run in the root of this repository command:

```sh
python3 ./scripts/compile.py
```

This automatically runs the language with params from ./scripts/CompileRunParams.txt (which is usually examples/ex[num].dio).

> Note: more info in ![Readme.md](./scripts/readme.md)

## Documentation

Dio is procedural, statically typed programming language. Working directory is generated through "-file" input, if you input "-file example/ex2.dio", DIO uses ./examples/ as it's working directory.

### Origins

The name `Dio` comes from ancient Greek philosopher [Diogenes of Sinope](https://en.wikipedia.org/wiki/Diogenes) and his [bowl story](https://medium.com/@ricosutioso/diogenes-the-beggar-philosopher-fdd71946f641). Essentially, Diogenes saw kid drinking water from river with his hands, so he threw away his bowl (one of two things he owned). It's called `Dio` because it's trying to be minimalistic language.

### File extensions

DIO supports three file extensions:

- .dio (original)
- .diogenes
- .diogenesdesinope

Everything else should raise error.

### launch Params

#### -f / --file

This parametre tells dio which file to use.

> If this parametre isn't technically needed, it'll open "REPL" mode, which isn't finished, so it doesn't fully works, please don't use it, it's not finished.

#### -d / --debug

This is now only used for writing out some values for this software (like version etc.).

#### -e / --execute

This executes whatever you type as next param. This was added for WebApp support.

#### -v / --version

This outputs current platform and version, immediately stops execution with exit code 0.

#### -a / --appInfo

This is used to print out just info about the app (platform, version, source).

### Variable types

- [x] int - basic number type, it's internally used as float.
- [x] float - basic number type, can be used with +, -, *, /, <, >, <=, >=, ==.
- [x] string - basic text (can only be used as output and be concatenated with "+").
- [x] bool - integer that's ONLY checked while creating if has bool values, after initialization it can have ANY int/float value.
- [x] number arrays - part of int.

### Comments

#### Single-line comments

Dio uses `>>` as the symbols for single-line comments. Comments end when lexer either find end of the line or the file ends.

#### Multi-line comments

Dio uses construction of `>*` (start) and `*<` (end) as multi-line comment, or the so called 'cat symbol' (**>* *<**). These comments end either when the file ends or when it finds the end symbol.

> Note: if you only ues `>*` everything afterwards will be taken af part of the comment.

### Variables

```py
>>Initialize 
int a = 10;

>>Using var
int b = a + 1;

>>Creating array
int arr = {1,2,3.5,7}
```

#### Booleans

All normal booleans (true, false) are here, represented by either the word "**true**" or "**false**" (this is changed in lexer to 1 and 0 respectively). Dio also has special boolean, maybe, which is either 1 or 0 (determined in the parser - same "line" different result).

### Functions

```py
>>Declare
def name()::void
    >>code
end

def withParams(type : name, type2 : name2 ...)::void
    >>they can have any amount of inputs
end

>>call
name()
withParams(a, 10) >> example, use variable and 10
```

#### Function Return Types

Functions can return values, they must return a value of the type specified. If no type is specified `::void` is being used as default value.

```py

def a()::int
    return 7;
end

def b()::string
    return "something"
end

def c()
    >>return can't be used!
end
```

#### Function chain functions

Traditionally in programming, you can used functions as inputs and they get interpreted as a value. This language does not support that (at least yet*), instead there is chaining which always places result as first input of the next function and moves all inputs that are already in the function. This is used mostly to assign values to variables that can then be used later.

```py
def a()::int
    return 7
end

def b(int : a)::void
    out(a)
end

def c(int : a)::int
    return a + 1
end

int retVal = a():c()
b(retVal)

>>Writes out 7, because chain functions pair together so "a" returns 7 which then "c" adds 1 making it 8
```

### Control Flow

#### WHILE Loops

```py
int i = 0;
while(i < 10)
    i = i + 1
    out(i)
end

>>outputs 1 -> 10
```

#### FOR Loops

```py
for (int i = 0; i < 10; i = i + 1)
    out(i)
end

>>outputs 0 -> 9
```

##### Loops with walrus operators, increments and loopers

> Note: All these can be used elsewhere but I believe this is where they shine the most.

```py
>>Walrus operators

for (int i = 0; i < 10; i += 1) >> will become i = i + 1
    out(i)
end

for (int i = 10; i > 0; i -= 1) >> will become i = i - 1
    out(i)
end

>*

All walrus operators:

+=
-=
*=
/=

*<

>>Increment / decrement

for (int i = 0; i < 10; i = i++) >> will become i = i + 1
    out(i)
end

for (int i = 10; i > 0; i = i--) >> will become i = i - 1
    out(i)
end

>>Loopers

for (int i = 0; i < 10; i+++) >> will become i = i + 1
    out(i)
end

for (int i = 10; i > 0; i+++) >> will become i = i - 1
    out(i)
end

```

#### Branching

As in standart languages:
If -> elseif -> else

```py

if (condition)
>>code
elseif (condition)
>>code
else
>>code
end
```

#### Goto

Goto-s are not not recommend to be used, they might have some specific use cases, they were added as 'first feature'.

```py

>>this is without any condition

::gotoName::

>>code

goto ::gotoName::


>> goto with condition

::gotoName::

>>code

goto (condition) ::gotoName::

```

> NOTE: if you don't use condition, it will run always

### Standard Library

#### out(something : anyType)::void

```py
int a = 5
out(a)
```

Out is print function that prints out the variables or what you put into it.

#### length(something : arrayType)::int

```py
int a = {1,2,3}

int len = length(a)

out(len)
```

Length returns the integer length of array type variables (string or number array).

#### error(text : string)::void

Raises error, similarly as the language does errors. This makes the program instantly exit with some error code.

#### warning(text : string)::void

Raises warning, this only writes out text but does not stop executing other code.

#### exec(text : string)::void

Executes shell commands.

#### input(text : string)::string

Writes out the text input and takes input, which is then returned as what the user writes.

### middle-processor

Middle processor is kind of middle ware used to add code or change how lexing works. This is imitating (in a way\*) pre-processor from C (or that's the idea behind itq*). All mid processor calls are prefixed with '#'.

#### get

- Takes argument of file.
- Adds the file into the program you're making, this is used to import to other file for multi-file support.

#### def and use

- `#def *name*: *valid dio code*` this makes macro which can be used with command `#use *name*`

#### specific OS code

- Using `#WIN *valid dio code*` will make this piece of code run only and only on windows
- Using `#LINUX *valid dio code*` will make this piece of code run only and only on linux
- Using `#APPLE *valid dio code*` will make this piece of code run only on apple devices (Mac, iOS, possibly that watch thingy if terminal emulator exists..)
- Using `#FREEBSD *valid dio code*` will make this piece of code run only and only on freeBSD
- Using `#ANDROID *valid dio code*` will make this piece of code run only and only on Android
- Using `#WEB *valid dio code*` will make this piece of code run only and only in the WebApp version

#### Pragma

##### Once

- declaring "#pragma once" at the start of the programme it'll make it so it can be loaded only once

### Special Variables

These work in similar way as macros, except they are used with prefix "$". They are SIMILAR as if you used some macro except they don't need anything else to be invoked than **$** + **name**.

#### $PLATFORM (string)

This will add tokens of values: ", PLATFORM-NAME (APPLE, ANDROID, LINUX...), "

```py
string platform = $PLATFORM

>>This can be also used in ifs etc.
```

#### $VERSION (string)

This will add tokens of values? ", VERSION-NUMBERS (0.1.0, 1.0.0, etc.), "

```py
string version = $VERSION
```
