# Programming language

This is test of making a programming language in C with the least amount of help from google as possible. This will be small interpreted language.

>NOTE: this is my first C code. I only used C++ (as the most SIMILAR language to C).

## OS compatibility

|OS|Status (tested)|Runs as should|notes|
|-|-|-|-|
|Windows|✅|Yes||
|Linux|✅|Yes||
|Android|✅|Yes|Termux needs to be used|
|FreeBSD|✅|Compiled, more testing needed||
|iOS|✅|Compiled, more testing needed|a-shell needs to be used (with clang), seems to have some issue with python test script but seems that it works|
|Mac (Intel)|✅|Yes||
|Mac (Apple silicone)|✅|Compiled, more testing needed||

> for version 1.0.0 I want to test as many OS and hardware as possible (eg. OpenBSD, NetBSD, Haiku OS, ReactOS, maybe some others? These are those that I aim for that aren't in the table above).

## Compiling and Running

### Stable version

Any code that's not in release is not meant as stable version, the main branch is more so development. Please for any project use any version from release.

> Note: currently there are no releases

### Compiling

To compile it run in the root of this repository command:

```sh
python3 ./scripts/compile.py
```

This automatically runs the language with params from ./scripts/CompileRunParams.txt (which is usually examples/ex[num].dio).

## Documentation

Dio is procedural, statically typed that's weakly typed programming language. Working directory is generated through "-file" input, if you input "-file example/ex2.dio", DIO uses ./examples/ as it's working directory.

### Origins

The name `Dio` comes from ancient Greek philosopher [Diogenes of Sinope](https://en.wikipedia.org/wiki/Diogenes) and his [bowl story](https://medium.com/@ricosutioso/diogenes-the-beggar-philosopher-fdd71946f641). Essentially, Diogenes saw kid drinking water from river with his hands, so he threw away his bowl (one of two things he owned). It's called `Dio` because it's trying to be minimalistic language (not in line count, but with how much it can do).

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

### Variable types

- [x] int - basic number type, it's internally used as float
- [x] float - basic number type, can be used with +, -, *, /, <, >, <=, >=, ==
- [x] string - basic text (can only be used as output and be concatenated with "+")
- [x] bool - basic booleans
- [x] number arrays - part of int

> NOTE: booleans internally are used as integers/floats, meaning the constrains for valid bools are checked while creating the variable not while using it. You can make boolean equal non boolean values AFTER you initialize it.

### Comments

#### Single-line comments

Dio uses `>>` as the symbol for single-line comments. Comments end when lexer either find ending of the line or the file ends.

#### Multi-line comments

Dio uses construction of `>*` (start) and `*<` (end) as multi-line comment. These comments end either when the file ends or when it finds the end symbol.

> Note: if you only ues `>*` everything afterwards will be taken af part of the comment.

### Variables

```py
>>Declaration
int a = 10;

>>use
int b = a + 1;

>>arrays
int arr = {1,2,3.5,7}
```

#### Booleans

All normal booleans (true, false) are here, represented by either the word "true" or "false" (this is changed in lexer to 1 and 0 respectively). Dio also has special boolean, maybe, which is either 1 or 0 (determined in the parser).

### Functions

```py
def name()::void
    >>code
end

def withParams(type : name, type2 : name2 ...)::void
    >>they can have any amount of inputs
end

>>call
name()
withParams(a, 10) >> example, use variable and 10

>>NOTE: var types are now not fully used, so function can be parsed any argument 
```

> Functions can be declared without the "::void" but they can't return anything

```py
>>Showing params and how chain functions

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

>Note: functions used in chain can't have more than one input for now, this will feature will be added

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

> NOTE: walrus operators are not implemented, you have to use "n = n ..."

#### Branching

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

#### out

```py
int a = 5
out(a)
```

Out is print function that prints out the variables or what you put into it.

### middle-processor

Middle processor is kind of middle ware used to add code or change how lexing works. This is imitating (in a way\*) pre-processor from C (or that's the idea behind itq*). All mid processor calls are prefixed with '#'.

#### get

- Takes argument of file.
- Adds the file into the program you're making, this is used to import to other file for multi-file support.

#### def and use

- `#def *name*: *valid dio code*` this makes macro which can be used with command `#use *name`

#### specific OS code

- Using `#WIN *valid dio code*` will make this piece of code run only and only on windows
- Using `#LINUX *valid dio code*` will make this piece of code run only and only on linux
- Using `#MAC *valid dio code*` will make this piece of code run only and only on mac OS
- Using `#FREEBSD *valid dio code*` will make this piece of code run only and only on freeBSD
- Using `#ANDROID *valid dio code*` will make this piece of code run only and only on Android

> Note: although OS like Android are supported, it's not yet possible to make code specific for android

#### Pragma

##### Once

- declaring "#pragma once" at the start of the programme it'll make it so it can be loaded only once
