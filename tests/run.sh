#!/bin/bash
set -uo pipefail

main() {
    local timeout_s=5
    local x_display=':1000'
    local xvfb_pipe='/tmp/.X11-unix/X1000'
    local this_dir="$(cd $(dirname "${BASH_SOURCE[0]}") &>/dev/null && pwd)/"
    local xterm_geom='80x24+0+0'
    local xterm_bg='grey3'
    local xterm_fg='grey93'
    local single_test_name=${TERMBOX2_TEST_NAME:-}
    local main_ec=0

    check_deps

    # loop through each 'test_*' dir
    for test_php in $(find . -type f -wholename '*/test_*/test.php' | sort); do
        local test_dir=$(dirname $test_php)
        local test_name=$(basename $test_dir)

        # check if we are running a specific test (TERMBOX2_TEST_NAME)
        if [ -n "$single_test_name" -a "$single_test_name" != "$test_name" ]; then
            continue
        fi

        # begin
        echo -e "\x1b[1m$test_name\x1b[0m: BEGIN"

        # make log file
        local test_log_cmd=$(mktemp '/tmp/tb_test.XXXXXXXXXX')
        local test_log_xterm="${test_log_cmd}.xterm"
        local test_log_php="${test_log_cmd}.php"

        # run Xvfb (headless X server)
        echo -n '  waiting for Xvfb slot...'
        while test -e $xvfb_pipe; do echo -n .; sleep 0.1; done
        echo
        Xvfb -screen 0 800x600x24 $x_display &>/dev/null &
        local xvfb_pid=$!

        # wait for Xvfb to come up
        echo -n '  starting Xvfb...'
        local max_wait=30
        while ! test -e $xvfb_pipe; do
            echo -n .
            max_wait=$((max_wait-1))
            [ "$max_wait" -le 0 ] && break
            sleep 1
        done
        echo

        # run test_bin in xterm in Xvfb
        echo '  running test in xterm'
        xterm -display $x_display \
            -u8 -geometry $xterm_geom -bg $xterm_bg -fg $xterm_fg \
            -xrm 'xterm*metaSendsEscape:true' \
            -xrm 'xterm*translations:#override\nShift <Key>Home:print-immediate()' \
            -xrm 'xterm*printOptsImmediate:1' \
            -xrm 'xterm*printModeImmediate:2' \
            -xrm "xterm*printFileImmediate:$test_log_xterm" \
            -e "php -d auto_prepend_file=$this_dir/test_prepend.php $test_php $test_log_cmd &>$test_log_php" \
            &>/dev/null &
        local xterm_pid=$!

        # tail test_log_cmd until we see 'screencap'
        local test_log_cursor=0
        local test_log_size=0
        local test_end_ts=$(($(date +%s) + $timeout_s))
        local test_skipped=0
        while true; do
            test_log_size=$(stat -c %s $test_log_cmd 2>/dev/null)
            [ -z "$test_log_size" ] && break # stat failed or deleted
            [ "$test_log_size" -lt "$test_log_cursor" ] && break # truncated
            local test_log_content=$(tail -c "+$test_log_cursor" $test_log_cmd | \
                head -c "$((test_log_size-test_log_cursor))")
            test_log_cursor=$test_log_size
            echo -n "$test_log_content"
            grep -q 'skip'      <<<"$test_log_content" && test_skipped=1 && break
            grep -q 'screencap' <<<"$test_log_content" && break
            sleep 0.1
            if [ "$(date +%s)" -ge "$test_end_ts" ]; then
                echo -e '\n  timeout'
                break
            fi
        done
        echo

        if [ "$test_skipped" -ne 1 ]; then
            # take screencap
            # xwd -root -display $x_display -out $test_dir/observed.xwd # graphical
            rm -f $test_dir/observed.*
            DISPLAY=$x_display xvkbd -window xterm -text '\S\[Home]' &>/dev/null # ansi
            local test_log_xterm_count=$(ls -1 ${test_log_xterm}* 2>/dev/null | wc -l)
            [ "$test_log_xterm_count" -eq 1 ] && cp ${test_log_xterm}* $test_dir/observed.ansi

            # diff screencap
            # convert $test_dir/expected.xwd $test_dir/observed.gif
            diff $test_dir/expected.ansi $test_dir/observed.ansi &>/dev/null
            diff_ec=$?
        fi

        # print result
        if [ "$test_skipped" -eq 1 ]; then
            echo -e "\x1b[1m$test_name\x1b[0m: \x1b[33mSKIP\x1b[0m"
        elif [ "$diff_ec" -eq 0 ]; then
            echo -e "\x1b[1m$test_name\x1b[0m: \x1b[32mOK\x1b[0m"
        else
            echo
            echo -e '  \x1b[31mdiff!\x1b[0m'
            # compare $test_dir/expected.gif $test_dir/observed.gif $test_dir/diff.gif
            # xz -c $test_dir/diff.gif | base64

            echo -e '\n  [xterm expected]'
            cat $test_dir/expected.ansi 2>/dev/null | sed 's/^/  /'; echo -e '\x1b[0m'

            echo -e '\n  [xterm observed]'
            cat $test_dir/observed.ansi 2>/dev/null | sed 's/^/  /'; echo -e '\x1b[0m'

            echo -e '\n  [xterm observed.ansi.b64]'
            base64 $test_dir/observed.ansi 2>/dev/null | sed 's/^/  /'

            if [ -s "$test_log_php" ]; then
                echo -e '\n  [php log]'
                cat $test_log_php | sed 's/^/  /'
            fi

            echo -e "\x1b[1m$test_name\x1b[0m: \x1b[31mERR\x1b[0m"
            main_ec=1
        fi
        echo

        # clean up
        kill $xterm_pid &>/dev/null
        kill $xvfb_pid &>/dev/null
        rm -f $test_log_cmd $test_log_xterm $test_log_php
    done

    return $main_ec
}

check_deps() {
    for dep in Xvfb xterm xvkbd; do
        if ! command -v $dep &>/dev/null; then
            echo "Missing dependency: $dep"
            exit 1
        fi
    done
}

main
