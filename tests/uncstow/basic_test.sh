source "$(dirname "$0")/../utils.sh"

mkdir -p "$ROOT/package/.config"
mkdir -p "$ROOT/package/.config/nvim"
mkdir -p "$ROOT/target"

echo "print('Hello world')" > "$ROOT/package/.config/nvim/init.lua"

~/Code/cstow/build/cstow -s "$ROOT/package" -d "$ROOT/target" ".config" > /dev/null
~/Code/cstow/build/cstow -s "$ROOT/package" -d "$ROOT/target" -D ".config" > /dev/null

output=$(ls "$ROOT/target")

if [[ -z "$output" ]]; then
    echo "[PASS]: Package uncstowed"
else
    echo "[FAILED]: Package is not uncstowed"
    exit 1
fi

echo "===> [PASS]: uncstow/basic_test.sh"
