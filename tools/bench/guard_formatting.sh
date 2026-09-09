#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/../.." && pwd)"
# Everything the repository produces goes under one directory; KMX_UNIT_OUTPUT_DIR moves it elsewhere. It is
# also where the test binary is searched for when the caller names neither an executable nor a directory.
output_root="${KMX_UNIT_OUTPUT_DIR:-${repo_root}/output}"
target_path="${1:-${output_root}}"
warn_ratio="${KMX_UNIT_FORMAT_BENCH_WARN_RATIO:-2.00}"
fail_ratio="${KMX_UNIT_FORMAT_BENCH_FAIL_RATIO:-2.50}"
result_csv="${KMX_UNIT_FORMAT_BENCH_RESULT_PATH:-${output_root}/benchmark/formatting-guard.csv}"

if ! command -v bash >/dev/null 2>&1; then
    echo "bash is required" >&2
    exit 1
fi

resolve_test_binary()
{
    local candidate="${1}"

    if [[ -f "${candidate}" ]] && [[ -x "${candidate}" ]]; then
        printf '%s\n' "${candidate}"
        return 0
    fi

    if [[ -d "${candidate}" ]]; then
        local matches=()
        mapfile -t matches < <(find "${candidate}" -type f \( -name 'kmx-unit-test' -o -name 'unit-test' -o -name 'unit-test.exe' \) | sort)
        if [[ ${#matches[@]} -ne 0 ]]; then
            printf '%s\n' "${matches[0]}"
            return 0
        fi
    fi

    return 1
}

if ! test_binary="$(resolve_test_binary "${target_path}")"; then
    echo "benchmark binary not found from: ${target_path}" >&2
    exit 1
fi

set +e
benchmark_output="$(
    CXX="${CXX:-g++}" \
    bash "${repo_root}/tools/cmake/run_with_compiler_runtime.sh" \
        "${test_binary}" "Formatting with width and nested width" "[benchmark]" 2>&1
)"
benchmark_status=$?
set -e

printf '%s\n' "${benchmark_output}"
if [[ ${benchmark_status} -ne 0 ]]; then
    echo "failed to execute formatter benchmark" >&2
    exit ${benchmark_status}
fi

extract_mean_us()
{
    local label="${1}"
    awk -v label="${label}" '
        index($0, label) == 1 {
            if (getline line) {
                n = split(line, fields, /[[:space:]]+/)
                for (i = 1; i <= n; ++i)
                    if (fields[i] ~ /^[0-9]+([.][0-9]+)?([eE][-+]?[0-9]+)?$/) {
                        print fields[i]
                        exit
                    }
            }
        }
    ' <<<"${benchmark_output}"
}

raw_fixed="$(extract_mean_us "raw: format_to {:>12.2f}")"
unit_fixed="$(extract_mean_us "unit: format_to {:>12.2f} meter")"
raw_nested="$(extract_mean_us "raw: format_to {:>{}.2f}")"
unit_nested="$(extract_mean_us "unit: format_to {:>{}.2f} meter")"

for value_name in raw_fixed unit_fixed raw_nested unit_nested; do
    if [[ -z "${!value_name}" ]]; then
        echo "could not parse benchmark mean for ${value_name}" >&2
        exit 1
    fi
done

fixed_ratio="$(awk -v raw="${raw_fixed}" -v unit="${unit_fixed}" 'BEGIN { printf "%.3f", unit / raw }')"
nested_ratio="$(awk -v raw="${raw_nested}" -v unit="${unit_nested}" 'BEGIN { printf "%.3f", unit / raw }')"

echo "formatter_guard_fixed_ratio=${fixed_ratio}"
echo "formatter_guard_nested_ratio=${nested_ratio}"
echo "formatter_guard_warn_threshold=${warn_ratio}"
echo "formatter_guard_fail_threshold=${fail_ratio}"

if [[ -n "${result_csv}" ]]; then
    mkdir -p "$(dirname "${result_csv}")"
    {
        echo "metric,value"
        echo "raw_fixed_mean_us,${raw_fixed}"
        echo "unit_fixed_mean_us,${unit_fixed}"
        echo "raw_nested_mean_us,${raw_nested}"
        echo "unit_nested_mean_us,${unit_nested}"
        echo "fixed_ratio,${fixed_ratio}"
        echo "nested_ratio,${nested_ratio}"
        echo "warn_ratio,${warn_ratio}"
        echo "fail_ratio,${fail_ratio}"
    } > "${result_csv}"
fi

warn_triggered=0
fail_triggered=0

if awk -v value="${fixed_ratio}" -v threshold="${warn_ratio}" 'BEGIN { exit !(value > threshold) }'; then
    warn_triggered=1
fi
if awk -v value="${nested_ratio}" -v threshold="${warn_ratio}" 'BEGIN { exit !(value > threshold) }'; then
    warn_triggered=1
fi
if awk -v value="${fixed_ratio}" -v threshold="${fail_ratio}" 'BEGIN { exit !(value > threshold) }'; then
    fail_triggered=1
fi
if awk -v value="${nested_ratio}" -v threshold="${fail_ratio}" 'BEGIN { exit !(value > threshold) }'; then
    fail_triggered=1
fi

if [[ ${warn_triggered} -eq 1 ]]; then
    echo "::warning title=Formatter benchmark overhead::Fixed ratio ${fixed_ratio}, nested ratio ${nested_ratio} exceeded warning threshold ${warn_ratio}."
fi

if [[ ${fail_triggered} -eq 1 ]]; then
    echo "::error title=Formatter benchmark regression::Fixed ratio ${fixed_ratio}, nested ratio ${nested_ratio} exceeded failure threshold ${fail_ratio}."
    exit 1
fi
