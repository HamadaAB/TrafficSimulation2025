//
// Created by alexa on 2/05/2025.
//

#include "Kruispunt.h"

Kruispunt::Kruispunt() {
    pos_baan1 = 0;
    pos_baan2 = 0;
}

Kruispunt::Kruispunt(std::string baan1, std::string baan2, int pos1, int pos2) {
    naam_baan1 = baan1;
    naam_baan2 = baan2;
    pos_baan1 = pos1;
    pos_baan2 = pos2;
}

std::string Kruispunt::get_road1_name() const { return naam_baan1; }
std::string Kruispunt::get_road2_name() const { return naam_baan2; }
int Kruispunt::get_position1() const { return pos_baan1; }
int Kruispunt::get_position2() const { return pos_baan2; }

void Kruispunt::set_road1_name(const std::string& name) { naam_baan1 = name; }
void Kruispunt::set_road2_name(const std::string& name) { naam_baan2 = name; }
void Kruispunt::set_position1(int pos) { pos_baan1 = pos; }
void Kruispunt::set_position2(int pos) { pos_baan2 = pos; }
