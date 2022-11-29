# OS

This repository is the collation of knowledge and learnings from tutorials, wikis and others' projects. It is how i learned C and x86 assembly, whilst helping me understand computers at a lower level. This project isn't serious is just for fun!

## Building pre-requisites
- nasm
- qemu
- gcc

## Running

### Linux
    To run the kernel, you need to have qemu installed. Then run `./run.sh` in the root directory of the project.
    To build the kernel, you need to have qemu installed. Then run `./build.sh` in the root directory of the project.

### Windows and Mac

We currently don't have instruction for running or building on windows or mac. If you want to run it on windows or mac, you can use a virtual machine. Additionally, Windows 10+ users can use WSL. I recommend Arch or Ubuntu distros.

## Credits

- [Lachlan Jowett](https://github.com/lochyj)
- [Pritam Zope - Tutorial](https://www.codeproject.com/Articles/1225196/Create-Your-Own-Kernel-In-C-2)
- [Carlos Fenollosa - Repo](https://github.com/cfenollosa/os-tutorial)
- [OS Dev - Wiki](https://wiki.osdev.org/Main_Page)
- [Frank Rosner - Tutorial that I used](https://dev.to/frosnerd)
- [The linux contributers - Repo](https://github.com/torvalds/linux)

## Legal

Some parts of this project were not created by me (Lachlan Jowett) and I do not claim ownership of them. Alongside this, the individual pieces of code have their own licenses, which can be found in the `LICENSE` file in their respective repos or on the public domain. The individual parts that were created by me are licensed under the MIT license. More details on licensing can be found in the `LICENSE` file in the root directory of this project or if not supplied, at https://github.com/lochyj/OS/blob/main/LICENCE.
