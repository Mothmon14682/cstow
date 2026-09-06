#!/usr/bin/env bash

source "$(dirname "$0")/../utils.sh"

mkdir -p "$ROOT/package/'nvim test'"
mkdir -p "$ROOT/target"

echo "print('Hello world')" > "$ROOT/package/'nvim test'/init.lua"

~/Code/cstow/build/cstow -s "$ROOT/package" -d "$ROOT/target" "'nvim test'" > /dev/null

after_stow=$(ls -a "$ROOT/target")

~/Code/cstow/build/cstow -s "$ROOT/package" -d "$ROOT/target" -D "'nvim test'" > /dev/null

after_unstow=$(ls -a "$ROOT/target")

if [[ "$after_stow" != "$after_unstow" ]]; then
    echo "[PASS]: Package uncstowed"
else
    echo "[FAILED]: Package is not uncstowed"
    exit 1
fi

echo "===> [PASS]: uncstow/package_name_contain_space_test.sh"
