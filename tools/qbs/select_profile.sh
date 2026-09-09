#!/usr/bin/env bash
set -euo pipefail

if ! command -v qbs >/dev/null 2>&1; then
    echo "qbs is not installed" >&2
    exit 1
fi

preferred=(
    "x86_64-linux-gnu-gcc-14"
    "x86_64-linux-gnu-gcc-13"
    "gcc14"
    "gcc13"
    "gcc"
    "clang-20"
    "clang20"
    "default"
)

profiles_dump="$(qbs config --list profiles 2>/dev/null || true)"
if [[ -z "${profiles_dump}" ]]; then
    qbs setup-toolchains --detect >/dev/null
    profiles_dump="$(qbs config --list profiles 2>/dev/null || true)"
fi

if [[ -z "${profiles_dump}" ]]; then
    echo "no qbs profiles found" >&2
    exit 1
fi

mapfile -t profiles < <(printf '%s\n' "${profiles_dump}" | sed -n 's/^profiles\.\([^.:]*\)\..*$/\1/p' | sort -u)

if [[ ${#profiles[@]} -eq 0 ]]; then
    echo "no qbs profiles parsed" >&2
    exit 1
fi

for candidate in "${preferred[@]}"; do
    for profile in "${profiles[@]}"; do
        if [[ "${profile}" == "${candidate}" ]]; then
            echo "${profile}"
            exit 0
        fi
    done
done

echo "${profiles[0]}"
