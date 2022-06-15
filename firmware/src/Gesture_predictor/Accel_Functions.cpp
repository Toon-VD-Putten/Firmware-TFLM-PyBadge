#include "definitions.h"
#include "Gesture_predictor/Accel_Functions.hpp"

//Library variable, do not alter.
volatile static bool transferComplete = false; //Needs to be volatile so the
                                               //compiler leaves it alone.
/*----------------------------------------------------------------------------*/
//Library function, do not use.
/*----------------------------------------------------------------------------*/
 static void i2cEventHandler(uintptr_t contextHandle){ 
    if (SERCOM2_I2C_ErrorGet() == SERCOM_I2C_ERROR_NONE)
    {
        transferComplete = true;
    }
    else{
        printf("I2C communication failed \r\n");
    }
}
/*----------------------------------------------------------------------------*/
//Library function, do not use.
/*----------------------------------------------------------------------------*/
void LIS3DH::read_I2R(uint8_t* rdData, uint32_t rdLength){ 
    SERCOM2_I2C_Read(LIS3DH_ADDRESS, rdData, rdLength);
    while (!transferComplete);
    transferComplete = false;
}
/*----------------------------------------------------------------------------*/
//Library function, do not use.
/*----------------------------------------------------------------------------*/
void LIS3DH::write_I2R(uint8_t* wrData, uint32_t wrLength){
    SERCOM2_I2C_Write(LIS3DH_ADDRESS, wrData, wrLength); 
    while (!transferComplete); //Wait for the transfer to be completed
    transferComplete = false;  //Reset wait condition
}
/*----------------------------------------------------------------------------*/
//The write_Read function is used by the other write/read functions.
/*----------------------------------------------------------------------------*/
void LIS3DH::write_Read(uint8_t* wrData, uint32_t wrLength, uint8_t* rdData, 
        uint32_t rdLength){
    SERCOM2_I2C_WriteRead(LIS3DH_ADDRESS,wrData,wrLength,rdData,rdLength);
        /* Poll and wait for the transfer to complete */
    while (!transferComplete);
    transferComplete = false;
}
/*----------------------------------------------------------------------------*/
//The Begin function sets up the callback i2c event handler, initiates certain
//registers, sets up up the interupt handling and data filtering.
/*----------------------------------------------------------------------------*/
void LIS3DH::begin(){
    SERCOM2_I2C_CallbackRegister(i2cEventHandler, 0);
    //Resetting the control registers to default values to make sure.
    //Hardwired bits needed for correct operation NO TOUCH
    write_Register(CTRL_REG0, 0b00010000);    
    //Sets up the interrupt for datarate sync NO TOUCH
    write_Register(CTRL_REG3, 0b00010000); 
    //Enables all 3 axis.
    write_Register(CTRL_REG1, 0b00000111);
    //Enable internal high pass filter 
    write_Register(CTRL_REG2, 0b01001000); 
}
/*----------------------------------------------------------------------------*/
//The write_Register function writes a value to a register of choice.
//Used when you don't care about a return value
/*----------------------------------------------------------------------------*/
void LIS3DH::write_Register(uint8_t reg, uint8_t value){
    uint8_t buffer[] = {reg,value};
    write_I2R(buffer, sizeof(buffer));
}
/*----------------------------------------------------------------------------*/
//The read_Register functions returns the value of a register of choice.
/*----------------------------------------------------------------------------*/
uint8_t LIS3DH::read_Register(uint8_t reg){
    uint8_t rxData = 0;
    write_Read(&reg, 1, &rxData, 1);
    return rxData;
}
/*----------------------------------------------------------------------------*/
//The read_X_Accel_Raw function reads the raw X accel value and 
//returns this to a 16 bit unsigned integer.
/*----------------------------------------------------------------------------*/
int16_t LIS3DH::read_X_Accel_Raw(void){
    uint8_t start = OUT_X_L | 0x80;     //Address to start reading from,
                                        //with bit set to read multiple registers
    uint8_t data[2] = {0};              //Set up array to store readings in.
    //while(INTERUPT_IN_Get() == false){} //While loop to sync register readings
                                        //with sensor datarate.
    write_Read(&start, 1, data, 2);     //Initiate transfer.
    int16_t X_Accel_Raw = 0;            //Reset X_Accel_Raw variable.
    X_Accel_Raw = data[0];              //First byte.      
    X_Accel_Raw |= data[1] << 8;        //Second byte shifted over.
    X_Accel_Raw >>= 6;                  //Data is left aligned, bit shift it over
                                        //6 places to right align it. 
    return X_Accel_Raw;                 //Return the X_Accel_Raw value. 
}
/*----------------------------------------------------------------------------*/
//The read_Y_Accel_Raw function reads the raw Y accel value and 
//returns this to a 16 bit unsigned integer.
/*----------------------------------------------------------------------------*/
int16_t LIS3DH::read_Y_Accel_Raw(void){
    uint8_t start = OUT_Y_L | 0x80;     //Address to start reading from,
                                        //with bit set to read multiple registers
    uint8_t data[2] = {0};              //Set up array to store readings in.
    //while(INTERUPT_IN_Get() == false){} //While loop to sync register readings
                                        //with sensor datarate.
    write_Read(&start, 1, data, 2);     //Initiate transfer.
    int16_t Y_Accel_Raw = 0;            //Reset Y_Accel_Raw variable.
    Y_Accel_Raw = data[0];              //First byte.      
    Y_Accel_Raw |= data[1] << 8;        //Second byte shifted over.
    Y_Accel_Raw >>= 6;                  //Data is left aligned, bit shift it over
                                        //6 places to right align it. 
    return Y_Accel_Raw;                 //Return the Y_Accel_Raw value. 
}
/*----------------------------------------------------------------------------*/
// The read_Z_Accel_Raw function reads the raw Z accel value and 
//returns this to a 16 bit unsigned integer.
/*----------------------------------------------------------------------------*/
int16_t LIS3DH::read_Z_Accel_Raw(void){
    uint8_t start = OUT_Z_L | 0x80;     //Address to start reading from,
                                        //with bit set to read multiple registers
    uint8_t data[2] = {0};              //Set up array to store readings in.
    //while(INTERUPT_IN_Get() == false){} //While loop to sync register readings
                                        //with sensor datarate.
    write_Read(&start, 1, data, 2);     //Initiate transfer.
    int16_t Z_Accel_Raw = 0;            //Reset Z_Accel_Raw variable.
    Z_Accel_Raw = data[0];              //First byte.      
    Z_Accel_Raw |= data[1] << 8;        //Second byte shifted over.
    Z_Accel_Raw >>= 6;                  //Data is left aligned, bit shift it over
                                        //6 places to right align it. 
    return Z_Accel_Raw;                 //Return the Z_Accel_Raw value.                                     
}
/*----------------------------------------------------------------------------*/
//The read_All_Raw function reads the raw values from the LIS3DH accelerometer 
//and stores them in a global variable.
/*----------------------------------------------------------------------------*/
void LIS3DH::read_All_Raw(void){
    uint8_t start = OUT_X_L | 0x80; //Address to start reading from, 
                                    //with bit set to read multiple registers.
    uint8_t Data[6] = {0};          //Set up array to store readings in.
    
    while(INTERUPT_IN_Get() == false){} //While loop to sync register readings
                                        //with sensor datarate. 
    write_Read(&start, 1, Data, 6);     //Initiate transfer.
    
    x_r, y_r, z_r = 0;      //Reset raw value variables.

    //Read XL/XH Registers
    x_r = Data[0];          //First byte.
    x_r |= ((uint16_t)Data[1]) << 8;    //Second byte shifted over.
    //Read YL/YH Registers
    y_r = Data[2];          //First byte
    y_r |= ((uint16_t)Data[3]) << 8;    //Second byte shifted over.
    //Read ZL/ZH Registers
    z_r = Data[4];          //First byte
    z_r |= ((uint16_t)Data[5]) << 8;    //Second byte shifted over.
    
    //x_r >>= 6;              //Data is left aligned, bit shift it over
    //y_r >>= 6;              //Data is left aligned, bit shift it over 
    //z_r >>= 6;              //Data is left aligned, bit shift it over 
}
/*----------------------------------------------------------------------------*/
//The read_All_Scaled function reads the raw values from the LIS3DH accelerometer 
//and scales them according to the set sensor scale before storing them in a 
//global variable.
/*----------------------------------------------------------------------------*/
void LIS3DH::read_All_Scaled(void){ 
    read_All_Raw();//Calls the above function for the raw sensor values.
    uint8_t lsb_Value = 0; //Variable for the sensor scale.
    x_g, y_g, z_g = 0; //Reset scaled value variables.

    lsb_Value = 1;
    switch(get_Range()){  //Gets the set range of the accelerometer.
        case LIS3DH_RANGE_2_G:    lsb_Value = 4;  break; //4000mG  Scale for 2G
        case LIS3DH_RANGE_4_G:    lsb_Value = 8;  break; //8000mG  Scale for 4G
        case LIS3DH_RANGE_8_G:    lsb_Value = 16; break; //16000mG Scale for 8G
        case LIS3DH_RANGE_16_G:   lsb_Value = 48; break; //32000mG Scale for 16G
    }  

    x_g = lsb_Value * ((float)x_r/
          LIS3DH_LSB16_TO_KILO_LSB10);
    y_g = lsb_Value * ((float)y_r/
          LIS3DH_LSB16_TO_KILO_LSB10);
    z_g = lsb_Value * ((float)z_r/
          LIS3DH_LSB16_TO_KILO_LSB10); 
}
/*----------------------------------------------------------------------------*/
//The get_DeviceID function returns the device ID
/*----------------------------------------------------------------------------*/
uint8_t LIS3DH::get_DeviceID(){
    uint8_t wrData = WHO_AM_I;          //Device ID register.
    uint8_t rdData = 0;                 //Device ID variable.
    write_Read(&wrData,1,&rdData,0x01); //Initiate transfer.
    return rdData;                      //Return Device ID
}
/*----------------------------------------------------------------------------*/
//The set_Range function sets the G-force range of the LIS3DH accelerometer.
/*----------------------------------------------------------------------------*/
void LIS3DH::set_Range(LIS3DH_RANGE range){   
    uint8_t ctrlReg4 = read_Register(CTRL_REG4);    //Read current value
                                                    //of register 4.
    ctrlReg4 &= 0b11001111;             //Sets range bits to 0.
    ctrlReg4 |= ((uint8_t)range) << 4;  //Sets range bits to the given range.
    write_Register(CTRL_REG4, ctrlReg4); //Writes to register.
}
/*----------------------------------------------------------------------------*/
//The get_Range function returns the current set range. See Accel_Functions.h
/*----------------------------------------------------------------------------*/
LIS3DH_RANGE LIS3DH::get_Range(void){
    uint8_t curRange = (read_Register(CTRL_REG4)& 0b00110000); //Get range bits.
    curRange = curRange>>4;       //Shifts the data so it can be compared. 
    switch(curRange){   //SwitchCase for range.
        case LIS3DH_RANGE_2_G: return LIS3DH_RANGE_2_G; break;
        case LIS3DH_RANGE_4_G: return LIS3DH_RANGE_4_G; break;
        case LIS3DH_RANGE_8_G: return LIS3DH_RANGE_8_G; break;
        case LIS3DH_RANGE_16_G: return LIS3DH_RANGE_16_G; break;
        default: break;
    }
    return LISD3DH_RANGE_ERROR;
}
/*----------------------------------------------------------------------------*/
//The set_DataRate function sets the data acquisition speed
//of the LIS3DH accelerometer.
/*----------------------------------------------------------------------------*/
void LIS3DH::set_DataRate( LIS3DH_DATARATE set_Rate){
    uint8_t ctrlReg1 = //Sets DataRate bits to 0 Leaving the other bits alone.
            (read_Register(CTRL_REG1)&0b00000111);  
    ctrlReg1 |= set_Rate; //Sets the DataRate bits leaving the other bits alone.
    write_Register(CTRL_REG1,ctrlReg1); //Writes to register.       
}
/*----------------------------------------------------------------------------*/
//The get_DataRate function gets the current data acquisition speed of the
//LID3DH accelerometer.
/*----------------------------------------------------------------------------*/
LIS3DH_DATARATE LIS3DH::get_DataRate(void){
    uint8_t ctrlReg1 = //Gets the DataRate bits.
            (read_Register(CTRL_REG1)&0b11110000); 
    switch (ctrlReg1){ //Switchcase for DataRate.
    case LIS3DH_DATARATE_1_HZ: return LIS3DH_DATARATE_1_HZ; break;
    case LIS3DH_DATARATE_10_HZ: return LIS3DH_DATARATE_10_HZ; break;
    case LIS3DH_DATARATE_25_HZ: return LIS3DH_DATARATE_25_HZ; break;
    case LIS3DH_DATARATE_50_HZ: return LIS3DH_DATARATE_50_HZ; break;
    case LIS3DH_DATARATE_100_HZ: return LIS3DH_DATARATE_100_HZ; break;
    case LIS3DH_DATARATE_200_HZ: return LIS3DH_DATARATE_200_HZ; break;
    case LIS3DH_DATARATE_400_HZ: return LIS3DH_DATARATE_400_HZ; break;
    default: break;    
    }
    return LIS3DH_DATARATE_ERROR;
}

bool LIS3DH::acceleration_Available(void){
    if(read_Register(STATUS_REG_AUX) & 8){
        return true;
    }
    return false;
}