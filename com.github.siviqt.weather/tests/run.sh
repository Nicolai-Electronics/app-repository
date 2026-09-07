#!/usr/bin/env bash
set -eu
sdk=${1:?Pass the Tanmatsu launcher source directory}
root=$(cd "$(dirname "$0")/.." && pwd)
test_dir=$(mktemp -d)
trap 'rm -rf "$test_dir"' EXIT
common=(-std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined -g -I "$root/include")
cc "${common[@]}" "$root/tests/test_core.c" "$root/src/weather.c" "$root/src/json.c" -o "$test_dir/core"
"$test_dir/core"
cc "${common[@]}" -pthread -I "$sdk/components/plugin-api/include" -I "$sdk/managed_components/badgeteam__badge-elf-api/include" \
  "$root/tests/test_service.c" "$root/src/weather.c" "$root/src/json.c" -o "$test_dir/service"
"$test_dir/service"
