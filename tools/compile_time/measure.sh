#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/../.." && pwd)"
# Everything the repository produces goes under one directory; KMX_UNIT_OUTPUT_DIR moves it elsewhere.
output_root="${KMX_UNIT_OUTPUT_DIR:-${repo_root}/output}"
iterations="${1:-5}"
csv_output="${2:-${output_root}/compile_time/report.csv}"
compiler="${CXX:-g++}"

if ! command -v "${compiler}" >/dev/null 2>&1; then
    echo "compiler not found: ${compiler}" >&2
    exit 1
fi

if ! command -v /usr/bin/time >/dev/null 2>&1; then
    echo "/usr/bin/time is required for timing" >&2
    exit 1
fi

if ! [[ "${iterations}" =~ ^[0-9]+$ ]] || [[ "${iterations}" -eq 0 ]]; then
    echo "iterations must be a positive integer" >&2
    exit 1
fi

common_flags=(
    -std=c++23
    -I"${repo_root}/source/inc"
    -O2
    -DNDEBUG
    -fsyntax-only
)

measure_probe()
{
    local label="${1}"
    local source="${2}"
    local values=()

    for ((i = 1; i <= iterations; ++i)); do
        local elapsed
        elapsed="$({ /usr/bin/time -f "%e" "${compiler}" "${common_flags[@]}" "${source}" >/dev/null; } 2>&1)"
        values+=("${elapsed}")
    done

    local summary
    summary="$(printf '%s\n' "${values[@]}" | awk '
        NR == 1 { min = $1; max = $1; sum = 0 }
        {
            if ($1 < min) min = $1;
            if ($1 > max) max = $1;
            sum += $1;
        }
        END {
            printf "%.3f %.3f %.3f", sum / NR, min, max;
        }
    ')"

    echo "${label};${summary}"
}

all_result="$(measure_probe all.hpp "${script_dir}/probe_all.cpp")"
selective_result="$(measure_probe selective "${script_dir}/probe_selective.cpp")"

all_avg="$(awk -F';' '{print $2}' <<<"${all_result}" | awk '{print $1}')"
sel_avg="$(awk -F';' '{print $2}' <<<"${selective_result}" | awk '{print $1}')"
ratio="$(awk -v all_avg="${all_avg}" -v sel_avg="${sel_avg}" 'BEGIN { printf "%.2f", all_avg / sel_avg }')"

all_csv_row="$(awk -F';' '{printf "%s,%s", $1, $2}' <<<"${all_result}")"
selective_csv_row="$(awk -F';' '{printf "%s,%s", $1, $2}' <<<"${selective_result}")"

echo "Compiler: ${compiler}"
echo "Iterations: ${iterations}"
echo "Flags: ${common_flags[*]}"
echo
echo "label,avg_s,min_s,max_s"
echo "${all_csv_row}"
echo "${selective_csv_row}"
echo "ratio_all_over_selective,${ratio}"

if [[ -n "${csv_output}" ]]; then
    mkdir -p "$(dirname "${csv_output}")"
    {
        echo "label,avg_s,min_s,max_s"
        echo "${all_csv_row}"
        echo "${selective_csv_row}"
        echo "ratio_all_over_selective,${ratio}"
    } > "${csv_output}"
fi

echo
if "${compiler}" --version | head -n 1 | grep -Eiq 'clang'; then
    trace_dir="${output_root}/compile_time/traces"
    mkdir -p "${trace_dir}"
    trace_flags=(
        -std=c++23
        -I"${repo_root}/source/inc"
        -O2
        -DNDEBUG
        -ftime-trace
        -c
    )

    "${compiler}" "${trace_flags[@]}" "${script_dir}/probe_all.cpp" -o "${trace_dir}/probe_all.o"
    "${compiler}" "${trace_flags[@]}" "${script_dir}/probe_selective.cpp" -o "${trace_dir}/probe_selective.o"

    echo "Clang trace files:"
    find "${trace_dir}" -maxdepth 1 -type f -name '*.json' -print | sort
else
    echo "Skipping trace generation: clang required for -ftime-trace."
fi
