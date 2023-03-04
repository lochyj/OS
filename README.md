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

## Running

### Linux
    To build the kernel, you need to have qemu installed. Then run `./build.sh` in the root directory of the project.

### Windows and Mac

We currently don't have instruction for running or building on windows or mac. If you want to run it on windows or mac, you can use a virtual machine, or a future unimplemented docker version. Additionally, Windows 10+ users can use WSL. I recommend Arch or Ubuntu distros.

## Credits

- [Lachlan Jowett](https://github.com/lochyj) -- MIT License
- [Frank Rosner - Tutorial that I used](https://dev.to/frosnerd) -- No License

- [Pritam Zope - Tutorial](https://www.codeproject.com/Articles/1225196/Create-Your-Own-Kernel-In-C-2) -- Inspiration
- [Carlos Fenollosa - Repo](https://github.com/cfenollosa/os-tutorial) -- Inspiration
- [OS Dev - Wiki](https://wiki.osdev.org/Main_Page) -- Helped me fix a stupid mistake
- [Max Schlüssel - Ghost - repo](https://github.com/maxdev1/ghost) -- Inspiration
- [The linux contributers - Repo](https://github.com/torvalds/linux) -- Inspiration

## Legal

Some parts of this project were not created by me (Lachlan Jowett) and I do not claim ownership of them. Alongside this, the individual pieces of code have their own licenses, which can be found in the `LICENSE` file in their respective repos or on the public domain. The individual parts that were created by me are licensed under the MIT license. More details on licensing can be found in the `LICENSE` file in the root directory of this project or if not supplied, at https://github.com/lochyj/OS/blob/main/LICENCE.
