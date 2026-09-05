#!/usr/bin/env bash

source "$(dirname "$0")/../utils.sh"

mkdir -p "$ROOT/package/.config"
mkdir -p "$ROOT/package/.config/nvim"
mkdir -p "$ROOT/target"

echo "print('Hello world')" > "$ROOT/package/.config/nvim/init.lua"

~/Code/cstow/build/cstow -s "$ROOT/package" -d "$ROOT/target" ".config" > /dev/null

assert_is_link "$ROOT/target/nvim"
assert_is_file "$ROOT/target/nvim/init.lua"

assert_readlink "$ROOT/target/nvim" "$ROOT/package/.config/nvim"
assert_same_content "$ROOT/target/nvim/init.lua" "$ROOT/package/.config/nvim/init.lua"

echo "===> [PASS]: cstow/basic_test.sh"
