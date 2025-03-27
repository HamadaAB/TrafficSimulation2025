

#ifndef ROAD_H
#define ROAD_H
#include <utility>

# include "string"

class Road {
private:
    std::string name;
    int length;
public:
    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] int get_length() const;

    void set_name(std::string new_name);
    void set_length(int new_length);
};
#endif //ROAD_H
