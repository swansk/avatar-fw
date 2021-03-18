#include "Controller.h"

void Controller::update(float p, float v, float kp, float kd, float t_ff) {
    _p = p;
    _v = v;
    _kp = kp,
    _kd = kd,
    _t_ff = t_ff;
}