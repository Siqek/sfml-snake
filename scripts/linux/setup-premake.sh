#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

premake5 --file="$SCRIPT_DIR/../../premake5.lua" gmake
