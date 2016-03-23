#include <pebble.h>
#include "main_menu.h"
#include "history_results.h"

#define MAIN_MENU_MANUAL_ENTRY_IDX        0
#define MAIN_MENU_RESULTS_HISTORY_IDX     1
#define MAIN_MENU_ALERTS_IDX              2
#define MAIN_MENU_SETTINGS_IDX            3

static MenuLayer *g_menu_layer = NULL;

static uint16_t main_menu_num_sections_get_cb(MenuLayer *menu_layer, void *data) 
{
    return 1;
}

static uint16_t main_menu_num_rows_get_cb(MenuLayer *menu_layer, uint16_t section_index, void *data)
{
    return 4;
}

static int16_t main_menu_header_height_get_cb(MenuLayer *menu_layer, uint16_t section_index, void *data)
{
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void main_menu_header_draw_cb(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data)
{
    switch (section_index)
    {
        case 0:
            menu_cell_basic_header_draw(ctx, cell_layer, "Main menu");
            break;
    }
}

static void main_menu_row_draw_cb(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data)
{
    // Determine which section we're going to draw in
    switch (cell_index->section)
    {
        case 0:
            // Use the row to specify which item we'll draw
            switch (cell_index->row)
            {
                case 0:
                    menu_cell_basic_draw(ctx, cell_layer, "Manual entry", "Add new glucose value", NULL);
                    break;
                case 1:
                    menu_cell_basic_draw(ctx, cell_layer, "Results history", "View last values", NULL);
                    break;
                case 2:
                    menu_cell_basic_draw(ctx, cell_layer, "Alerts", "Set thresholds alerts", NULL);
                    break;
                case 3:
                    menu_cell_basic_draw(ctx, cell_layer, "Settings", "Customize SugarStrap", NULL);
                    break;
            }
        break;
    }
}

static void main_menu_select_click_cb(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{
    switch(cell_index->row)
    {
    case MAIN_MENU_RESULTS_HISTORY_IDX:
        history_results_window_init();
        break;
    default:
        break;
    }
}

static void main_menu_window_load(Window *window)
{
    // Now we prepare to initialize the menu layer
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_frame(window_layer);

    // Create the menu layer
    g_menu_layer = menu_layer_create(bounds);
    menu_layer_set_callbacks(g_menu_layer, NULL, (MenuLayerCallbacks){
        .get_num_sections = main_menu_num_sections_get_cb,
        .get_num_rows = main_menu_num_rows_get_cb,
        .get_header_height = PBL_IF_RECT_ELSE(main_menu_header_height_get_cb, NULL),
        .draw_header = PBL_IF_RECT_ELSE(main_menu_header_draw_cb, NULL),
        .draw_row = main_menu_row_draw_cb,
        .select_click = main_menu_select_click_cb,
    });

    // Bind the menu layer's click config provider to the window for interactivity
    menu_layer_set_click_config_onto_window(g_menu_layer, window);

    layer_add_child(window_layer, menu_layer_get_layer(g_menu_layer));
}

static void main_menu_window_unload(Window *window)
{
    // Destroy the menu layer
    menu_layer_destroy(g_menu_layer);
    g_menu_layer = NULL;
}

void main_menu_window_init()
{
    static Window *main_menu_window = NULL;
    main_menu_window = window_create();
    window_set_window_handlers(main_menu_window, (WindowHandlers) {
        .load = main_menu_window_load,
        .unload = main_menu_window_unload,
    });
    window_stack_pop(true);
    window_stack_push(main_menu_window, true);
}