#include <pebble.h>
#include "history_results.h"
#include "randgen.h"


static MenuLayer *g_menu_layer = NULL;

static int g_current_rows_num = 10;
static char buff[20];

static const char* history_title_by_index_get(int idx)
{
    snprintf(buff, sizeof(buff), "Title entry %d", g_current_rows_num);
    return buff;
}

static const char* history_subtitle_by_index_get(int idx)
{
    return "Subtitle entry";
}

static uint16_t history_results_num_sections_get_cb(MenuLayer *menu_layer, void *data) 
{
    return 1;
}

static uint16_t history_results_num_rows_get_cb(MenuLayer *menu_layer, uint16_t section_index, void *data)
{
    return g_current_rows_num;
}

static int16_t history_results_header_height_get_cb(MenuLayer *menu_layer, uint16_t section_index, void *data)
{
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void history_results_header_draw_cb(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data)
{
    switch (section_index)
    {
        case 0:
            menu_cell_basic_header_draw(ctx, cell_layer, "Results history");
            break;
    }
}

static void history_results_row_draw_cb(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data)
{
    switch (cell_index->section)
    {
        case 0:
            menu_cell_basic_draw(ctx, cell_layer, history_title_by_index_get(cell_index->row), history_subtitle_by_index_get(cell_index->row), NULL);
        break;
    }
}

static void history_results_select_click_cb(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{
}

static void history_results_window_load(Window *window)
{
    // Now we prepare to initialize the menu layer
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_frame(window_layer);

    // Create the menu layer
    g_menu_layer = menu_layer_create(bounds);
    menu_layer_set_callbacks(g_menu_layer, NULL, (MenuLayerCallbacks){
        .get_num_sections = history_results_num_sections_get_cb,
        .get_num_rows = history_results_num_rows_get_cb,
        .get_header_height = PBL_IF_RECT_ELSE(history_results_header_height_get_cb, NULL),
        .draw_header = PBL_IF_RECT_ELSE(history_results_header_draw_cb, NULL),
        .draw_row = history_results_row_draw_cb,
        .select_click = history_results_select_click_cb,
    });

    // Bind the menu layer's click config provider to the window for interactivity
    menu_layer_set_click_config_onto_window(g_menu_layer, window);

    layer_add_child(window_layer, menu_layer_get_layer(g_menu_layer));
    
    /* initialize random seed */
    randgen_seed(time_ms(NULL, NULL));
}

static void history_results_window_unload(Window *window)
{
    // Destroy the menu layer
    menu_layer_destroy(g_menu_layer);
    g_menu_layer = NULL;
}

static void history_results_window_appear(Window *window)
{
    /* Initialize history data */
    g_current_rows_num = randgen(20);
    menu_layer_reload_data(g_menu_layer);
}

static void history_results_window_disappear(Window *window)
{
}

void history_results_window_init()
{
    static Window *history_results_window = NULL;
    if (!history_results_window)
    {
        history_results_window = window_create();
        window_set_window_handlers(history_results_window, (WindowHandlers) {
            .load = history_results_window_load,
            .unload = history_results_window_unload,
            .appear = history_results_window_appear,
            .disappear = history_results_window_disappear,
        });
    }
    window_stack_push(history_results_window, true);
}




