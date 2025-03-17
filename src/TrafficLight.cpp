#include "TrafficLight.h"

void TrafficLight::change_light() {
    if (is_green()) {
        is_light_green = false;
    }
    else {
        is_light_green = true;
    }
}