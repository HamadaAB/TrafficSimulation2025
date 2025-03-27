#include "TrafficLight.h"


 std::string TrafficLight::get_road_name() const{return road_name;}
 int TrafficLight::get_position() const{return position;}
 int TrafficLight::get_cyclus() const{return cyclus;}
 bool TrafficLight::is_green() const{return is_light_green;}
 double TrafficLight::get_last_change_time() const{return last_change_time;}

void TrafficLight::set_road_name(const std::string& name) {road_name = name;}
void TrafficLight::set_position(int pos) {position = pos;}
void TrafficLight::set_cyclus(int new_cyclus) {cyclus = new_cyclus;}
void TrafficLight::set_light(bool light) {is_light_green = light;}
void TrafficLight::set_last_change_time(double time) {last_change_time = time;}


void TrafficLight::change_light() {
    if (is_green()) {
        is_light_green = false;
    }
    else {
        is_light_green = true;
    }
}