#include "sht30.h"
#include "i2c.h"

void SHT30Setup(){
    I2C_Start(SHT30_ADRESS);
    I2C_Write(COMMAND_BYTE_1);
    I2C_Write(COMMAND_BYTE_2);
    I2C_Stop();
    I2C_Start(SHT30_ADRESS + 1);
    I2C_Read(ACK);
    I2C_Read(ACK);
    I2C_Read(ACK);
    I2C_Read(ACK);
    I2C_Read(ACK);
    I2C_Read(NACK);
    I2C_Stop();
}

bool SHT30Read(float *temp, float *humi){
    unsigned int dataT = 0, dataRH = 0;
    char checksumTemp = 0, checksumHumi = 0;
    
    if(!(I2C_Start(SHT30_ADRESS)))      return false;
    if(!(I2C_Write(COMMAND_BYTE_1)))    return false;
    if(!(I2C_Write(COMMAND_BYTE_2)))    return false;
    if(!(I2C_Stop()))                   return false;
    
    if(!(I2C_Start(SHT30_ADRESS + 1)))  return false;
    dataT = I2C_Read(ACK) << 8;
    dataT |= I2C_Read(ACK);
    checksumTemp = I2C_Read(ACK);
    dataRH = I2C_Read(ACK) << 8;
    dataRH |= I2C_Read(ACK);
    checksumHumi = I2C_Read(NACK);
    if(!(I2C_Stop()))                   return false;      
        
    *temp = (( (float) dataT * 175.0) / 65535.0) - 45.0;
    *humi = ( (float) dataRH * 100.0) / 65535.0;  
    
    return true;
}
