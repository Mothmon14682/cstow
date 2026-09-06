#!/usr/bin/env bash

source "$(dirname "$0")/../utils.sh"

mkdir -p "$ROOT/package/'nvim test'"
mkdir -p "$ROOT/target"

echo "print('Hello world')" > "$ROOT/package/'nvim test'/init.lua"

~/Code/cstow/build/cstow -s "$ROOT/package" -d "$ROOT/target" "'nvim test'" > /dev/null

assert_is_link "$ROOT/target/init.lua"

assert_same_content "$ROOT/target/init.lua" "$ROOT/package/'nvim test'/init.lua"

echo "===> [PASS]: cstow/package_name_contain_space_test.sh"
