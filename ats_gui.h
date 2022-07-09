
typedef struct gui_input {
    v2 cursor_position;

    u32 up : 1;
    u32 down : 1;
    u32 pressed : 1;
} gui_input;

typedef struct gui_layout {
    // setup:
    v2 location;

    f32 font_size;
    f32 line_spacing;

    // internal:
    b8 has_focus;
    i32 index;
    v2 pos;
    i32 item_count;

    gui_input input;
} gui_layout;

static void
gui_update_input(gui_input* input) {
    memset(input, 0, sizeof *input);
    input->cursor_position = platform.mouse.pos;

    if (platform.mouse.pressed[MOUSE_BUTTON_LEFT] || platform.keyboard.pressed[KEY_ENTER] || platform.keyboard.pressed[KEY_SPACE]) {
        input->pressed = true;
    }

    if (platform.keyboard.repeat[KEY_UP] || platform.mouse.scroll.y > 0) {
        input->up = true;
    }

    if (platform.keyboard.repeat[KEY_DOWN] || platform.mouse.scroll.y < 0) {
        input->down = true;
    }
}

static void
gui_begin(gui_layout* layout) {
    layout->has_focus = false;
    layout->pos = layout->location;
    layout->item_count = 0;

    gui_update_input(&layout->input);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m4_ortho(0, platform.width, platform.height, 0, -1, 1).e);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

static void
gui_end(gui_layout* layout) {
    if (layout->input.up) layout->index--;
    if (layout->input.down) layout->index++;

    layout->index = clamp(layout->index, 0, layout->item_count - 1);
}

static b32
gui_do_button(gui_layout* layout, const char* text) {
    i32 len = strlen(text);
    r2 rect = {
        .min = layout->pos,
        .max = v2(layout->pos.x + layout->font_size * len, layout->pos.y + layout->font_size),
    };

    if (r2_contains(rect, layout->input.cursor_position)) {
        layout->index = layout->item_count;
        layout->has_focus = true;
    }

    b32 clicked = (layout->index == layout->item_count) && layout->input.pressed;
    gl_string_format(layout->pos.x, layout->pos.y, 0, layout->font_size, layout->font_size, layout->index == layout->item_count? 0xffffffff : 0xff777777, text);

    layout->pos.y += layout->font_size + layout->line_spacing;
    layout->item_count++;
    return clicked;
}

