#include <pebble.h>

#include "testing_window.h"
#include "startup_window.h"

static void init()
{
    startup_window_init();
}

static void deinit()
{
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}
