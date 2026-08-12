#!/usr/bin/env bash
#
# configure-llvm.sh - configure & build PCem with the LLVM toolchain
# (clang / clang++ compiler, lld linker, Ninja generator) and produce a
# binary debuggable with lldb (CodeLLDB in VSCode).
#
# Usage:
#   ./configure-llvm.sh [options] [-- <extra cmake args>]
#
# Options:
#   -b, --build-dir DIR   Build directory                (default: build)
#   -t, --build-type T    RelWithDebInfo|Debug|Release   (default: RelWithDebInfo)
#   -j, --jobs N          Parallel build jobs            (default: nproc)
#   -h, --help            Show this help
#
# Environment overrides: CC, CXX, BUILD_TYPE
# Anything after "--" (or unknown -D flags) is passed through to CMake,
# e.g.:  ./configure-llvm.sh -DUSE_NETWORKING=OFF
#
set -euo pipefail

BUILD_DIR="build"
BUILD_TYPE="${BUILD_TYPE:-RelWithDebInfo}"
JOBS="$(nproc 2>/dev/null || echo 4)"
EXTRA_ARGS=()

usage() {
    sed -n '2,19p' "$0" | sed 's/^# \{0,1\}//'
    exit "${1:-0}"
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        -b|--build-dir)   BUILD_DIR="$2"; shift 2 ;;
        -t|--build-type)  BUILD_TYPE="$2"; shift 2 ;;
        -j|--jobs)        JOBS="$2"; shift 2 ;;
        -h|--help)        usage ;;
        --)               shift; EXTRA_ARGS+=("$@"); break ;;
        -*)               EXTRA_ARGS+=("$1"); shift ;;
        *)                usage 1 ;;
    esac
done

case "$BUILD_TYPE" in
    RelWithDebInfo|Debug|Release) ;;
    *) echo "error: unsupported build type '$BUILD_TYPE' (use RelWithDebInfo|Debug|Release)" >&2; exit 1 ;;
esac

# --- toolchain checks -------------------------------------------------------
for tool in clang clang++ lld ninja cmake; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "error: '$tool' not found in PATH." >&2
        echo "       Install the LLVM toolchain, e.g.:  pacman -S clang lld  (Arch)" >&2
        echo "                                  or:  apt install clang lld  (Debian)" >&2
        exit 1
    fi
done

# Prefer the system LLVM installation in /usr/bin (native build, matched lldb);
# fall back to PATH lookup. Override with CC/CXX.
resolve_tool() {
    local name="$1"
    if [[ "$name" != */* ]] && [[ -x "/usr/bin/$name" ]]; then
        echo "/usr/bin/$name"
    else
        command -v "$name"
    fi
}

CC="${CC:-clang}"
CXX="${CXX:-clang++}"
CC_PATH="$(resolve_tool "$CC")"
CXX_PATH="$(resolve_tool "$CXX")"

echo "==> PCem LLVM build"
echo "    C compiler : $CC_PATH"
echo "    C++ compiler: $CXX_PATH"
echo "    Build dir  : $BUILD_DIR   (type: $BUILD_TYPE)"
"$CC_PATH" --version | head -n 1 | sed 's/^/    /'

# --- linker: prefer LLD through CMAKE_LINKER_TYPE (CMake >= 3.28) ----------
LINKER_ARGS=()
CMAKE_VER="$(cmake --version | head -n 1 | sed -E 's/.*version ([0-9.]+).*/\1/')"
if [[ "$(printf '%s\n3.28' "$CMAKE_VER" | sort -V | head -n 1)" == "3.28" ]]; then
    LINKER_ARGS+=(-DCMAKE_LINKER_TYPE=LLD)
else
    LINKER_ARGS+=(-DCMAKE_C_FLAGS="-fuse-ld=lld" -DCMAKE_CXX_FLAGS="-fuse-ld=lld")
fi

# --- configure --------------------------------------------------------------
echo "==> Configuring with CMake $CMAKE_VER"
cmake -S . -B "$BUILD_DIR" \
    -G Ninja \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_C_COMPILER="$CC_PATH" \
    -DCMAKE_CXX_COMPILER="$CXX_PATH" \
    "${LINKER_ARGS[@]}" \
    "${EXTRA_ARGS[@]}"

# --- build -------------------------------------------------------------------
echo "==> Building pcem (jobs=$JOBS)"
cmake --build "$BUILD_DIR" --target pcem -j "$JOBS"

echo
echo "==> Done."
echo "    Binary : $BUILD_DIR/src/pcem"
if LLDB_PATH="$(resolve_tool lldb 2>/dev/null)" && [[ -n "$LLDB_PATH" ]]; then
    echo "    Debug  : lldb $BUILD_DIR/src/pcem   (or press F5 in VSCode with CodeLLDB)"
else
    echo "    Debug  : install lldb, or use the CodeLLDB VSCode extension (F5)"
fi
