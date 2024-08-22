# Multi Threaded Directory Search in C

Using Multi-threading and directory traversing in a cross platform way, using third party libraries, yes or no.

## Introduction

### Multi Threading on Windows and `POSIX` systems

Creating multi threaded programs are challenging by themselves, making them work cross platform is even more.

Generally on Windows we're supposed to use `windows.h` and `CreateThread` and other fellas to manage threads while on `POSIX` operating systems we're using `pthread.h` to manage them and then linking the executable with `pthread` library using `-lpthread` flag.

Well, `gcc` on windows "supports" using `pthread` but you need to use `gcc` compiler which is totally fine and also worth mentioning here that we still need to use `-lpthread` flag while compiling our program.

To achieve that I'm going to use my own multi threading library known as [DThreads](https://github.com/dezashibi-c/dthreads).

**Though I could split files into two "_windows.c" and "_posix.c" files to create a more readable implementation and maintainable but honestly it's still readable.**

### Accessing Directory in Windows and `POSIX` systems

Again same as multi threading we've got `dirent.h` on `posix` systems and `windows.h` for windows api on windows.

I knew that there is a remake of `dirent.h` that supposed to work on windows, I'm not talking about those shipped with `MingW` or `Cygwin`, I would like to test various compilers, recently for a project I'm very interested to work more with `zig cc` as it can be a drop-in `C` compiler.

The remake is in fact the [dirent.h](https://github.com/tronkko/dirent) by [Toni Rönkkö](https://github.com/tronkko), I've never had a chance to use this library, it's a one header file and well what's better than that.

So the plan is to write a cross platform program and with more biased to the code style on Linux than windows (Sorry Microsoft 😅).

## What the project is?

I wanted to have something in my portfolio to show that I know working with multi-threaded projects, the good old one file project of mine was more or less this directory search thingy.

But that's not fun to just reuse that old buddy here without actually writing anything, so I've decided to see if I can create it in a newer fashion (well based on my old fashion codes for 8 years ago). and also talk about using third party libraries especially when they can make life easier for more portable and cross platform implementations.

The project is a cli tool, user pass a path and a keyword and it searches all the files in the directory in batches in `n` threads and shows the result.

## Final Result and Thoughts

I'm quite happy with the result, the structure of code is more or less close to the `POSIX` C code and I like that.

The program searches the given path with traversing over children directories and check files in batches to see if the given keyword exists in them or not and lists the file names with full path.

## Third Party Notices

This project uses the following third-party libraries:

### `dirent.h` (MIT License)

Please refer to [LICENSE](/third_party/dirent/LICENSE).

## License

Creative Commons Attribution-NonCommercial 4.0 International (CC BY-NC 4.0) License.

Please refer to [LICENSE](/LICENSE) file.
