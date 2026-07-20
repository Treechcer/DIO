# Programming language

This is test of making a programming language in C with the least amount of help from google as possible. This will be small interpreted language.

>NOTE: this is my first C code. I only used C++ (as the most SIMILAR language to C).

## Documentation

Dio is procedural, statically typed that's weakly typed programming language.

### Types

(implemented)

- [x] int - basic number type, it's internally used as float
- [x] float - basic number type, can be used with +, -, *, /, <, >, <=, >=, ==
- [ ] string
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

> NOTE: true and false are in lexer made into 1 and 0, so `true * 10` is valid code and is 10.

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
int a = 0;
while(a < 10)
    a = a + 1
    out(a)
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

- Make variable types work somehow differently
