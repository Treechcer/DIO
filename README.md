# Programming language

This is test of making a programming language in C with the least amount of help from google as possible. This will be small interpreted language.

>NOTE: this is my first C code. I only used C++ (as the most SIMILAR language to C).

## Documentation

Dio is procedural, statically typed that's weakly typed programming language.

### Origins

The name `Dio` comes from ancient Greek philosopher [Diogenes of Sinope](https://en.wikipedia.org/wiki/Diogenes) and his [bowl story](https://medium.com/@ricosutioso/diogenes-the-beggar-philosopher-fdd71946f641). Essentially, Diogenes saw kid drinking water from river with his hands, so he threw away his bowl (one of two things he owned). It's called `Dio` because it's trying to be minimalistic language (not in line count, but with how much it can do).

### Types

(implemented)

- [x] int - basic number type, it's internally used as float
- [x] float - basic number type, can be used with +, -, *, /, <, >, <=, >=, ==
- [x] string - basic text (can only (for now) be used as output)
- [ ] bool

### Comments

Dio only uses single line comments, `>>`, these can be used anywhere in the line, however everything after it will not be evaluated in any way.

### Variables

```py
>>Declaration
int a = 10;

>>use
int b = a + 1;
```

#### Booleans

All normal booleans (true, false) are here, represented by either the word "true" or "false" (this is changed in lexer to 1 and 0 respectively). Dio also has special boolean, maybe, which is either 1 or 0.

> NOTE: in loops for some reason it never generates a new number making the loop behave same???

### Functions

```py
def name()
    >>code
end

def withParams(type : name, type2 : name2 ...)
    >>they can have any amount of inputs
end

>>call
name()
withParams(a, 10) >> example, use variable and 10

>>NOTE: var types are now not fully used, so function can be parsed any argument 
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

## ISSUES (right now)

- TODO: TEST so I can update this

## NEXT

- loading files (essentially same as I use STD by dynamically in lexer, add like file identifiers, something like "#FILE:NAME" to correctly add file in or start a new lex and add everything to it, whatever is easier)
- Make variable types work differently and check them + make like "any" as parameter for functions or add overloading (either wold work)
