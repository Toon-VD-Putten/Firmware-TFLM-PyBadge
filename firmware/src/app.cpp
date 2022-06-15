/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "definitions.h"
#include "Gesture_predictor/TF_Functions.hpp"
#include "Gesture_predictor/Accel_Functions.hpp"


APP_DATA appData;
LIS3DH LIS3DH;

//static int16_t buffer[600] = {0};
//uint16_t buffer_Index = 0;
//static int16_t notbuffer[600] = {0};
//
//bool buffer_Full = false;
//bool buffer_Looped = false;
//bool buffer_Ready = false;
//bool reset_Buffer = false;
//bool new_Data = false;
//bool pending_initial_data = false;
//int length = 200;

void APP_Initialize ( void )
{
    appData.state = APP_STATE_INIT;   
}
/******************************************************************************/

void APP_Tasks ( void )
{
   volatile bool setup = 0;
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            printf("Starting program\r\n");
//            LIS3DH.begin();
//            LIS3DH.set_Range(LIS3DH_RANGE_4_G);
//            LIS3DH.set_DataRate(LIS3DH_DATARATE_25_HZ);
            TF_Initialize();
            //vTaskDelay(500 / portTICK_PERIOD_MS);
            bool appInitialized = true;
            if (appInitialized)
            {           
             appData.state = APP_STATE_PREFORM_INFERENCE;   
            }
            break;
        }
        
        case APP_STATE_FILL_BUFFER:
        {
            break;
        }
        
        
        case APP_STATE_PREFORM_INFERENCE:
        {
            TF_PreformInference();
            break;
        }
         
        case APP_STATE_SELFTEST:
        {  
        }
        
        /* The default state should never be executed. */
        default:
        {
            //appData.state = APP_STATE_INIT;
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
