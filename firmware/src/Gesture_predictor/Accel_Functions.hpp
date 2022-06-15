#ifndef Accel_Functions_HPP    /* Guard against multiple inclusion */
#define Accel_Functions_HPP

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {}
#endif


#define SCL_CLOCK_FREQUENCY 100Khz
#define LIS3DH_ADDRESS 0x18
#define range_Bitmask 00110000                               
/*----------------------------------------------------------------------------*/
//LIS3DHH REGISTER ADDRESES
/*----------------------------------------------------------------------------*/  
///Status register 
#define STATUS_REG_AUX 0x07 //(321OR 3OR 2OR 1OR 321DA 3DA 2DA 1DA)
/*
 * DEFAULT (00000000)
 * 1, 2, 3 axis overwritten
 * 3 axis overwritten
 * 2 axis overwritten
 * 1 axis overwritten
 * 1,2,3 axis new data available
 * 3 axis new data available
 * 2 axis new data available
 * 1 axis new data available
 */
 
/*----------------------------------------------------------------------------*/
//8 BIT ACCELEROMETER DATA REGISTERS
/*----------------------------------------------------------------------------*/    
//1-axis acceleration data 2's complement  
#define OUT_ADC1_L 0x08 //low byte
#define OUT_ADC1_H 0x09 //high byte
//2-axis acceleration data 2's complement
#define OUT_ADC2_L 0x0A //low byte
#define OUT_ADC2_H 0x0B //high byte
//3-axis acceleration data 2's complement
#define OUT_ADC3_L 0x0C //low byte
#define OUT_ADC3_H 0x0D //high byte
  
#define INT_COUNTER_REG 0x0E //(0b00000000) (C7 IC6 IC5 IC4 IC3 IC2 IC1 IC0) 
#define WHO_AM_I 0x0F //Device identification register (00110011)  
#define TEMP_CFG_REG 0x1F //Enables/disables the ADC and or Temp sensor
//Default:      00000000   
//ADC-enabled:  10000000
//T-enabled:   01000000 
/*----------------------------------------------------------------------------*/
//CTRL REGISTERS
/*----------------------------------------------------------------------------*/
#define CTRL_REG0 0x1E
#define CTRL_REG1 0x20  //(ODR3, ODR2, ODR1, ODR0, LPen, Zen, Yen, Xen)
#define CTRL_REG2 0x21  //(HPM1, HPM0, HPCF2, HPCF1 ,FDS ,HPCLICK ,HPIS2 ,HPIS1)
#define CTRL_REG3 0x22  //(I1_CLICK, I1_AOI1, I1_AOI2, I1_DRDY1, I1_DRDY2, I1_WTM, I1_OVERRUN, --)
#define CTRL_REG4 0x23  //(BDU, BLE, FS1, FS0, HR, ST1, ST0, SIM)
#define CTRL_REG5 0x24  //(BOOT, FIFO_EN, --, --, LIR_INT1, D4D_INT1, 0, 0)
#define CTRL_REG6 0x25  //(I2_CLICKen, I2_INT1, 0, BOOT_I1, 0, --, H_LACTIVE, --)
 
#define REFERENCE 0x26 //Reference value for Interrupt generation. Default value: 0 
#define STATUS_REG 0x27    //(ZYXOR, ZOR, YOR, XOR, ZYXDA, ZDA, YDA, XDA)

#define OUT_X_L 0x28    // X-axis data lowbyte
#define OUT_X_H 0x29    // X-axis data highbyte
#define OUT_Y_L 0x2A    // Y-axis data lowbyte
#define OUT_Y_H 0x2B    // Y-axis data highbyte
#define OUT_Z_L 0x2C    // Z-axis data lowbyte
#define OUT_Z_H 0x2D    // z-axis data highbyte
    
#define FIFO_CTRL_REG 0x2E 
#define FIFO_SRC_REG 0x2F 

/*----------------------------------------------------------------------------*/
//Interupt registers
/*----------------------------------------------------------------------------*/
#define INT1_CFG 0x30           //Interupt setup register(AOI, 6D, ZHIE/ZUPE, ZLIE/ZDOWNE, YHIE/YUPE, YLIE/YDOWNE, XHIE/XUPE, XLIE/XDOWNE)
#define INT1_SOURCE     0x31    //Interupt source register (0 IA ZH ZL YH YL XH XL)
#define INT1_THS        0x32    //Interupt threshold register ()
#define INT1_DURATION   0x33    //Interupt duration register

#define CLICK_CFG   0x38 
#define CLICK_SRC   0x39 
#define CLICK_THS   0x3A 

#define TIME_LIMIT      0x3B 
#define TIME_LATENCY    0x3C 
#define TIME_WINDOW     0x3D 


#define LIS3DH_LSB16_TO_KILO_LSB10 64000

#define GRAVITY_EARTH (9.80665F) 

 /** A structure to represent scales **/
typedef enum { //CTRL_REG4
  LIS3DH_RANGE_16_G = 0x03, // +/- 16g
  LIS3DH_RANGE_8_G  = 0x02,  // +/- 8g
  LIS3DH_RANGE_4_G  = 0x01,  // +/- 4g
  LIS3DH_RANGE_2_G  = 0x00,   // +/- 2g (default value)
  LISD3DH_RANGE_ERROR = 0xff
} LIS3DH_RANGE;

/** A structure to represent axes **/
typedef enum {
  LIS3DH_AXIS_X = 0x0,
  LIS3DH_AXIS_Y = 0x1,
  LIS3DH_AXIS_Z = 0x2,
} LIS3DH_AXIS;

/** Used with register 0x20 (CTRL_REG1) to set bandwidth **/
typedef enum {
  LIS3DH_DATARATE_400_HZ =  0b01110000, //  400Hz
  LIS3DH_DATARATE_200_HZ =  0b01100000, //  200Hz
  LIS3DH_DATARATE_100_HZ =  0b01010000, //  100Hz
  LIS3DH_DATARATE_50_HZ =   0b01000000,  // 50Hz
  LIS3DH_DATARATE_25_HZ =   0b00110000,  // 25Hz
  LIS3DH_DATARATE_10_HZ =   0b00100000,  // 10 Hz
  LIS3DH_DATARATE_1_HZ  =    0b00010000, // 1 Hz
  LIS3DH_DATARATE_POWERDOWN = 0b00000000,
  LIS3DH_DATARATE_ERROR = 0,        
} LIS3DH_DATARATE;   

/*----------------------------------------------------------------------------*/
//Function definitions
/*----------------------------------------------------------------------------*/

class LIS3DH{
public: 
    void begin();
    uint8_t get_DeviceID(void); 
    
    void set_Range(LIS3DH_RANGE range);
    LIS3DH_RANGE get_Range(void);
    
    void set_DataRate(LIS3DH_DATARATE);   
    LIS3DH_DATARATE get_DataRate(void);
    
    int16_t read_X_Accel_Raw(void);
    int16_t read_Y_Accel_Raw(void);
    int16_t read_Z_Accel_Raw(void);
    
    void read_All_Raw(void);
    void read_All_Scaled(void);   
    
    bool acceleration_Available(void);
    
    void write_Register(uint8_t reg, uint8_t value);
    uint8_t read_Register(uint8_t reg);

    int16_t x_r; /**< x axis value */
    int16_t y_r; /**< y axis value */
    int16_t z_r; /**< z axis value */

    float x_g; /**< x_g axis value (calculated by selected range) */
    float y_g; /**< y_g axis value (calculated by selected range) */
    float z_g; /**< z_g axis value (calculated by selected range) */
    
    int16_t x_mg; /**< x_g axis value (calculated by selected range) */
    int16_t y_mg; /**< y_g axis value (calculated by selected range) */
    int16_t z_mg; /**< z_g axis value (calculated by selected range) */ 
    
    uint8_t SAMPLE = 0;
    
private:
    void read_I2R(uint8_t* rdData, uint32_t rdLength);
    void write_I2R(uint8_t* wrData, uint32_t wrLength);
    void write_Read(uint8_t* wrData, uint32_t wrLength, uint8_t* rdData, uint32_t rdLength);
};

    /* Provide C++ Compatibility */
#ifdef __cplusplus

#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
