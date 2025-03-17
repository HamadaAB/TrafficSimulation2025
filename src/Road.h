

#ifndef ROAD_H
#define ROAD_H
#include <utility>

# include "string"

class Road {
private:
    std::string name;
    int length;
public:
    [[nodiscard]] std::string get_name() const{return name;}
    [[nodiscard]] int get_length() const{return length;}

    void set_name(std::string new_name) {name = std::move(new_name);}
    void set_length(int new_length) {length = new_length;}
};
#endif //ROAD_H
