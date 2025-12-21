# Blog

## Initial thought

We have, only 7 days to create a programming language and a game with
the language we created. We think than the most reasonable thing to do
is to create a language with a limited number of features. So, if we
want to finish this jam with a language and a game, we'll need a
maximum of 3.5 days for the language and 3.5 days for the game,
in order to make a game that's somewhat good.

Personally (@ArthurPV), I really want to create a language with as
little dependency as possible, because otherwise it's no fun. It
would be too easy to create a language with LLVM, plus I've
already created several with it.

So, to learn new things and make it fun, we decided to create an
interpreted language that could call C functions by loading dynamic
libraries with
[dl](https://man7.org/linux/man-pages/man3/dlopen.3.html).

So here's the plan:

- Lexer
- Parser
- Interpreter

That's basically all we need. Also the language will be
[GC](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science))
less, **really really** unsafe (probably more than C) and cursed.

In addition, there is a small additional difficulty:
we are both university students and we have our final exams
during the jam, so we won't really have a whole week. But who cares?
The goal is just to have fun and stay up all night as much as possible.

## 14th December night and 15th December early in the morning

We started the jam around 8 p.m, and we basically created the lexer in
about four hours of coding and lively discussion. Honestly, there's
not much to say. We created a fairly standard lexer that any basic
language could have. By the way, for those who don't know what a lexer
is, it's basically a program that takes a stream of characters (e.g.,
the contents of a file) and converts them into tokens.

While @Sam5429 takes care of the lexer, I'll take care of the AST to
save some time, so that when he's done, we can start the parser right
away.

It took me about 3 hours, and all I can say is that doing an AST for
that long was horrible, boring, and repetitive.

We basically tested/debug our lexer for a maximum of 1 hour, and then
we moved on to the parser directly.

We started working on the parser structure, but we decided to stop
[here](https://github.com/Sam5429/coulang/commit/65d42344fac7ff479ee33347be473b3b145fb1fd)
because it was almost 7 a.m.

## 15th December afternoon and night 

During the afternoon, I pretty much finished the entire parser. Again,
there's not much to say about it; it's a fairly standard parser, with
no tricks that are too complicated to understand (except that I may
have messed up the operation precedence, but we'll try to fix that
later).

Then I started implementing things like `scope` so that I could
finish implementing the interpreter the next day. We've finished
the day [here](https://github.com/Sam5429/coulang/commit/5bb656131a8ea1fb964ea8828248518654afe161).

## 16th December

On this day, @Sam5429 focused mainly on improving the error system, so
that it would be reasonably acceptable and understandable (location and
all that stuff).

For my part, I worked quite a bit on the interpreter part. I had pretty
much finished it, all that was left was to manage the symbol call part,
loaded from `dl`. And that's when things started to get a little more
complicated. The problem I encountered is that in C, when you load a
symbol with `dl`, you do the following:

```c
void *handle = dlopen("yourlib.so");

if (!handle) {
    exit(1);
}

void (*foo)(size_t) = dlsym(handle, "foo");

if (!foo) {
    exit(1);
}

void *p = foo(1)
```

The problem lies in the penultimate line. To call the `foo` function,
we do a cast. And obviously, in C, at least as far as I know, it is
impossible to do the following if `foo` is a `void*`: `foo(...)`.

So, if it's impossible to do in C, it's possible to do it in Assembly
language. Yeah I know... but that will be for another day. We've
finished the day [here](https://github.com/Sam5429/coulang/commit/8d002204a72b2f425a29a689dc01fac5f480c4f3).

## 17th December

As explained above, we must write assembly code to be able to call the
symbols that we load into the interpreter. So, basically, we must move
the values that we pass to the functions to the right places while
respecting the
[C (x86_64) calling convention](https://en.wikipedia.org/wiki/X86_calling_conventions).

After writing that, I managed to get the interpreter to work, in order
to open a window via [raylib](https://github.com/raysan5/raylib).

We've finished the day
[here](https://github.com/Sam5429/coulang/commit/a6873e4e9deabf3e167b7644a2364f182289b58b).

## 18th December night and 19th December

First, on that day, I fixed a bug in the assembly code, which was that
I was pushing the parameters onto the stack in the wrong order.

We started discussing what kind of game we wanted to make, and we ended
up expecting a game like
[Space Invaders](https://en.wikipedia.org/wiki/Space_Invaders) (will
be probably less good).

@Sam5429, took care of adding the game's first assets and coding its initial features,
while I fixed the bugs he found and added new features.

For instance, a funny bug that @Sam5429 found was that we were computing expressions passed
to a function call on the scope of the function being called, which caused an incorrect
error message about a variable not being found.

Here is an example of code that could trigger this bug, for the context:

```
load "libc.so" printf int

fn add(x int) int {
    printf("%d", x)
}

fn main() int {
        if 1 {
            val x int = 0
            add(x)
        }
}
```

And the [fix](https://github.com/Sam5429/coulang/commit/82fb51f73cba27ec821b607dcf598c90e3952bcc)
of this bug.

In short, to make the game, we chose to use raylib as specified, as it is a fairly easy library to
learn. It was the first time we had used this library, so we took some time to read the
documentation and code examples to familiarize ourselves with it.

Since our language doesn't really have an option for creating structures, we had to create
glue in C in order to adapt some of the raylib functions and perform the necessary operations
on structures in C. After that, all we had to do was load them into
the interpreter to be able to use them.

At the end of the day @Sam5429 finished creating a collision system and basic ship movements,
as well as asteroid generation.

We're done [here](https://github.com/Sam5429/coulang/commit/bafcf2ff41ec86c17b760f32b9a914fbe5929b5e).

## 20th December

On this day, the main goal was to finish the game, on time of course.
@Sam5429, took care of creating new assets and creating the game's own sound effects.
Then implement them in the game. It has an end-of-game screen and is responsible for
randomly displaying asteroids and creating a new item to regain a life.

For my part, I took care of adding a scoring system and adding them with the assets
that @Sam5429 had made, as well as adding the number of lives remaining.

Thanks to the creator of this jam, we had a great time.
