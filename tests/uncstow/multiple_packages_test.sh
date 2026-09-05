#!/usr/bin/env bash

source "$(dirname "$0")/../utils.sh"

mkdir -p "$ROOT/package/nvim"
mkdir -p "$ROOT/package/nvim/nvim"
mkdir -p "$ROOT/package/kitty/kitty"
mkdir -p "$ROOT/target"

echo "print('Hello world')" > "$ROOT/package/nvim/nvim/init.lua"
echo "this is a test" > "$ROOT/package/kitty/kitty/kitty.conf"

~/Code/cstow/build/cstow -s "$ROOT/package" -d "$ROOT/target" "nvim" "kitty" > /dev/null

after_stow=$(ls -a "$ROOT/target")

~/Code/cstow/build/cstow -s "$ROOT/package" -d "$ROOT/target" -D "nvim" "kitty" > /dev/null

after_unstow=$(ls -a "$ROOT/target")

if [[ "$after_stow" != "$after_unstow" ]]; then
    echo "[PASS]: Package uncstowed"
else
    echo "[FAILED]: Package is not uncstowed"
    exit 1
fi

echo "===> [PASS]: uncstow/multiple_packages_test.sh"
