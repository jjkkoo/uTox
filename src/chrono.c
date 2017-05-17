#include "chrono.h"

#include "debug.h"
#include "macros.h"


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "native/thread.h"

bool chrono_thread_init = false;
static int thread_count = 0;

#include "native/ui.h"
static void chrono_thread(void *args) {
    LOG_INFO("Chono", "Thread starting");

    CHRONO_INFO *info = args;
    chrono_thread_init = true;

    thread_count++;
    LOG_ERR("Chrono", "Number of threads: %d", thread_count);

    while (*info->ptr != *info->target) {
        *info->ptr += info->step;
        yieldcpu(info->interval_ms);
        redraw();
    }

    chrono_thread_init = false;

    if (info->callback) {
        info->callback(info->cb_data);
    }

    info->finished = true;

    LOG_INFO("Chrono", "Thread exited cleanly");
}

bool chrono_start(CHRONO_INFO *info) {
    if (!info) {
        LOG_ERR("Chrono", "Chrono info structure is null.");
        return false;
    }

    thread(chrono_thread, info);

    return true;
}

bool chrono_end(CHRONO_INFO *info) {
    if (!info) {
        LOG_ERR("Chrono", "Chrono info is null");
        return false;
    }

    info->finished = true;

    while (chrono_thread_init) { //wait for thread to die
        yieldcpu(1);
    }

    return true;
}

void chrono_callback(uint32_t ms, void func(void *), void *funcargs) {
    yieldcpu(ms);
    func(funcargs);
}
