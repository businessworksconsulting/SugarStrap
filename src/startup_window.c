#include <pebble.h>

#include "randgen.h"
#include "startup_window.h"
#include "testing_window.h"
#include "main_menu.h"

static int tmp_counter = 0;
static void tmp_counter_increment()
{
    tmp_counter++;
}

static void canvas_update_proc(Layer *layer, GContext *ctx)
{
    GBitmap *bitmap = gbitmap_create_with_resource(RESOURCE_ID_STARTUP);

    graphics_context_set_compositing_mode(ctx, GCompOpSet);
    GRect bitmap_bounds = gbitmap_get_bounds(bitmap);
    graphics_draw_bitmap_in_rect(ctx, bitmap, bitmap_bounds);
    
    gbitmap_destroy(bitmap);
}

static void tick_handler(struct tm *tick_time, TimeUnits changed)
{
    tmp_counter_increment();
    if (tmp_counter >= 5)
    {
        tick_timer_service_unsubscribe();
        randgen_seed(time_ms(NULL, NULL));
        if (randgen(1))
            testing_window_init();
        else
            main_menu_window_init();
    }
}

static Layer *canvas_layer = NULL;
static void startup_window_load(Window *window)
{
    Layer *window_layer = window_get_root_layer(window);
    GRect window_bounds = layer_get_bounds(window_layer);

    canvas_layer = layer_create(window_bounds);
    layer_set_update_proc(canvas_layer, canvas_update_proc);
    layer_add_child(window_layer, canvas_layer);
    
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void startup_window_unload(Window *window)
{
    tick_timer_service_unsubscribe();
    layer_remove_child_layers(window_get_root_layer(window));
    layer_destroy(canvas_layer);
    canvas_layer = NULL;
    window_destroy(window);
}

void startup_window_init()
{
    static Window *g_startup_window = NULL;
    g_startup_window = window_create();
    window_set_window_handlers(g_startup_window, (WindowHandlers)
        {
        .load = startup_window_load,
        .unload = startup_window_unload
        });
    window_stack_push(g_startup_window, true);
}

