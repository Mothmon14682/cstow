#!/usr/bin/env bash

source "$(dirname "$0")/../utils.sh"

mkdir -p "$ROOT/package/nvim"
mkdir -p "$ROOT/package/nvim/nvim"
mkdir -p "$ROOT/target"

echo "print('Hello world')" > "$ROOT/package/nvim/nvim/init.lua"
mkdir -p "$ROOT/target/nvim/init.lua"

before=$(ls -la "$ROOT/target")

if ~/Code/cstow/build/cstow -s "$ROOT/package" -d "$ROOT/target" "nvim" > /dev/null; then
    echo "[FAILED]: cstow unexpectedly succeeded"
    exit 1
fi

after=$(ls -la "$ROOT/target")

if [[ "$before" != "$after" ]]; then
    echo "[FAILED]: the filesystem did change after a failure"
fi

echo "===> [PASS]: cstow/multiple_packages_test.sh"
