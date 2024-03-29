#ifndef RS485_Katz_Comm_H
#define RS485_Katz_Comm_H

#define SEL_RX 0
#define SEL_TX 1

#include "mbed.h"
#include "RS485.h"
#include "Controller.h"
#include "RS485KatzMsgs.h"
#include "MathOps.h"

// #include "user_config.h"

enum DUPLEX_MODE {
    RECEIVE = 0,
    SEND = 1
};

class RS485KatzComm {
    public:
        /**
         * Construct a RS485KatzComm instance, inject RS485 interface.
         * 
         * @param rs485 Instance of rs485 interface.
         * @param de Instance of pin connected to DE MAX485 pin.
         * @param nre Instance of pin connected to !RE MAX485 pin.
         * @param controller Pointer to controller observer.
         */
        RS485KatzComm(RS485 &rs485, DigitalOut &de, DigitalOut &nre, Controller *controller): _rs485(rs485), _de(de), _nre(nre) {
            _controller = controller;
        }

        /**
         * Start listening for comms on RS485 line.
         */
        void listen_begin();

        /**
         * Stop listening for comms on RS485 line.
         */
        void listen_end();

        /**
         * Send status over RS485 line.
         * 
         * @param id byte representing ID of board.
         * @param p  16 bit position, between -4*pi and 4*pi.
         * @param v  12 bit velocity, between -30 and + 30 rad/s.
         * @param a  12 bit current, between -40 and 40.
         */
        void send_status_msg(uint8_t id, uint16_t p, uint16_t v, uint16_t a);

        /**
         * Set duplex mode.
         * 
         * @param mode enum representing duplex mode
         */
        void set_duplex_mode(enum DUPLEX_MODE mode);

    private:
        /**
         * Function to monitor RS485 line and update observers on msg reception
         */
        void monitor_update_observers();

        RS485 &_rs485;
        DigitalOut &_de;
        DigitalOut &_nre;
        Thread _thread;
        Controller *_controller = nullptr;

};

#endif // RS485_Katz_Comm_H