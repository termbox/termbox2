// cc -DTB_IMPL -I.. readline.c -lreadline -ltinfo

#include <termbox2.h>
#include <readline/readline.h>
#include <readline/history.h>

static void render(void);
static void handle_readline(char *line);
static int getc_tee(FILE *in);
static void handle_completion(char **matches, int len, int max);
static int handle_signal(void);
static void noop1(void);
static void noop2(int ig);

static int done = 0;
static char entry[1024] = {0};
static char completion[1024] = {0};
static int teefd[2];
static struct tb_event last_ev = {0};
static int ev_count = 0;

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    // Init termbox.
    // Read from read-end of teefd pipe. Write to tty.
    // We will tee input from readline into termbox.
    int ttyfd = open("/dev/tty", O_RDWR);
    pipe(teefd);
    fcntl(teefd[0], F_SETFL, O_NONBLOCK);
    fcntl(teefd[1], F_SETFL, O_NONBLOCK);
    tb_init_rwfd(teefd[0], ttyfd);
    tb_set_input_mode(TB_INPUT_ALT);

    // Disable readline output as termbox will handle that.
    rl_redisplay_function = noop1;
    rl_prep_term_function = noop2;
    rl_deprep_term_function = noop1;
    rl_completion_display_matches_hook = handle_completion;

    // Hook readline input.
    // Here's where we tee input via a pipe.
    rl_getc_function = getc_tee;

    // Disable readline signal handlers. Otherwise, e.g., SIGINT exits the
    // program. Leave rl_catch_sigwinch on so it can catch resize events.
    rl_catch_signals = 0;

    // Set a signal hook so we can re-render on resize.
    //
    // Note termbox will not see resize events until we call tb_peek_event in
    // getc_tee, so use rl_get_screen_size instead of tb_width / tb_height for
    // layout if needed.
    //
    // You might think we should call tb_peek_event in handle_signal to catch
    // resize events. However if we also catch a key event, then we'd be out of
    // sync with readline's input. Hypothetically this could be handled better
    // if termbox had a version of tb_peek_event that allowed the caller to
    // look specifically for resize events. Too niche probably.
    rl_signal_event_hook = handle_signal;

    // Install readline callback.
    rl_callback_handler_install(NULL, handle_readline);

    while (!done) {
        render();
        rl_callback_read_char();
    }

    rl_callback_handler_remove();
    tb_shutdown();

    close(teefd[0]);
    close(teefd[1]);
    close(ttyfd);

    return 0;
}

static void render(void) {
    int rows, cols;
    int y = 0;

    tb_clear();

    tb_printf(0, y++, TB_BOLD, 0, ">%.*s", rl_end, rl_line_buffer);
    tb_set_cursor(rl_point + 1, 0);

    tb_print(0, y++, 0, 0, *entry ? entry : "-");

    y++;
    rl_get_screen_size(&rows, &cols);
    tb_printf(0, y++, 0, 0, "readline screen: %dx%d", cols, rows);
    tb_printf(0, y++, 0, 0, " termbox screen: %dx%d", tb_width(), tb_height());
    // readline cols seems to be off by 1...

    y++;
    tb_printf(0, y++, 0, 0, "termbox event %d {", ev_count);
    tb_printf(0, y++, 0, 0, "  type = %u", last_ev.type);
    tb_printf(0, y++, 0, 0, "  mod = %u", last_ev.mod);
    tb_printf(0, y++, 0, 0, "  key = %u", last_ev.key);
    tb_printf(0, y++, 0, 0, "  ch = %u", last_ev.ch);;
    tb_print(0,  y++, 0, 0, "}");

    y++;
    tb_print(0, y++, 0, 0, "completion matches:");
    tb_print(0, y++, 0, 0, completion);

    tb_present();
}

static void handle_readline(char *line) {
    static int count = 0;

    if (!line) {
        return;
    }

    if (strlen(line) > 0) {
        add_history(line);
        snprintf(entry, sizeof(entry), "%d: %s", count++, line);
        if (*line == 'q') done = 1;
    }

    free(line);
}

static int getc_tee(FILE *in) {
    struct tb_event ev;
    int c = rl_getc(in);

    // Copy input to termbox via pipe
    unsigned char uc = (unsigned char)c;
    write(teefd[1], &uc, sizeof(uc));

    while (tb_peek_event(&ev, 0) == TB_OK) {
        ev_count++;
        memcpy(&last_ev, &ev, sizeof(ev));
    }

    return c;
}

static void handle_completion(char **matches, int len, int max) {
    char *c = completion;
    size_t clen = sizeof(completion);
    int i;
    (void)max;

    for (i = 0; i < len && clen > 0; i++) {
        size_t mlen = snprintf(c, clen, "  %s\n", matches[i]);
        c += mlen;
        clen -= mlen;
    }
}

static int handle_signal(void) {
    render();
    return 0;
}

static void noop1(void) {
}

static void noop2(int ig) {
    (void)ig;
}
