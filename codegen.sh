#!/bin/bash
set -uo pipefail

read -r -d '' builtin_terms <<'EOD'
    xterm
    linux
    screen        tmux
    rxvt-256color
    rxvt-unicode  rxvt
    Eterm
EOD

read -r -d '' terminfo_keys <<'EOD'
    kf1   F1
    kf2   F2
    kf3   F3
    kf4   F4
    kf5   F5
    kf6   F6
    kf7   F7
    kf8   F8
    kf9   F9
    kf10  F10
    kf11  F11
    kf12  F12
    kich1 INSERT
    kdch1 DELETE
    khome HOME
    kend  END
    kpp   PGUP
    knp   PGDN
    kcuu1 ARROW_UP
    kcud1 ARROW_DOWN
    kcub1 ARROW_LEFT
    kcuf1 ARROW_RIGHT
    kcbt  BACK_TAB
EOD

read -r -d '' terminfo_funcs <<'EOD'
    smcup ENTER_CA
    rmcup EXIT_CA
    cnorm SHOW_CURSOR
    civis HIDE_CURSOR
    clear CLEAR_SCREEN
    sgr0  SGR0
    smul  UNDERLINE
    bold  BOLD
    blink BLINK
    sitm  ITALIC
    rev   REVERSE
    smkx  ENTER_KEYPAD
    rmkx  EXIT_KEYPAD
    dim   DIM
    invis INVISIBLE
EOD

read -r -d '' extra_keys <<'EOD'
    MOUSE_LEFT
    MOUSE_RIGHT
    MOUSE_MIDDLE
    MOUSE_RELEASE
    MOUSE_WHEEL_UP
    MOUSE_WHEEL_DOWN
EOD

main() {
    local IFS=$'\n'
    local codegen_type=$1

    # codegen terminfo_cap_indexes
    # codegen #define TB_CAP_*
    # codegen #define TB_KEY_*
    c_cap_indexes='static const int16_t terminfo_cap_indexes[] = {'$'\n'
    c_cap_defines=''
    c_cap_num=0
    c_key_defines=''
    c_key_num=0
    for terminfo_cap_tuple in $terminfo_keys; do
        string_name=$(awk '{print $1}' <<<"$terminfo_cap_tuple")
        define_name=$(awk '{print $2}' <<<"$terminfo_cap_tuple")
        string_index=$(terminfo_string_index $string_name)
        c_cap_indexes+="    $string_index, // $string_name (TB_CAP_${define_name})"$'\n'
        c_cap_defines+="#define TB_CAP_${define_name} $c_cap_num"$'\n'
        c_key_defines+="#define TB_KEY_${define_name} (0xffff - $c_key_num)"$'\n'
        let c_cap_num+=1
        let c_key_num+=1
    done
    c_cap_defines+="#define TB_CAP__COUNT_KEYS $c_cap_num"$'\n'
    for extra_keys_tuple in $extra_keys; do
        define_name=$(awk '{print $1}' <<<"$extra_keys_tuple")
        c_key_defines+="#define TB_KEY_${define_name} (0xffff - $c_key_num)"$'\n'
        let c_key_num+=1
    done
    for terminfo_cap_tuple in $terminfo_funcs; do
        string_name=$(awk '{print $1}' <<<"$terminfo_cap_tuple")
        define_name=$(awk '{print $2}' <<<"$terminfo_cap_tuple")
        string_index=$(terminfo_string_index $string_name)
        c_cap_indexes+="    $string_index, // $string_name (TB_CAP_${define_name})"$'\n'
        c_cap_defines+="#define TB_CAP_${define_name} $c_cap_num"$'\n'
        let c_cap_num+=1
    done
    c_cap_defines+="#define TB_CAP__COUNT $c_cap_num"$'\n'
    c_cap_indexes+='};'

    c_codegen_comment="/* Produced by $0 on $(date -uR) */"

    if [ "$codegen_type" == "c" ]; then
        echo "$c_codegen_comment"
        echo
        echo "$c_cap_indexes"
        echo

        # codegen built-in _keys and _funcs as _caps
        c_term_name_cap='static struct {'$'\n'
        c_term_name_cap+='    const char *name;'$'\n'
        c_term_name_cap+='    const char **caps;'$'\n'
        c_term_name_cap+='    const char *alias;'$'\n'
        c_term_name_cap+='} builtin_terms[] = {'$'\n'
        for builtin_terms_tuple in $builtin_terms; do
            term_name=$(awk '{print $1}' <<<"$builtin_terms_tuple")
            term_alias=$(awk '{print $2}' <<<"$builtin_terms_tuple")
            c_term_name=$(tr -d '\n' <<<$term_name | tr -c 'A-Za-z0-9' '_' | tr 'A-Z' 'a-z')

            c_term_caps="static const char *${c_term_name}_caps[] = {"$'\n'
            for terminfo_cap_tuple in $terminfo_keys $terminfo_funcs; do
                string_name=$(awk '{print $1}' <<<"$terminfo_cap_tuple")
                define_name=$(awk '{print $2}' <<<"$terminfo_cap_tuple")
                c_string_literal=$(terminfo_string_literal $term_name $string_name)
                c_term_caps+="    ${c_string_literal}, // $string_name (TB_CAP_${define_name})"$'\n'
            done
            c_term_caps+='};'

            echo "// $term_name"
            echo "$c_term_caps"
            echo

            c_term_name_cap+="    { \"${term_name}\", ${c_term_name}_caps, \"${term_alias}\" },"$'\n'
        done
        c_term_name_cap+='    { NULL, NULL, NULL },'$'\n'
        c_term_name_cap+='};'$'\n'

        echo "$c_term_name_cap"

    elif [ "$codegen_type" == "h" ]; then

        echo "$c_codegen_comment"
        echo -n "$c_key_defines"
        echo
        echo -n "$c_cap_defines"

    fi
}

terminfo_string_index() {
    local string_name=$1
    infocmp -E | grep -w $string_name | awk '{print $2}' | sed 's|:$||g'
}

terminfo_string_literal() {
    local term_name=$1
    local string_name=$2
    local var_name=$(infocmp -E $term_name | grep -w $string_name | awk '{print $5}' | sed 's|,$||g')
    if [ "$var_name" == "ABSENT_STRING" ]; then
        echo '""'
        return 0
    fi
    local c_string_literal=$(infocmp -E $term_name | grep -w "${var_name}" | grep ' = ' | awk '{print $NF}' | sed 's|;$||g')
    if [ -z "$c_string_literal" ]; then
        echo '""'
        return 0
    fi
    echo $c_string_literal
}

main "$@"
