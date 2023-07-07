#include <vector>
using std::vector; 

struct Point {
    double x, y; 
}; 

struct Attendee {
    Point location; 
    vector<double> tastes; 
}; 

struct Stage {
    double room_width, room_height; 
    double stage_width, stage_height; 
    Point stage_bottom_left; 
    vector<int> musicians; 
    vector<Attendee> attendees; 
}; 