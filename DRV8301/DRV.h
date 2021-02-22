#ifndef DRV_H
#define DRV_H

/// Registers ///
#define SR1 0x0 /// Status Register 1
#define SR2 0x1 /// Status Register 2
#define CR1 0x2 /// Control Register 1
#define CR2 0x3 /// Control Register 2

/// Register maps, ordered from low bits to high bits ///
/// Control Register 1 (Gate Driver Control) Fields ///
#define GATE_CURR_PEAK_1700MA 0x0
#define GATE_CURR_PEAK_700MA 0x1
#define GATE_CURR_PEAK_250MA 0x2
#define GATE_NORMAL 0x0
#define GATE_RESET_FAULTS 0x1 /// reset gate driver latched faults (reverts to 0)
#define PWM_MODE_6X 0x0
#define PWM_MODE_3X 0x1
#define OCP_LIMIT 0x0
#define OCP_LATCH 0x1
#define OCP_REPORT 0x2
#define OCP_NONE 0x3
#define VDS_LVL_0_06 0x0
#define VDS_LVL_0_068 0x1
#define VDS_LVL_0_076 0x2
#define VDS_LVL_0_086 0x3
#define VDS_LVL_0_097 0x4
#define VDS_LVL_0_109 0x5
#define VDS_LVL_0_123 0x6
#define VDS_LVL_0_138 0x7
#define VDS_LVL_0_155 0x8
#define VDS_LVL_0_175 0x9
#define VDS_LVL_0_197 0xA
#define VDS_LVL_0_222 0xB
#define VDS_LVL_0_250 0xC
#define VDS_LVL_0_282 0xD
#define VDS_LVL_0_317 0xE
#define VDS_LVL_0_358 0xF
#define VDS_LVL_0_403 0x10
#define VDS_LVL_0_454 0x11
#define VDS_LVL_0_511 0x12
#define VDS_LVL_0_576 0x13
#define VDS_LVL_0_648 0x14
#define VDS_LVL_0_730 0x15
#define VDS_LVL_0_822 0x16
#define VDS_LVL_0_926 0x17
#define VDS_LVL_1_043 0x18
#define VDS_LVL_1_175 0x19
#define VDS_LVL_1_324 0x1A
#define VDS_LVL_1_491 0x1B
#define VDS_LVL_1_679 0x1C
#define VDS_LVL_1_892 0x1D
#define VDS_LVL_2_131 0x1E
#define VDS_LVL_2_400 0x1F

/// Control Register 2 (Current Shunt Amplifiers and Misc Control) ///
#define REPORT_BOTH 0x0 /// both OT and OC at nOCTW pin
#define REPORT_OT 0x1
#define REPORT_OC 0x2
#define CSA_GAIN_10 0x0
#define CSA_GAIN_20 0x1
#define CSA_GAIN_40 0x2
#define CSA_GAIN_80 0x3
#define CSA1_CONNECT 0x0    /// shunt amplifier connects to load through input pins
#define CSA1_DISCONNECT 0x1 /// shunt amplifier shorts input pins and disconnects from load for extern calibration
#define CSA2_CONNECT 0x0
#define CSA2_DISCONNECT 0x1
#define OC_TOFF_CYCLE 0x0    /// cycle by cycle
#define OC_TOFF_OFF_TIME 0x1 /// off time control

class DRV8301
{
public:
    DRV8301(SPI *spi, DigitalOut *cs);
    int read_SR1();
    int read_SR2();
    int read_register(int reg);
    void write_register(int reg, int val);
    void write_CR1(int OC_ADJ_SET, int OCP_MODE, int PWM_MODE, int GATE_RESET, int GATE_CURRENT);
    void write_CR2(int OC_TOFF, int DC_CAL_CH2, int DC_CAL_CH1, int GAIN, int OCTW_MODE);

    void enable_gd(void);
    void disable_gd(void);
    void calibrate(void);
    void print_faults();

private:
    SPI *_spi;
    DigitalOut *_cs;
    uint16_t spi_write(uint16_t val);
};

#endif