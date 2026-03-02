# assemble.awk - Assembles termbox2.h from template and source files
#
# Usage: awk -f assemble.awk termbox2.h.in > termbox2.h
#
# Processes the template by:
# 1. Inlining #include "foo.h" (stripping include guards)
# 2. Injecting .c file contents at BEGIN/END markers
#    (stripping #include "..." and #define TB_IMPL from .c files)

function extract_include(line,    i, j) {
    i = index(line, "\"")
    if (i == 0) return ""
    j = index(substr(line, i + 1), "\"")
    if (j == 0) return ""
    return substr(line, i + 1, j - 1)
}

# Inject a .c file, stripping the preamble (everything up to and including
# the last #include "..." line). System #include <...> lines after the
# preamble are kept.
function strip_inject(file,    n, i, last_local, line, lines) {
    n = 0
    while ((getline line < file) > 0) {
        lines[++n] = line
    }
    close(file)

    # Find the last #include "..." line
    last_local = 0
    for (i = 1; i <= n; i++) {
        if (lines[i] ~ /^#include "/) last_local = i
    }

    # Print everything after the preamble
    for (i = last_local + 1; i <= n; i++) {
        print lines[i]
    }
}

# Read a header file, strip its include guard, inline nested #include "..."
# Uses a unique prefix per call depth to avoid array clobbering
function inline_header(file,    n, i, start, end, last, line, f, lines) {
    n = 0
    while ((getline line < file) > 0) {
        n++
        lines[file, n] = line
    }
    close(file)

    start = 1
    end = n

    # Detect and strip include guard
    if (n >= 2 && lines[file, 1] ~ /^#ifndef [A-Z_]+_H/ && lines[file, 2] ~ /^#define [A-Z_]+_H/) {
        start = 3
        for (last = n; last >= 1; last--) {
            if (lines[file, last] ~ /^#endif/) {
                end = last - 1
                break
            }
        }
    }

    for (i = start; i <= end; i++) {
        line = lines[file, i]
        if (line ~ /^#include "/) {
            f = extract_include(line)
            if (f != "") {
                inline_header(f)
                continue
            }
        }
        print line
    }
}

{
    # Inline headers at BEGIN/END header markers
    if ($0 ~ /\/\* BEGIN header .+ \*\//) {
        print
        f = $0
        sub(/.*BEGIN header /, "", f)
        sub(/ \*\/.*/, "", f)
        inline_header(f)
        next
    }
    if ($0 ~ /\/\* END header .+ \*\//) {
        print
        next
    }

    # Inject .c files at markers
    if ($0 ~ /\/\* BEGIN core \*\//) {
        print
        strip_inject("termbox2.c")
        next
    }
    if ($0 ~ /\/\* END core \*\//) {
        print
        next
    }
    if ($0 ~ /\/\* BEGIN platform posix \*\//) {
        print
        strip_inject("tb_posix.c")
        next
    }
    if ($0 ~ /\/\* END platform posix \*\//) {
        print
        next
    }
    if ($0 ~ /\/\* BEGIN platform wasm \*\//) {
        print
        strip_inject("tb_wasm.c")
        next
    }
    if ($0 ~ /\/\* END platform wasm \*\//) {
        print
        next
    }

    print
}
