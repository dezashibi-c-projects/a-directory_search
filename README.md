# Multi Threaded Directory Search in C

Using Multi-threading and directory traversing in a cross platform way, using third party libraries, yes or no.

## Introduction

### Multi Threading on Windows and `POSIX` systems

Creating multi threaded programs are challenging by themselves, making them work cross platform is even more.

Generally on Windows we're supposed to use `windows.h` and `CreateThread` and other fellas to manage threads while on `POSIX` operating systems we're using `pthread.h` to manage them and then linking the executable with `pthread` library using `-lpthread` flag.

Well, `gcc` on windows "supports" using `pthread` but you need to use `gcc` compiler which is totally fine and also worth mentioning here that we still need to use `-lpthread` flag while compiling our program.

I've been using both APIs (Windows and POSIX) and I was thinking to create something that can switch between them accordingly, I've done that but in a higher level meaning not for the specific thread operation individually. So as every "insane" programmer must do first I've searched to see if someone else already did that or not, fortunately I've found [CThreads](https://github.com/PerformanC/CThreads) by [@ThePedroo](https://github.com/ThePedroo).

### Should I use libs by others?

That's a tough question to answer to be honest but I personally have my own answer that works for me, so I'm going to share it with you the dear reader.

- I always start with the answer "No" then I go to the following process
- I search to see if there is any similar work done
- If Yes:
  - First license:
    - I never argue over giving attribution and I think in my idea giving attribution is a must even if the programmer didn't ask for it.
    - If I'm almost sure I might use my work in a commercial way in any possible way I just quickly check to see if the license is `GPL` based and I quickly close the browser tab if so.
    - I prefer MIT Licenses, BSD and Apache are good as well. I respect `Public Domain` and `UnLicence` projects, and I try to go for them first and if I can I would support them, why not.
  - Second implementation:
    - Over-complication is not my cup of tea.
    - Bundling too many things together is my enemy.
    - `CMakeLists.txt` --> Meh 😒, I really prefer two file projects (one header and one implementation) even more than single header one. but single header project works for me too.
    - If that's almost close to what I've had in mind then why bother, I go for it. I'm not suffering from masochism to redundantly implement everything by myself.
      - **IMPORTANT TIP HERE:** That's true only when it's not a over-complication, badly crowded, hard to debug project.
    - I would prefer to see my preferred conventions as well but if the criteria mentioned previously are met it's not a big deal just to fork the project and change it to my taste.
- So that's when I say yes use third party libraries.

I like [CThreads](https://github.com/PerformanC/CThreads) by [@ThePedroo](https://github.com/ThePedroo), it really is the closest implementation to my mind and heart! I even have no problem with namings.

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

`dirent.h` is totally match to the `POSIX` version, `CThreads` on the other hand is as close to `pthread` as it can be which is very nice.

The program searches the given path with traversing over children directories and check files in batches to see if the given keyword exists in them or not and lists the file names with full path.

## Third Party Notices

This project uses the following third-party libraries:

### `dirent.h` (MIT License)

Please refer to [LICENSE](/third_party/dirent/LICENSE).

### `CThreads` (BSD 2-Clause License)

Please refer to [LICENSE](/third_party/CThreads/LICENSE).

## License

This project is licensed under the CC BY-NC 4.0 License. See the [LICENSE](LICENSE) file for details.
