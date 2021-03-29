#ifndef AS5047P_H
#define AS5047P_H

#define PI 3.14159

class PositionSensor {
public:
    virtual void Sample(float dt) = 0;
    virtual float GetMechPosition() {return 0.0f;}
    virtual float GetMechPositionFixed() {return 0.0f;}
    virtual float GetElecPosition() {return 0.0f;}
    virtual float GetMechVelocity() {return 0.0f;}
    virtual float GetElecVelocity() {return 0.0f;}
    virtual void ZeroPosition(void) = 0;
    virtual int GetRawPosition(void) = 0;
    virtual void SetElecOffset(float offset) = 0;
    virtual int GetCPR(void) = 0;
    virtual  void WriteLUT(int new_lut[128]) = 0;
};
 
class AS5047P: public PositionSensor{
public:
    AS5047P(int CPR, float offset, int ppairs, SPI &spi, DigitalOut &cs);
    virtual void Sample(float dt);
    virtual float GetMechPosition();
    virtual float GetMechPositionFixed();
    virtual float GetElecPosition();
    virtual float GetMechVelocity();
    virtual float GetElecVelocity();
    virtual int GetRawPosition();
    virtual void ZeroPosition();
    virtual void SetElecOffset(float offset);
    virtual void SetMechOffset(float offset);
    virtual int GetCPR(void);
    virtual void WriteLUT(int new_lut[128]);
    virtual int GetSPIReadError();
private:
    float position, ElecPosition, ElecOffset, MechPosition, MechOffset, modPosition, oldModPosition, oldVel, velVec[40], MechVelocity, ElecVelocity, ElecVelocityFilt;
    int raw, _CPR, rotations, old_counts, _ppairs, first_sample, err;
    SPI &_spi;
    DigitalOut &_cs;
    int readAngleCmd;
    int offset_lut[128];
 
};
 
#endif // AS5047P_H
