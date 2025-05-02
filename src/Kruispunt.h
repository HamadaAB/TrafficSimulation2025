//
// Created by alexa on 2/05/2025.
//

#ifndef PSE_KRUISPUNT_H
#define PSE_KRUISPUNT_H
#include "string"


class Kruispunt {
private:
    std::string naam_baan1;
    int pos_baan1;
    std::string naam_baan2;
    int pos_baan2;

public:
    Kruispunt();

    Kruispunt(std::string baan1, std::string baan2, int pos1, int pos2);

    [[nodiscard]] std::string get_road1_name() const;
    [[nodiscard]] std::string get_road2_name() const;
    [[nodiscard]] int get_position1() const;
    [[nodiscard]] int get_position2() const;

    void set_road1_name(const std::string& name);
    void set_road2_name(const std::string& name);
    void set_position1(int pos);
    void set_position2(int pos);
};


#endif //PSE_KRUISPUNT_H
