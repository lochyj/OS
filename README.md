# OS

This repository is the collation of knowledge and learnings from tutorials, wikis and others' projects. This project was started to further my knowledge of C and x86 assembly, whilst helping me understand computers at a lower level. This isn't a serious project, and it is exclusively for fun and education!

## What is the name of the OS?
I have decided to call the OS / Kernel BlinkOS
Below is the logo for the os created by https://patorjk.com/software/taag/. It is used in the boot screen of the OS.
```
  ____  _ _       _     ____   _____ 
 |  _ \| (_)     | |   / __ \ / ____|
 | |_) | |_ _ __ | | _| |  | | (___  
 |  _ <| | | '_ \| |/ / |  | |\___ \ 
 | |_) | | | | | |   <| |__| |____) |
 |____/|_|_|_| |_|_|\_\\____/|_____/ 
```

## Building pre-requisites

- nasm
- qemu
- gcc
- grub
- mtools
- xorriso

These are all available on most package managers.

Additionally, you should build your own cross-compiler by following this nice tutorial on the [OSDev Wiki](https://wiki.osdev.org/GCC_Cross-Compiler)


## Running

### Linux

To build the kernel, you need to have the pre-requisites installed. Then run `./build.sh` in the root directory of the project.
- Tested on Fedora 37

### Windows and Mac

Currently I don't have instruction for running or building on windows or mac. If you want to run it on windows or mac, you can use a virtual machine, or a future unimplemented docker version. Additionally, Windows 10/11 users can use the Windows Subsystem for Linux (WSL). I recommend Ubuntu. However, I believe it is possible to compile it without these things on both Windows and Mac, you just need to install the pre-requisites. If you do this, please let me know how you did it and if you ran into any trouble.

## Future plans

I currently don't have any future plans for this project and it is purely for fun and education.

In the meantime here is a list of things i want to implement:

- Use makefiles to build the kernel
- Implement a filesystem such as FAT or EXT
- Create my own LIBC (or copy it from OpenBSD)
- And a few others that are too small to list

## Credits

- [Lachlan Jowett - Contributor](https://github.com/lochyj)
- [Carlos Fenollosa - Repo](https://github.com/cfenollosa/os-tutorial)
- [OS Dev - Wiki](https://wiki.osdev.org/Main_Page)
- [The Linux contributors - Repo](https://github.com/torvalds/linux)
- [The OpenBSD contributors - Repo](https://github.com/openbsd/src)
- [Austanss - Repo](https://github.com/austanss/skylight/)
- [Bran - Tutorial](http://www.osdever.net/bkerndev/Docs/title.htm)

## Legal

Some parts of this project were not created by me (Lachlan Jowett) and I do not claim ownership of them. Alongside this, the individual pieces of code have their own licenses, which can be found in the `LICENSE` file in their respective repos or on the public domain. The individual parts that were created by me are licensed under the MIT license. More details on licensing can be found in the `LICENSE` file in the root directory of this project or if not supplied, at https://github.com/lochyj/OS/blob/main/LICENCE.
