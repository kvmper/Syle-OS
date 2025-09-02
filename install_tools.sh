#!/bin/bash

packages_arch=(
  base-devel
  nasm
  gcc
  lib32-glibc
  binutils
  grub
  xorriso
  mtools
  qemu-full
  gmp
  libmpc
  mpfr
)
packages_debian=(
  build-essential
  nasm
  libc6-i386
  binutils
  grub-pc
  xorriso
  mtools
  qemu
  bison
  flex
  libgmp3-dev
  libmpc-dev
  libmpfr-dev
  texinfo
  libisl-dev
)
packages_fedora=(
  @development-tools
  nasm
  glibc.i686
  binutils
  grub2
  xorriso
  mtools
  qemu

  bison
  flex
  gmp-devel
  mpc-devel
  mpfr-devel
  texinfo
  isl-devel
)


cat /etc/os-release

select option in "Arch-based" "Debian-based" "Fedora" "Other"
do
    case "$REPLY" in
    1)
        read -p "Do you want to use yay? [Y/n] " answer
        answer=${answer:-Y}
        if [[ "$answer" =~ ^[Yy]$ ]]; then
            if ! command -v yay &> /dev/null; then
                echo "You do not have yay installed"
            else 
                yay -Syu
                yay -S --noconfirm "${packages_arch[@]}"
            fi
        else
            sudo pacman -Syu --noconfirm
            sudo pacman -S --noconfirm "${packages_arch[@]}"
        fi
        break
        ;;
    2)
        sudo apt update && sudo apt upgrade -y
        sudo apt install -y "${packages_debian[@]}"
        break
        ;;
    3)
        sudo dnf upgrade --refresh -y
        sudo dnf install -y "${packages_fedora[@]}"
        break
        ;;
    4)
        echo "Sorry, but you cannot run this script"
        ;;
    *)
        echo "Invalid choice"
        ;;
    esac
done
