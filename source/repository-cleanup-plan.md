# Repository Cleanup Strategy

The workspace is intentionally dirty while implementation and validation proceed. This checklist keeps cleanup safe and reviewable without losing in-progress work.

## 1. Snapshot before cleanup

```bash
git status --short
git diff --stat
git diff > /tmp/kmx-unit-working.patch
```

## 2. Group changes by concern

Stage and commit in narrow slices so review remains mechanical:

- formatter and benchmark performance
- compile-time probe tooling
- CI and build reliability updates
- quantity-kind extensibility and tests
- documentation and policy updates

Example staging flow:

```bash
git add source/inc/kmx/unit/text.hpp test/src/kmx/unit/benchmark.cpp
git commit -m "Optimize width formatting path and add formatter benchmarks"
```

## 3. Keep unrelated local edits out of release commits

If unrelated local edits exist, park them before preparing release commits:

```bash
git stash push --keep-index --include-untracked -m "local unrelated work"
```

Then finish and verify release commits, and restore local work:

```bash
git stash pop
```

## 4. Verification gates before final push

Run these gates from a clean build directory:

```bash
profile="$(bash tools/qbs/select_profile.sh)"
qbs resolve -f kmx-unit.qbs --build-directory output/qbs profile:"${profile}" config:release
qbs build -f kmx-unit.qbs --build-directory output/qbs profile:"${profile}" config:release
qbs run -f kmx-unit.qbs --build-directory output/qbs profile:"${profile}" config:release --products unit-test
```

Run the formatter guard against the repo-local QBS build directory or the resolved executable path:

```bash
CXX=g++ bash tools/bench/guard_formatting.sh output/qbs
CXX=g++ bash tools/bench/guard_formatting.sh "$(find output/qbs -type f -name unit-test | head -n 1)"
```

Optional compatibility gates:

```bash
CXX=g++-14 bash tools/compile_time/measure.sh 5 output/compile_time/report.csv
profile="$(bash tools/qbs/select_profile.sh)" && qbs build -f kmx-unit.qbs --build-directory output/qbs profile:"${profile}"
```

## 5. Final history hygiene

- Keep commit subjects behavior-focused and under one concern each.
- Avoid force-push and history rewrite on shared branches.
- Prefer additive follow-up commits over amending once review has started.
