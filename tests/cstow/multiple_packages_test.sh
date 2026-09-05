#!/usr/bin/env bash

source "$(dirname "$0")/../utils.sh"

mkdir -p "$ROOT/package/nvim"
mkdir -p "$ROOT/package/nvim/nvim"
mkdir -p "$ROOT/package/kitty/kitty"
mkdir -p "$ROOT/target"

echo "print('Hello world')" > "$ROOT/package/nvim/nvim/init.lua"
echo "this is a test" > "$ROOT/package/kitty/kitty/kitty.conf"

~/Code/cstow/build/cstow -s "$ROOT/package" -d "$ROOT/target" "nvim" "kitty" > /dev/null

assert_is_link "$ROOT/target/nvim"
assert_is_link "$ROOT/target/kitty"
assert_is_file "$ROOT/target/nvim/init.lua"
assert_is_file "$ROOT/target/kitty/kitty.conf"

assert_readlink "$ROOT/target/nvim" "$ROOT/package/nvim/nvim"
assert_readlink "$ROOT/target/kitty" "$ROOT/package/kitty/kitty"
assert_same_content "$ROOT/target/nvim/init.lua" "$ROOT/package/nvim/nvim/init.lua"
assert_same_content "$ROOT/target/kitty/kitty.conf" "$ROOT/package/kitty/kitty/kitty.conf"

echo "===> [PASS]: cstow/multiple_packages_test.sh"
