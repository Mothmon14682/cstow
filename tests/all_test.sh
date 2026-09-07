#!/usr/bin/env bash

set -e

folders=(cstow uncstow)

for folder in "${folders[@]}"; do
    for script in /home/tuan/Code/cstow/tests/$folder/*.sh; do
        bash "$script"
    done
done
