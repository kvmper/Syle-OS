# Syle OS - Master branch

### Current tasks:
- **Everything** needs to be remade
- Organize the code and split it into multiple files
- Replace grub with a custom multiboot bootloader
- Add more functionality
- Add documentation
- Optimize

## Building and running

### Prerequisites
Run the `install_tools.sh` script. You can do it by going into the directory of that file and running `chmod +x install_tools.sh` and `./install_tools.sh`.
> Note: Only Arch, Fedora and Debian based distos are supported. I'm gonna update the script someday to make it better and support way more distributions. 

To build the system, you need to be in root of the project then run `make build`. This will build the object files and the kernel.bin file then combine them into an .iso file, the file is located in /iso/ and you can run it by using `make run`.

## What will the finished OS be?
The finished Operating System will have most basic features such as, file creation, reading from disk, and a basic FAT16 filesystem. It will also consist of a basic shell which can do tasks such as removing, creating files and way more.
## Is it near completion, when will it be finished?
No, it is not anywhere **near** completion, it will take years for it to be finished. I'm not thinking of stopping working on this project anytime soon. I'm looking for people who will help me find issues with this project and provide recommendations to improve the project as it grows.

## Development

Currently, the development is focused on scripts. After I finish making scripts work will continue onto the kernel by adding things mentioned in the "Current tasks" section. The tree of the project is also gonna be improved and non-essential stuff will be removed.

**Current state:** Development is slowed down, but has not stopped (everything will become way better)
