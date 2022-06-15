#include "Accel_Handler.hpp"
#include "Accel_Functions.hpp"
#include "definitions.h"
#include <cstring>
#include <cmath>


LIS3DH lis3dh;

// A buffer holding the last 200 sets of 3-channel values
float save_data[600] = {0.0};
// Most recent position in the save_data buffer
int begin_index = 0;
// True if there is not yet enough data to run inference
bool pending_initial_data = true;
int sample_every_n = 0;
int kTargetHz = 25;
bool new_Data;
uint16_t buffer_Index = 0;

int count = 0;

TfLiteStatus SetupAccelerometer(tflite::ErrorReporter* error_reporter){
    lis3dh.begin();
    lis3dh.set_Range(LIS3DH_RANGE_4_G);
    lis3dh.set_DataRate(LIS3DH_DATARATE_25_HZ);  
  // Check if accelerometer is alive.
    if (!lis3dh.get_DeviceID()){
        error_reporter->Report("No accelerometer detected");
        return kTfLiteError;
    }

  // Determine how many measurements to keep in order to meet kTargetHz.
    //For sample rate = 25 and kTargetHz = 25, sample_every_n = 1
    sample_every_n = static_cast<int>(roundf(25 / kTargetHz)); //kTargetHz
    error_reporter->Report("SetupAccelerometer complete!");
    return kTfLiteOk;
}

bool GetData(tflite::ErrorReporter* error_reporter, float* input, int length, bool should_Clear_Buffer){
    if (should_Clear_Buffer) {
        memset(save_data, 0, 300 * sizeof(float));
        begin_index = 0;
        pending_initial_data = true;
        printf("BUFFER RESET:\r\n");
    }
    
    bool new_Data = false;
    
    while(INTERUPT_IN_Get()){
        SIG_OUT_Toggle();
          
        lis3dh.read_All_Scaled();
       
        float temp[3] = {0,0,0};
        float x,y,z;
        
        temp[0] = lis3dh.z_g;
        temp[1] = lis3dh.x_g;
        temp[2] = lis3dh.y_g;
        
        
        x = temp[0];
        y = temp[1];
        z = temp[2];
        
        x *= -1;
        y *= -1;
        
        //printf("%.3d\t%.3d\t%.3d\r\n",(int)(x*100),(int)(y*100),(int)(z*100));
        
        save_data[begin_index++] = x * 1000;
        save_data[begin_index++] = y * 1000;   
        save_data[begin_index++] = z * 1000;       
        
        if(begin_index >= 600){ //600/3 = 200 samples.
            //printf("INDEX RESET\r\n");
            begin_index = 0;
        }
        new_Data = true;
    }
    if (!new_Data) {
        //printf("DATA NOT READY YET\r\n");
        return false;
    }
    if (pending_initial_data && begin_index >= 200) {
        //printf("INITIAL DATA PENDING:\r\n");
        pending_initial_data = false;
    }
    if (pending_initial_data) {
        //printf("NOT ENOUGH DATA\r\n");  
        return false;
    }   
    //printf("WRITING TO INPUT TENSOR \r\n");
    for (int i = 0; i < length; ++i) {
        int ring_array_index = begin_index + i - length;
        if (ring_array_index < 0) {
          ring_array_index += 600;
        }
        
        input[i] = save_data[ring_array_index];
      }
    return true;
}

