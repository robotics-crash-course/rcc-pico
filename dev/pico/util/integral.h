#include "stdint.h"

#ifndef INTEGRAL_H
#define INTEGRAL_H

struct integral {
        int64_t current_time;
        int64_t previous_time;
        int64_t timestep;
        float sum;
};

#endif /* INTEGRAL_H */

