#ifndef RS485KatzComm_H
#define RS485KatzComm_H

#include "mbed.h"
#include "RS485.h"

// #include "../structs.h"
// #include "user_config.h"
// #include "../math_ops.h"

class RS485KatzComm {
    public:
        /**
         * Construct a RS485KatzComm instance, inject RS485 interface.
         * 
         * @param rs485 Instance of rs485 interface.
         */
        RS485KatzComm(RS485 &rs485, Controller controller): rs485(rs485) {
            controller = controller;
         }

        /**
         * Start listening for comms on RS485 line.
         */
        void listen_begin();

        /**
         * Stop listening for comms on RS485 line.
         */
        void listen_end();

    private:
        /**
         * Function to monitor RS485 line and update observers on msg reception
         */
        void monitor_update_observers();

        RS485 &rs485;
        Thread thread;
        Controller *controller;
 
    void pack_reply(CANMessage *msg, float p, float v, float t);
    void unpack_cmd(CANMessage msg, ControllerStruct * controller);
}

#endif