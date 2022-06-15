/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef Accel_Handler_HPP    /* Guard against multiple inclusion */
#define Accel_Handler_HPP

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"

/* TODO:  Include other files here if needed. */

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#define kChannelNumber 3
    
extern int begin_index;

TfLiteStatus SetupAccelerometer(tflite::ErrorReporter* error_reporter);

bool GetData(tflite::ErrorReporter* error_reporter, float* input, int length, bool reset_Buffer);



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
