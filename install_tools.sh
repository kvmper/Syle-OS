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
echo
echo -e "\e[33mIf you do not know what distro you are currently using, refer to above.\e[0m"

select option in "Arch-based" "Debian-based" "Fedora-based" "Other"
do
    case "$REPLY" in
    1)
        read -p "Which AUR helper do you want to use? [yay/paru/none] " aur_helper
        aur_helper=${aur_helper:-none}
       
        if [[ "$aur_helper" =~ ^(yay|paru)$ ]]; then
            if ! command -v "$aur_helper" &> /dev/null; then
                echo "You do not have $aur_helper installed, please install $aur_helper and re-run this script or choose not to use $aur_helper"
            else 
                $aur_helper -Syu
                $aur_helper -S --noconfirm "${packages_arch[@]}"
            fi
        else
            sudo pacman -Syu --noconfirm
            sudo pacman -S --noconfirm "${packages_arch[@]}"
        fi
        break
        ;;
    2)
        
        if ! command -v apt &> /dev/null; then
            echo "You are not using a debian-based distro" 
        else
            sudo apt update && sudo apt upgrade -y
            sudo apt install -y "${packages_debian[@]}"
        fi
        break
        ;;
    3)
        if ! command -v dnf &> /dev/null; then
            echo "You are not using a fedora-based distro" 
        else
            sudo dnf upgrade --refresh -y
            sudo dnf install -y "${packages_fedora[@]}"
        fi

        break
        ;;
    4)
        echo "Sorry, but you cannot run this script"
        break
        ;;
    *)
        echo "Invalid choice"
        ;;
    esac
done
