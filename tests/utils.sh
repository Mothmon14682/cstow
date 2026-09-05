#!/usr/bin/env bash

set -euo pipefail

ROOT=$(mktemp -d)

cleanup(){
    rm -rf "$ROOT";
}

trap cleanup EXIT

CSTOW=${CSTOW:-./cstow}

assert_is_link(){
    local path="$1"

    if [[ -L "$path" ]]; then
        echo "[PASS]: Symlink test for: $path"
    else
        echo "[FAILED]: $path is not a symlink"
        exit 1
    fi
}

assert_is_file(){
    local path="$1"

    if [[ -f "$path" ]]; then
        echo "[PASS]: File test for: $path"
    else
        echo "[FAILED]: $path is not a file"
        exit 1
    fi
}

assert_is_dir(){
    local path="$1"

    if [[ -d "$path" ]]; then
        echo "[PASS]: Directory test for: $path"
    else
        echo "[FAILED]: $path is not a directory"
        exit 1
    fi
}

assert_readlink(){
    local symlink="$1"
    local expected="$2"

    if [[ "$(readlink $symlink)" = "$2" ]]; then
        echo "[PASS]: Readed symlink $symlink -> $expected"
    else
        echo "[FAILED]: Symlink $symlink does not point to $expected"
        exit 1
    fi
}

assert_same_content(){
    local file1="$1"
    local file2="$2"

    if [[ "$(cat $file1)" = "$(cat $file2)" ]]; then
        echo "[PASS]: $file1 and $file2 have same the content"
    else
        echo "[FAILED]: $file1 and $file2 not have the same content"
    fi
}
