#include "Controller.h"

void Controller::update(float p, float v, float kp, float kd, float t_ff) {
    _p = p;
    _v = v;
    _kp = kp,
    _kd = kd,
    _t_ff = t_ff;
}

float * Controller::read_status() {
    float p = (float)_positionSensor.GetCPR();
    float v = 0;
    float a = 0;
    float arr[3] = {p, v, a};
    
    return arr;
}