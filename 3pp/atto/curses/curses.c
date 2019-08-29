/*
 * Copyright (c) 1981, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "simba.h"

#include "curses.h"

#ifndef CONFIG_EMACS_COLUMNS_MAX
#    define CONFIG_EMACS_COLUMNS_MAX                       80
#endif

#ifndef CONFIG_EMACS_ROWS_MAX
#    define CONFIG_EMACS_ROWS_MAX                          24
#endif

struct line_t {
    unsigned int flags;
    char text[CONFIG_EMACS_COLUMNS_MAX + 1];
};

static WINDOW window;
static void *input_p;
static void *output_p;

WINDOW *curscr = NULL;  /* Current screen. */
WINDOW *stdscr = NULL;  /* Standard screen. */
int COLS = CONFIG_EMACS_COLUMNS_MAX;
int LINES = CONFIG_EMACS_ROWS_MAX;

static struct line_t lines[CONFIG_EMACS_ROWS_MAX];

char *__unctrl[256] = {
        "^@",  "^A",  "^B",  "^C",  "^D",  "^E",  "^F",  "^G",
        "^H",  "^I",  "^J",  "^K",  "^L",  "^M",  "^N",  "^O",
        "^P",  "^Q",  "^R",  "^S",  "^T",  "^U",  "^V",  "^W",
        "^X",  "^Y",  "^Z",  "^[", "^\\",  "^]",  "^~",  "^_",
         " ",   "!",  "\"",   "#",   "$",   "%",   "&",   "'",
         "(",   ")",   "*",   "+",   ",",   "-",   ".",   "/",
         "0",   "1",   "2",   "3",   "4",   "5",   "6",   "7",
         "8",   "9",   ":",   ";",   "<",   "=",   ">",   "?",
         "@",   "A",   "B",   "C",   "D",   "E",   "F",   "G",
         "H",   "I",   "J",   "K",   "L",   "M",   "N",   "O",
         "P",   "Q",   "R",   "S",   "T",   "U",   "V",   "W",
         "X",   "Y",   "Z",   "[",  "\\",   "]",   "^",   "_",
         "`",   "a",   "b",   "c",   "d",   "e",   "f",   "g",
         "h",   "i",   "j",   "k",   "l",   "m",   "n",   "o",
         "p",   "q",   "r",   "s",   "t",   "u",   "v",   "w",
         "x",   "y",   "z",   "{",   "|",   "}",   "~",   "^?",

        "0x80", "0x81",	"0x82", "0x83", "0x84", "0x85", "0x86", "0x87",
        "0x88", "0x89",	"0x8a", "0x8b", "0x8c", "0x8d", "0x8e", "0x8f",
        "0x90", "0x91",	"0x92", "0x93", "0x94", "0x95", "0x96", "0x97",
        "0x98", "0x99",	"0x9a", "0x9b", "0x9c", "0x9d", "0x9e", "0x9f",
        "0xa0", "0xa1",	"0xa2", "0xa3", "0xa4", "0xa5", "0xa6", "0xa7",
        "0xa8", "0xa9",	"0xaa", "0xab", "0xac", "0xad", "0xae", "0xaf",
        "0xb0", "0xb1",	"0xb2", "0xb3", "0xb4", "0xb5", "0xb6", "0xb7",
        "0xb8", "0xb9",	"0xba", "0xbb", "0xbc", "0xbd", "0xbe", "0xbf",
        "0xc0", "0xc1",	"0xc2", "0xc3", "0xc4", "0xc5", "0xc6", "0xc7",
        "0xc8", "0xc9",	"0xca", "0xcb", "0xcc", "0xcd", "0xce", "0xcf",
        "0xd0", "0xd1",	"0xd2", "0xd3", "0xd4", "0xd5", "0xd6", "0xd7",
        "0xd8", "0xd9",	"0xda", "0xdb", "0xdc", "0xdd", "0xde", "0xdf",
        "0xe0", "0xe1",	"0xe2", "0xe3", "0xe4", "0xe5", "0xe6", "0xe7",
        "0xe8", "0xe9",	"0xea", "0xeb", "0xec", "0xed", "0xee", "0xef",
        "0xf0", "0xf1",	"0xf2", "0xf3", "0xf4", "0xf5", "0xf6", "0xf7",
        "0xf8", "0xf9",	"0xfa", "0xfb", "0xfc", "0xfd", "0xfe", "0xff",
};

/* static void vt100_screen_clear() */
/* { */
/*     std_fprintf(output_p, FSTR("\x1b[2J")); */
/* } */

static void vt100_cursor_save(void)
{
    std_fprintf(output_p, FSTR("\x1b""7"));
}

static void vt100_cursor_restore(void)
{
    std_fprintf(output_p, FSTR("\x1b""8"));
}

/* static void vt100_cursor_move(int y, int x) */
/* { */
/*     std_fprintf(output_p, FSTR("\x1b[%d;%dH"), y, x); */
/* } */

static void vt100_cursor_move_up(int rows)
{
    std_fprintf(output_p, FSTR("\x1b[%dA"), rows);
}

static void vt100_cursor_move_down(int rows)
{
    std_fprintf(output_p, FSTR("\x1b[%dB"), rows);
}

static void vt100_cursor_move_right(int columns)
{
    std_fprintf(output_p, FSTR("\x1b[%dC"), columns);
}

static void vt100_cursor_make_visible()
{
    std_fprintf(output_p, FSTR("\x1b[?25h"));
}

static void vt100_cursor_make_invisible()
{
    std_fprintf(output_p, FSTR("\x1b[?25l"));
}

static void vt100_clear_to_eol(void)
{
    std_fprintf(output_p, FSTR("\x1b[K"));
}

static void cursor_move(int y, int x)
{
    vt100_cursor_restore();

    if (y > 0) {
        vt100_cursor_move_down(y);
    }

    if (x > 0) {
        vt100_cursor_move_right(x);
    }
}

/* static void vt100_display_reversed(void) */
/* { */
/*     std_fprintf(output_p, FSTR("\x1b[7m")); */
/* } */

/* static void vt100_display_reset_all_attributes(void) */
/* { */
/*     std_fprintf(output_p, FSTR("\x1b[0m")); */
/* } */

/**
 * Initialize the window.
 */
WINDOW *initscr()
{
    int i;

    window.begx = 0;
    window.begy = 0;
    window.maxx = COLS;
    window.maxy = LINES;
    window.curx = 0;
    window.cury = 0;
    window.flags = 0;

    curscr = stdscr = &window;

    for (i = 0; i < membersof(lines); i++) {
        lines[i].flags = 0;
        lines[i].text[0] = '\0';
    }

    /* Setup the screen. */
    std_fprintf(output_p, FSTR("\r"));
    vt100_clear_to_eol();

    for (i = 0; i < CONFIG_EMACS_ROWS_MAX - 1; i++) {
        std_fprintf(output_p, FSTR("\r\n"));
        vt100_clear_to_eol();
    }

    vt100_cursor_move_up(CONFIG_EMACS_ROWS_MAX - 1);
    vt100_cursor_save();

    return (&window);
}

/**
 * Output the current window, including the cursor.
 */
int wrefresh(WINDOW *win_p)
{
    int i;
    size_t len;

    /* Less flickering with cursor invisible. */
    vt100_cursor_make_invisible();

    for (i = 0; i < membersof(lines); i++) {
        /* Only print dirty lines to same bandwith. */
        if ((lines[i].flags & __ISDIRTY) == 0) {
            continue;
        }

        lines[i].flags = 0;
        len = strlen(&lines[i].text[0]);

        cursor_move(i, 0);

        if (len > 0) {
            chan_write(output_p, &lines[i].text[0], len);
        }

        vt100_clear_to_eol();
    }

    cursor_move(win_p->cury, win_p->curx);
    vt100_cursor_make_visible();

    return (OK);
}

/**
 * Move the cursor.
 */
int wmove(WINDOW *win_p, int y, int x)
{
    if ((x < 0) || (y < 0)) {
        return (ERR);
    }

    if ((x >= win_p->maxx) || (y >= win_p->maxy)) {
        return (ERR);
    }

    win_p->cury = y;
    win_p->curx = x;

    return (OK);
}

int wclrtoeol(WINDOW *win_p)
{
    if (lines[win_p->cury].text[win_p->curx] != '\0') {
        lines[win_p->cury].text[win_p->curx] = '\0';
        lines[win_p->cury].flags |= __ISDIRTY;
    }

    return (OK);
}

int wclear(WINDOW *win_p)
{
    return (OK);
}

/**
 * Turn on best highlighting mode of the terminal.
 */
int wstandout(WINDOW *win_p)
{
    return (OK);
}

/**
 * Turn off best highlighting mode of the terminal.
 */
int wstandend(WINDOW *win_p)
{
    return (OK);
}

/**
 * Add a single character at current cursor position.
 */
int waddch(WINDOW *win_p, int ch)
{
    char c;

    c = ch;

    return (__waddbytes(win_p, &c, 1, 0));
}

/**
 * Add one or more character(s) at current cursor position.
 */
int __waddbytes(WINDOW *win_p,
                const char *bytes_p,
                int count,
                int so)
{
    int i;
    char ch;

    for (i = 0; i < count; i++) {
        ch = bytes_p[i];

        if (win_p->curx >= CONFIG_EMACS_COLUMNS_MAX) {
            return (ERR);
        }

        switch (ch) {

        case '\r':
            win_p->curx = 0;
            break;

        case '\n':
            lines[win_p->cury].text[win_p->curx] = '\0';
            lines[win_p->cury].flags |= __ISDIRTY;
            win_p->cury++;
            win_p->curx = 0;
            break;

        default:
            if (lines[win_p->cury].text[win_p->curx] != ch) {
                lines[win_p->cury].text[win_p->curx] = ch;
                lines[win_p->cury].text[win_p->curx + 1] = '\0';
                lines[win_p->cury].flags |= __ISDIRTY;
            }

            win_p->curx++;
            break;
        }
    }

    return (OK);
}

/**
 * Get a character from the input channel.
 */
int wgetch(WINDOW *win_p)
{
    char c;

    chan_read(input_p, &c, 1);

    return (c);
}

void idlok(WINDOW *win_p, int bf)
{
}

int endwin()
{
    int i;

    /* Clear the emacs screen. */
    vt100_cursor_restore();

    for (i = 0; i < CONFIG_EMACS_ROWS_MAX; i++) {
        vt100_clear_to_eol();

        if (i < CONFIG_EMACS_ROWS_MAX - 1) {
            std_fprintf(output_p, FSTR("\r\n"));
        }
    }

    vt100_cursor_restore();

    return (OK);
}

int raw()
{
    return (OK);
}

int noraw()
{
    return (OK);
}

int noecho()
{
    return (OK);
}

void flushinp()
{
}

void atto_curses_set_input_channel(void *chin_p)
{
    input_p = chin_p;
}

void atto_curses_set_output_channel(void *chout_p)
{
    output_p = chout_p;
}
