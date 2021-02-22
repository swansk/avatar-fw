#include "mbed.h"
#include "DRV.h"

DRV8301::DRV8301(SPI *spi, DigitalOut *cs)
{
    /// TODO: verify the spi timings are the same
    _spi = spi;
    _cs = cs;
    _cs->write(1);
    wait_us(10);
    _spi->format(16, 1);
    _spi->frequency(500000);
}

uint16_t DRV8301::spi_write(uint16_t val)
{
    _cs->write(0);
    wait_us(10);
    uint16_t reply = _spi->write(val);
    _cs->write(1);
    return reply;
}

int DRV8301::read_SR1(void)
{
    uint16_t val = (1 << 15) | SR1;
    return spi_write(val);
}

int DRV8301::read_SR2(void)
{
    uint16_t val = (1 << 15) | SR2;
    return spi_write(val);
}

int DRV8301::read_register(int reg)
{
    return spi_write((1 << 15) | (reg << 11));
}

void DRV8301::write_register(int reg, int val)
{
    spi_write((reg << 11) | val);
}

void DRV8301::write_CR1(int OC_ADJ_SET, int OCP_MODE, int PWM_MODE, int GATE_RESET, int GATE_CURRENT)
{
    uint16_t val = (CR1 << 11) | (OC_ADJ_SET << 6) | (OCP_MODE << 4) | (PWM_MODE << 3) | (GATE_RESET << 2) | GATE_CURRENT;
    spi_write(val);
}

void DRV8301::write_CR2(int OC_TOFF, int DC_CAL_CH2, int DC_CAL_CH1, int GAIN, int OCTW_MODE)
{
    uint16_t val = (CR2 << 11) | (OC_TOFF << 6) | (DC_CAL_CH2 << 5) | (DC_CAL_CH1 << 4) | (GAIN << 2) | OCTW_MODE;
    spi_write(val);
}

void DRV8301::print_faults(void)
{
    uint16_t val1 = read_SR1();
    wait_us(10);
    uint16_t val2 = read_SR2();
    wait_us(10);

    if (val1 & (1 << 10))
    {
        printf("\n\rFAULT\n\r");
    }

    if (val1 & (1 << 9))
    {
        printf("VDS_OCP\n\r");
    }
    if (val1 & (1 << 8))
    {
        printf("GDF\n\r");
    }
    if (val1 & (1 << 7))
    {
        printf("UVLO\n\r");
    }
    if (val1 & (1 << 6))
    {
        printf("OTSD\n\r");
    }
    if (val1 & (1 << 5))
    {
        printf("VDS_HA\n\r");
    }
    if (val1 & (1 << 4))
    {
        printf("VDS_LA\n\r");
    }
    if (val1 & (1 << 3))
    {
        printf("VDS_HB\n\r");
    }
    if (val1 & (1 << 2))
    {
        printf("VDS_LB\n\r");
    }
    if (val1 & (1 << 1))
    {
        printf("VDS_HC\n\r");
    }
    if (val1 & (1))
    {
        printf("VDS_LC\n\r");
    }

    if (val2 & (1 << 10))
    {
        printf("SA_OC\n\r");
    }
    if (val2 & (1 << 9))
    {
        printf("SB_OC\n\r");
    }
    if (val2 & (1 << 8))
    {
        printf("SC_OC\n\r");
    }
    if (val2 & (1 << 7))
    {
        printf("OTW\n\r");
    }
    if (val2 & (1 << 6))
    {
        printf("CPUV\n\r");
    }
    if (val2 & (1 << 5))
    {
        printf("VGS_HA\n\r");
    }
    if (val2 & (1 << 4))
    {
        printf("VGS_LA\n\r");
    }
    if (val2 & (1 << 3))
    {
        printf("VGS_HB\n\r");
    }
    if (val2 & (1 << 2))
    {
        printf("VGS_LB\n\r");
    }
    if (val2 & (1 << 1))
    {
        printf("VGS_HC\n\r");
    }
    if (val2 & (1))
    {
        printf("VGS_LC\n\r");
    }
}

/// TODO: verify if these are needed or 8301 has these registers
void DRV8301::enable_gd(void)
{
    uint16_t val = (read_register(DCR)) & (~(0x1 << 2));
    write_register(DCR, val);
}

void DRV8301::disable_gd(void)
{
    uint16_t val = (read_register(DCR)) | (0x1 << 2);
    write_register(DCR, val);
}

void DRV8301::calibrate(void)
{
    uint16_t val = 0x1 << 4 + 0x1 << 3 + 0x1 << 2;
    write_register(CSACR, val);
}