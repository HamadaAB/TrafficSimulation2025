//
// Created by alexa on 1/05/2025.
//

#ifndef PSE_BUSHALTE_H
#define PSE_BUSHALTE_H

#include "string"


class Bushalte {
private:
    std::string naam_baan;
    int positie;
    int wachttijd;

public:
    // default constructor
    Bushalte();
    // quick constructor
    Bushalte(std::string baan, int pos, int wacht);

    [[nodiscard]] std::string get_road_name() const;
    [[nodiscard]] int get_position() const;
    [[nodiscard]] int get_wait_time() const;

    void set_road_name(const std::string& name);
    void set_position(int pos);
    void set_wait_time(int time);
};


#endif //PSE_BUSHALTE_H
