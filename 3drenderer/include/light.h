#pragma once
#include "vector.h"
#include <stdint.h>

struct light_t {
    vec3_t direction;
};

extern light_t light;

uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor);