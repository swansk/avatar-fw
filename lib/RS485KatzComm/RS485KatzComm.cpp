#include "RS485KatzComm.h"

#define P_MIN -12.5f
#define P_MAX 12.5f
#define V_MIN -65.0f
#define V_MAX 65.0f
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 5.0f
#define T_MIN -18.0f
#define T_MAX 18.0f

void RS485KatzComm::listen_begin() {
    _thread.start(callback(this, &RS485KatzComm::monitor_update_observers));
}

void RS485KatzComm::listen_end() {
    _thread.join();
}

void RS485KatzComm::monitor_update_observers() {
    /// RS485 Command Packet Structure ///
    /// 16 bit position command, between -4*pi and 4*pi
    /// 12 bit velocity command, between -30 and + 30 rad/s
    /// 12 bit kp, between 0 and 500 N-m/rad
    /// 12 bit kd, between 0 and 100 N-m*s/rad
    /// 12 bit feed forward torque, between -18 and 18 N-m
    /// CAN Packet is 8 8-bit words
    /// Formatted as follows.  For each quantity, bit 0 is LSB
    /// 0: [position[15-8]]
    /// 1: [position[7-0]] 
    /// 2: [velocity[11-4]]
    /// 3: [velocity[3-0], kp[11-8]]
    /// 4: [kp[7-0]]
    /// 5: [kd[11-4]]
    /// 6: [kd[3-0], torque[11-8]]
    /// 7: [torque[7-0]]
    uint8_t msg[7];
    while (1) {
        if (_rs485.readable() > 0) {
            // Set msg to zeros
            memset(msg, 0, sizeof(msg));
            ThisThread::sleep_for(200); // TODO not sure what this does here.
            _rs485.recvMsg(msg, sizeof(msg), 20);
            int p_int = (msg[0]<<8)|msg[1];
            int v_int = (msg[2]<<4)|(msg[3]>>4);
            int kp_int = ((msg[3]&0xF)<<8)|msg[4];
            int kd_int = (msg[5]<<4)|(msg[6]>>4);
            int t_int = ((msg[6]&0xF)<<8)|msg[7];
            
            float p_des = uint_to_float(p_int, P_MIN, P_MAX, 16);
            float v_des = uint_to_float(v_int, V_MIN, V_MAX, 12);
            float kp = uint_to_float(kp_int, KP_MIN, KP_MAX, 12);
            float kd = uint_to_float(kd_int, KD_MIN, KD_MAX, 12);
            float t_ff = uint_to_float(t_int, T_MIN, T_MAX, 12);

            _controller->update(p_des, v_des, kp, kd, t_ff);
        }
    }
}

void RS485KatzComm::send_status_msg(uint8_t id, float p, float v, float a) {
    int p_int = float_to_uint(p, P_MIN, P_MAX, 16);
    int v_int = float_to_uint(v, V_MIN, V_MAX, 12);
    int t_int = float_to_uint(a, -T_MAX, T_MAX, 12);
    // RS485 Status Packet Structure
    // 16 bit position, between -4*pi and 4*pi
    // 12 bit velocity, between -30 and + 30 rad/s
    // 12 bit current, between -40 and 40;
    // CAN Packet is 5 8-bit words
    // Formatted as follows.  For each quantity, bit 0 is LSB
    // 0: [position[15-8]]
    // 1: [position[7-0]] 
    // 2: [velocity[11-4]]
    // 3: [velocity[3-0], current[11-8]]
    // 4: [current[7-0]]
    uint8_t data[6] = {id, p_int >> 8, p_int & 0xFF, v_int >> 4, ((v_int & 0xF) << 4 + (t_int >> 8)), t_int & 0xFF};
    _duplexSel = SEL_TX;
    _rs485.sendMsg(data, sizeof(data));
    // Wait for msg to send, TODO how long do we need to wait?
    ThisThread::sleep_for(1);
    _duplexSel = SEL_RX;
}
