#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ALIAS_FILE="$HOME/.bash_aliases"

if [ ! -f "$ALIAS_FILE" ]; then
    touch "$ALIAS_FILE"
fi

add_alias_if_missing() {
    local name=$1
    local cmd=$2
    if ! grep -q "^alias $name=" "$ALIAS_FILE"; then
        echo "alias $name=\"$cmd\"" >> "$ALIAS_FILE"
    else
        echo "Alias '$name' is already defined in $ALIAS_FILE" >&2
    fi
}

add_alias_if_missing run-app     "$SCRIPT_DIR/run-app.sh"
add_alias_if_missing build-app   "make -C $SCRIPT_DIR/../build"
add_alias_if_missing clean-build "make -C $SCRIPT_DIR/../build clean"

if [ -f "$HOME/.bashrc" ]; then
    source "$HOME/.bashrc"
fi

set +e
