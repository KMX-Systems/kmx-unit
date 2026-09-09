#!/usr/bin/env bash
set -euo pipefail

if [[ $# -eq 0 ]]; then
    echo "usage: run_with_compiler_runtime.sh <command> [args...]" >&2
    exit 1
fi

compiler="${CXX:-g++}"
runtime_library=""
runtime_library_soname=""
runtime_path_entries=()

append_runtime_dir()
{
    local library_path="${1}"
    if [[ -z "${library_path}" ]]; then
        return
    fi
    if [[ ! -f "${library_path}" ]]; then
        return
    fi

    local runtime_dir
    runtime_dir="$(cd "$(dirname "${library_path}")" && pwd)"
    for existing in "${runtime_path_entries[@]:-}"; do
        if [[ "${existing}" == "${runtime_dir}" ]]; then
            return
        fi
    done
    runtime_path_entries+=("${runtime_dir}")
}

if command -v "${compiler}" >/dev/null 2>&1; then
    runtime_library="$(${compiler} -print-file-name=libstdc++.so 2>/dev/null || true)"
    runtime_library_soname="$(${compiler} -print-file-name=libstdc++.so.6 2>/dev/null || true)"

    # Prefer the directory the compiler uses for linking, then add the soname path as a fallback.
    append_runtime_dir "${runtime_library}"
    append_runtime_dir "${runtime_library_soname}"
fi

if [[ ${#runtime_path_entries[@]} -ne 0 ]]; then
    runtime_prefix="$(IFS=:; printf '%s' "${runtime_path_entries[*]}")"
    if [[ -n "${LD_LIBRARY_PATH:-}" ]]; then
        export LD_LIBRARY_PATH="${runtime_prefix}:${LD_LIBRARY_PATH}"
    else
        export LD_LIBRARY_PATH="${runtime_prefix}"
    fi
fi

exec "$@"
