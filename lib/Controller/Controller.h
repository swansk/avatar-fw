#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mbed.h"

#include "DRV8301.h"
#include "AS5047P.h"

class Controller {
    public:
        Controller(AS5047P &positionSensor, DRV8301 &motorDriver): _positionSensor(positionSensor), _motorDriver(motorDriver) {}
        // Controller(AS5047P &positionSensor): _positionSensor(positionSensor) {}

        /**
         * Update the controller with new PD and torque values.
         * 
         * @param p    Position command, between -4*pi and 4*pi. 
         * @param v    Velocity command, between -30 and + 30 rad/s.
         * @param kp   kp, between 0 and 500 N-m/rad.
         * @param kd   kd, between 0 and 100 N-m*s/rad.
         * @param t_ff feed forward torque, between -18 and 18 N-m.
         */
        void update(float p, float v, float kp, float kd, float t_ff);

        /**
         * Read the status of the controller.
         * 
         * @returns array containing: 16 bit position, between -4*pi and 4*pi
         *                            12 bit velocity, between -30 and + 30 rad/s
         *                            12 bit current, between -40 and 40;
         */
        float * read_status();

    private:
        AS5047P &_positionSensor;
        DRV8301 &_motorDriver;
        float _p;
        float _v;
        float _kp;
        float _kd; 
        float _t_ff; 
};

#endif // CONTROLLER_H
