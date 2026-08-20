#!/bin/bash
# Profile PCem CPU execution with perf (LBR branch stacks) + llvm-profgen,
# producing an LLVM *sample* profile for sample-based PGO (AutoFDO).
# Run this from a desktop terminal (perf_event_open and LBR support are
# typically unavailable in containers/sandboxes).
#
# Builds pcem with clang/lld (see configure-llvm.sh), records a perf session
# with branch-stack sampling, then runs llvm-profgen to generate:
#
#   perf.data          raw perf recording (also usable with 'perf report --tui')
#   perf-map.txt       copy of the recompiler JIT map (/tmp/perf-<pid>.map).
#                      llvm-profgen cannot use it, but interactive perf
#                      reports can resolve recompiled blocks with it.
#   code.profdata      LLVM sample profile (llvm-profgen output), consumed at
#                      build time by -fprofile-sample-use=code.profdata
#                      (NOT -fprofile-instr-use: that flag is for instrumented
#                      profiles and rejects llvm-profgen output).
#
# Environment:
#   BUILD_TYPE    RelWithDebInfo (default; realistic -O2 profile with line
#                 info) | Debug (-O0; easier line attribution, but the
#                 profile is not representative of release CPU behaviour)
#   JOBS          parallel build jobs (default: nproc)
#   SKIP_BUILD=1  skip the build step
#   SAMPLE_FREQ   perf sampling frequency (default: 999; raise it if
#                 llvm-profgen warns about low sample density)
#   PROFILE_OUT   llvm-profgen output path (default: code.profdata)
#   PROFGEN_OPTS  extra space-separated args passed through to llvm-profgen
#
# Usage: ./profile-cpu.sh [-- <args passed to pcem>]
#
# Notes:
#   * llvm-profgen requires LBR (branch-stack) data, so recording uses
#     'perf record -b'. On CPUs/kernels without LBR support the record step
#     fails immediately; there is no fallback mode.
#   * JIT-generated recompiler code has no C source and is not attributed
#     by llvm-profgen; only the C/C++ emulator code is profiled.
#   * Stop the recording with Ctrl+C (SIGINT) or by quitting pcem. Killing
#     perf with SIGKILL leaves perf.data truncated ("data size field is 0").

set -uo pipefail
cd "$(dirname "$0")"

BUILD_TYPE="${BUILD_TYPE:-RelWithDebInfo}"
JOBS="${JOBS:-$(nproc)}"
SAMPLE_FREQ="${SAMPLE_FREQ:-999}"
BIN="build/src/pcem"
PERF_DATA="perf.data"
PERF_MAP="perf-map.txt"
PERF_MAP_PID="perf-map.pid"
PROFILE_OUT="${PROFILE_OUT:-code.profdata}"

# --- toolchain checks ---------------------------------------------------------
need() {
    command -v "$1" >/dev/null 2>&1 || {
        echo "error: '$1' not found in PATH. Install the LLVM toolchain + perf." >&2
        exit 1
    }
}
for tool in perf clang clang++ lld ninja cmake llvm-profgen llvm-profdata; do
    need "$tool"
done

# --- build with the LLVM toolchain (clang + lld) ------------------------------
if [[ "${SKIP_BUILD:-0}" != "1" ]]; then
    echo "==> Building with LLVM toolchain (type: $BUILD_TYPE, jobs: $JOBS)"
    CMAKE_ARGS=()
    if [[ "$BUILD_TYPE" != "Debug" ]]; then
        # Frame pointers make DWARF call-graph unwinding cheaper and let
        # backtraces survive where CFI is unavailable.
        CMAKE_ARGS+=(-DCMAKE_C_FLAGS="-fno-omit-frame-pointer")
        CMAKE_ARGS+=(-DCMAKE_CXX_FLAGS="-fno-omit-frame-pointer")
    fi
    ./configure-llvm.sh -t "$BUILD_TYPE" -j "$JOBS" "${CMAKE_ARGS[@]}"
else
    echo "==> Skipping build (SKIP_BUILD=1), using $BIN"
    [[ -x "$BIN" ]] || { echo "error: $BIN not found" >&2; exit 1; }
fi

# --- record -------------------------------------------------------------------
# PCEM_PERFMAP makes the recompiler write /tmp/perf-<pid>.map so JIT code
# gets symbols in interactive perf reports.
# -b (LBR branch stacks) is REQUIRED by llvm-profgen; -g additionally records
# the call stack, giving llvm-profgen "hybrid" samples for better profiles.
echo "==> Recording to $PERF_DATA (Ctrl+C or quit pcem to stop)"
if ! PCEM_PERFMAP=1 perf record -b -g \
        -F "$SAMPLE_FREQ" \
        --call-graph dwarf,16384 \
        -o "$PERF_DATA" \
        -- "$BIN" "$@"; then
    echo "note: perf record exited non-zero." >&2
    if [[ ! -s "$PERF_DATA" ]]; then
        echo "error: no perf.data was written. 'perf record -b' requires LBR" >&2
        echo "       (branch-stack) hardware support; on CPUs/kernels without" >&2
        echo "       it the recording fails immediately. llvm-profgen has no" >&2
        echo "       fallback for non-LBR data." >&2
        exit 1
    fi
    echo "      (Ctrl+C during recording is normal and finalizes the file.)" >&2
fi

if ! perf report -i "$PERF_DATA" --stdio >/dev/null 2>&1; then
    echo "error: $PERF_DATA has no usable samples (was perf killed with" >&2
    echo "       SIGKILL? 'data size field is 0' means the file was never" >&2
    echo "       finalized). Re-run and stop with Ctrl+C." >&2
    exit 1
fi

# --- JIT symbol map -----------------------------------------------------------
# Keep a copy of /tmp/perf-<pid>.map next to perf.data so interactive perf
# reports keep resolving recompiled blocks even after /tmp is cleaned.
# llvm-profgen itself does not read this file.
MAP_FILE="$(ls -t /tmp/perf-*.map 2>/dev/null | head -1)"
if [[ -n "$MAP_FILE" ]] && [[ $(( $(date +%s) - $(stat -c %Y "$MAP_FILE") )) -lt 600 ]]; then
    cp "$MAP_FILE" "$PERF_MAP"
    echo "$MAP_FILE" | sed 's/.*perf-\([0-9]*\)\.map/\1/' > "$PERF_MAP_PID"
    echo "==> JIT map saved: $PERF_MAP (pid $(cat "$PERF_MAP_PID"))"
else
    echo "warning: no fresh /tmp/perf-<pid>.map found - recompiled code will" >&2
    echo "         show up as [unknown] in interactive perf reports." >&2
fi
# --- llvm-profgen -------------------------------------------------------------
echo "==> Generating LLVM sample profile: $PROFILE_OUT"
PROFGEN_OPTS_ARRAY=()
[[ -n "${PROFGEN_OPTS:-}" ]] && read -r -a PROFGEN_OPTS_ARRAY <<< "$PROFGEN_OPTS"
if ! llvm-profgen --binary="$BIN" --perfdata="$PERF_DATA" \
        --output="$PROFILE_OUT" "${PROFGEN_OPTS_ARRAY[@]+"${PROFGEN_OPTS_ARRAY[@]}"}"; then
    echo "error: llvm-profgen failed. Branch-stack (LBR) data is required" >&2
    echo "       ('perf record -b'); see the warnings above." >&2
    exit 1
fi

if [[ -s "$PROFILE_OUT" ]]; then
    echo "==> Profile summary:"
    llvm-profdata show --sample "$PROFILE_OUT" 2>/dev/null | head -20
else
    echo "warning: $PROFILE_OUT is empty or missing - see llvm-profgen" >&2
    echo "         output above (low sample density? missing LBR data?)." >&2
fi

# --- done ---------------------------------------------------------------------
echo
echo "==> Done. Outputs:"
for f in "$PERF_DATA" "$PERF_MAP" "$PROFILE_OUT"; do
    [[ -f "$f" ]] && printf '    %-24s %s\n' "$f" "$(du -h "$f" | cut -f1)"
done
cat <<'TIPS'

Tips:
  * Use the profile in a PGO build as a *sample* profile:
      cmake -S . -B build-pgo -G Ninja -DCMAKE_BUILD_TYPE=Release \
          -DUSE_PGO=ON -DPCEM_PGO_PROFILE_FILE=$PWD/code.profdata
    The build must consume llvm-profgen output with -fprofile-sample-use
    (not -fprofile-instr-use).
  * Low sample density?  SAMPLE_FREQ=4000 ./profile-cpu.sh  and/or run
    pcem longer. llvm-profgen warns when the density is too low.
  * Interactive:  perf report -i perf.data --tui   (uses perf-map.txt for
    recompiled guest blocks, which llvm-profgen cannot attribute).
  * Slower/lines-friendlier build:  BUILD_TYPE=Debug ./profile-cpu.sh
  * If perf.data is ever "not properly terminated", re-record; SIGKILL
    truncates it. Ctrl+C finalizes it correctly.
TIPS

