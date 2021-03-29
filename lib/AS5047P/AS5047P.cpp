#include "mbed.h"
#include "AS5047P.h"
 
AS5047P::AS5047P(int CPR, float offset, int ppairs, SPI &spi, DigitalOut &cs) : _spi(spi), _cs(cs) {
    //_CPR = CPR;
    _CPR = CPR;
    _ppairs = ppairs;
    ElecOffset = offset;
    rotations = 0;
    
    _cs.write(1);
    readAngleCmd = 0xffff;   
    MechOffset = offset;
    modPosition = 0;
    oldModPosition = 0;
    oldVel = 0;
    raw = 0;
    first_sample = 0;
    for (int i = 0; i < 100; i++)              // Initial measurement is really noisy
    {
        _spi.write(0);
        wait_us(100);
    }
}
    
void AS5047P::Sample(float dt){
    GPIOA->ODR &= ~(1 << 15);
    _cs.write(0);   
    raw = _spi.write(readAngleCmd);
    raw &= 0x3FFF;
    // err = 1 << 14 & raw;
    // raw = raw >> 2;      //Extract last 14 bits
    GPIOA->ODR |= (1 << 15);
    int off_1 = offset_lut[raw >> 7];
    int off_2 = offset_lut[((raw >> 7) + 1) % 128];
    int off_interp = off_1 + ((off_2 - off_1) * (raw - ((raw >> 7) << 7)) >> 7); // Interpolate between lookup table entries
    int angle = raw + off_interp;                                                // Correct for nonlinearity with lookup table from calibration
    
    if (first_sample) {
        if (angle - old_counts > _CPR / 2) {
            rotations -= 1;
        } else if (angle - old_counts < -_CPR / 2) {
            rotations += 1;
        }
    }
    if (!first_sample) first_sample = 1;
    
    old_counts = angle;
    oldModPosition = modPosition;
    modPosition = ((2.0f * PI * ((float) angle)) / (float)_CPR);
    position = (2.0f * PI * ((float) angle + (_CPR * rotations))) / (float)_CPR;
    MechPosition = position - MechOffset;
    float elec = ((2.0f * PI / (float)_CPR) * (float)((_ppairs * angle) % _CPR)) + ElecOffset;
    if (elec < 0) elec += 2.0f * PI;
    else if (elec > 2.0f * PI) elec -= 2.0f * PI ; 
    ElecPosition = elec;
    
    float vel;
    //if(modPosition<.1f && oldModPosition>6.1f){
 
    if ((modPosition-oldModPosition) < -3.0f) {
        vel = (modPosition - oldModPosition + 2.0f*PI)/dt;
    }
    //else if(modPosition>6.1f && oldModPosition<0.1f){
    else if ((modPosition - oldModPosition) > 3.0f) {
        vel = (modPosition - oldModPosition - 2.0f*PI)/dt;
    } else {
        vel = (modPosition-oldModPosition)/dt;
    }    
    
    int n = 40;
    float sum = vel;
    for (int i = 1; i < (n); i++) {
        velVec[n - i] = velVec[n-i-1];
        sum += velVec[n-i];
    }
    velVec[0] = vel;
    MechVelocity =  sum/((float)n);
    ElecVelocity = MechVelocity*_ppairs;
    ElecVelocityFilt = 0.99f*ElecVelocityFilt + 0.01f*ElecVelocity;
    _cs.write(1);  
}
 
int AS5047P::GetRawPosition(){
    return raw;
}

int AS5047P::GetSPIReadError(){
    return err;
}
 
float AS5047P::GetMechPositionFixed(){
    return MechPosition+MechOffset;
}
    
float AS5047P::GetMechPosition(){
    return MechPosition;
}
 
float AS5047P::GetElecPosition(){
    return ElecPosition;
}
 
float AS5047P::GetElecVelocity(){
    return ElecVelocity;
}
 
float AS5047P::GetMechVelocity(){
    return MechVelocity;
}
 
void AS5047P::ZeroPosition(){
    rotations = 0;
    MechOffset = 0;
    Sample(.00025f);
    MechOffset = GetMechPosition();
}
    
void AS5047P::SetElecOffset(float offset){
    ElecOffset = offset;
}

void AS5047P::SetMechOffset(float offset){
    MechOffset = offset;
    first_sample = 0;
}
 
int AS5047P::GetCPR(){
    return _CPR;
}
 
 
void AS5047P::WriteLUT(int new_lut[128]){
    memcpy(offset_lut, new_lut, sizeof(offset_lut));
}
