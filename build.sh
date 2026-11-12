#!/usr/bin/env bash
set -euo pipefail

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

usage() {
  echo "Usage: ./build.sh [all|test]"
  echo "  all   Build both libraries and run host tests if available (default)"
  echo "  test  Run host tests if available"
}

build_core() {
  local core="$1"
  cmake -S "$root_dir" -B "$root_dir/build-$core" \
    -DTITAN_RTOS_CORE="$core" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
  cmake --build "$root_dir/build-$core"
}

run_tests() {
  if [ ! -f "$root_dir/tests/CMakeLists.txt" ]; then
    echo "No host tests are configured yet."
    return
  fi
  cmake -S "$root_dir/tests" -B "$root_dir/build-host"
  cmake --build "$root_dir/build-host"
  (cd "$root_dir/build-host" && ctest --output-on-failure)
}

if [ "$#" -gt 1 ]; then
  usage >&2
  exit 2
fi

case "${1:-all}" in
  test) run_tests ;;
  all) build_core m7; build_core m4; run_tests ;;
  -h|--help) usage ;;
  *) usage >&2; exit 2 ;;
esac
