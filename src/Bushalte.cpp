//
// Created by alexa on 1/05/2025.
//

#include "Bushalte.h"
// default constructor
Bushalte::Bushalte() {
    positie = 0;
    wachttijd = 0;
}

// quick constructor
Bushalte::Bushalte(const std::string baan, int pos, int wacht) {
    naam_baan = baan;
    positie = pos;
    wachttijd = wacht;
}

std::string Bushalte::get_road_name() const { return naam_baan; }
int Bushalte::get_position() const { return positie; }
int Bushalte::get_wait_time() const { return wachttijd; }

void Bushalte::set_road_name(const std::string& name) { naam_baan = name; }
void Bushalte::set_position(int pos) { positie = pos; }
void Bushalte::set_wait_time(int time) { wachttijd = time; }
