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

#ifndef _TF_Functions_HPP    /* Guard against multiple inclusion */
#define _TF_Functions_HPP

/* Section: Included Files                                                    */
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"


#include "Gesture_predictor/Gesture_predictor.hpp"
#include "Gesture_predictor/Output_handler.hpp"
#include "Gesture_predictor/Accel_Handler.hpp"

/* TODO:  Include other files here if needed. */

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif



#define TENSOR_MEMORY 60 * 1024
#define DEBUG_FLAG 0
    
    
   
    void TF_Initialize(void);
    void TF_PreformInference(void);
    void TF_Main(void);
    

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
