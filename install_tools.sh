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
  qemu
)

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
                for pkg in "${packages[@]}"; do
                    yay -S --noconfirm "$pkg"
                done
            fi
        else
            sudo pacman -Syu
            for pkg in "${packages[@]}"; do
                sudo pacman -S --noconfirm "$pkg"
            done
        fi
        break
        ;;
    2)
        
        ;;
    3)
        ;;
    4)
        echo "Sorry, but you cannot run this script"
        ;;
    *)
        echo "Invalid choice"
        ;;
    esac
done
