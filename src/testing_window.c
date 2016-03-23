#include <pebble.h>

#include "testing_window.h"

static TextLayer *output_layer = NULL;
static Layer *canvas_layer = NULL;

static int image_sequence = 1;

static void image_sequence_increment()
{
    image_sequence = image_sequence % 5 + 1;
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context)
{
    text_layer_set_text(output_layer, "Up pressed!");
}
static void down_click_handler(ClickRecognizerRef recognizer, void *context)
{
    text_layer_set_text(output_layer, "Down pressed!");
}

static void click_config_provider(void *context)
{
    // Register the ClickHandlers
    window_raw_click_subscribe(BUTTON_ID_UP, down_click_handler, up_click_handler, context);
    window_raw_click_subscribe(BUTTON_ID_SELECT, down_click_handler, up_click_handler, context);
    window_raw_click_subscribe(BUTTON_ID_DOWN, down_click_handler, up_click_handler, context);
}

static GBitmap* bitmap_select_and_create()
{
    switch(image_sequence)
    {
    case 1:
        return gbitmap_create_with_resource(RESOURCE_ID_TESTING_1);
    case 2:
        return gbitmap_create_with_resource(RESOURCE_ID_TESTING_2);
    case 3:
        return gbitmap_create_with_resource(RESOURCE_ID_TESTING_3);
    case 4:
        return gbitmap_create_with_resource(RESOURCE_ID_TESTING_4);
    case 5:
        return gbitmap_create_with_resource(RESOURCE_ID_TESTING_5);
    default:
        return gbitmap_create_with_resource(RESOURCE_ID_TESTING_1);
    }
}

static void canvas_update_proc(Layer *layer, GContext *ctx)
{
    GBitmap *bitmap = bitmap_select_and_create();
    GRect bitmap_bounds = gbitmap_get_bounds(bitmap);
    
    graphics_context_set_compositing_mode(ctx, GCompOpSet);
    graphics_draw_bitmap_in_rect(ctx, bitmap, bitmap_bounds);
    
    gbitmap_destroy(bitmap);
}

static void tick_handler(struct tm *tick_time, TimeUnits changed)
{
    image_sequence_increment();
    layer_mark_dirty(canvas_layer);
}

static void testing_window_load(Window *window)
{
    Layer *window_layer = window_get_root_layer(window);
    GRect window_bounds = layer_get_bounds(window_layer);

    // Create output TextLayer
    output_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 5, window_bounds.size.h));
    text_layer_set_font(output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
    text_layer_set_text(output_layer, "No button pressed yet.");
    text_layer_set_overflow_mode(output_layer, GTextOverflowModeWordWrap);
    layer_add_child(window_layer, text_layer_get_layer(output_layer));

    // Create canvas layer
    canvas_layer = layer_create(window_bounds);
    layer_set_update_proc(canvas_layer, canvas_update_proc);
    layer_add_child(window_layer, canvas_layer);
    
    // Get updates when the current minute changes
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void testing_window_unload(Window *window)
{
    text_layer_destroy(output_layer);
    output_layer = NULL;
    layer_destroy(canvas_layer);
    canvas_layer = NULL;
    window_destroy(window);
}

void testing_window_init()
{
    static Window *testing_window = NULL;
    testing_window = window_create();
    window_set_window_handlers(testing_window, (WindowHandlers)
        {
        .load = testing_window_load,
        .unload = testing_window_unload
        });
    window_set_click_config_provider(testing_window, click_config_provider);
    window_stack_pop(true);
    window_stack_push(testing_window, true);
}
