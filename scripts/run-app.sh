#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

CONFIG=Debug
EXECUTABLE_NAME=Game

if [ -n "$1" ]; then
    if [ "${1,,}" = "debug" ]; then
        CONFIG=Debug
    elif [ "${1,,}" = "release" ]; then
        CONFIG=Release
    else
        echo "Error: Invalid configuration '$1'. Expected 'Debug' or 'Release'."
        exit 1
    fi
fi

BUILD_OUTPUT_DIR="$SCRIPT_DIR/../bin/$CONFIG"
EXECUTABLE_PATH="$BUILD_OUTPUT_DIR/$EXECUTABLE_NAME"

if [ -x "$EXECUTABLE_PATH" ]; then
    cd "$SCRIPT_DIR/../"
    "$EXECUTABLE_PATH"
else
    echo "Error: $EXECUTABLE_PATH can't be executed or doesn't exist!" >&2
    exit 1
fi
