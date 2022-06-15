#include "definitions.h"
#include "Output_handler.hpp"
#include "Gesture_predictor/TF_Functions.hpp"
#include "Gesture_predictor/gesture_model.h"
#include "Gesture_predictor/Gesture_predictor.hpp"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "magic_wand.h"

// Globals, used for compatibility with Arduino-style sketches.


namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* model_input_tensor = nullptr;
int input_length;

// Create an area of memory to use for input, output, and intermediate arrays.
// The size of this will depend on the model you're using, and may need to be
// determined by experimentation.
constexpr uint16_t kTensorArenaSize = TENSOR_MEMORY;
uint8_t tensor_arena[kTensorArenaSize];
} 

bool should_Clear_Buffer = false;

void TF_Initialize(void){
    
// Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  static tflite::MicroErrorReporter micro_error_reporter;  // NOLINT
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  
  model = tflite::GetModel(GESTURE_MODEL_tflite);
  
  //Version checks
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter, "Model provided is schema version %d not equal " 
            "to supported version %d.", model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // Loads in the operations needed to run inference on the model
  // Use "AllOpsResolver" to load in all possible operations
  static tflite::MicroMutableOpResolver<5> micro_op_resolver;  // NOLINT
  micro_op_resolver.AddConv2D();
  micro_op_resolver.AddDepthwiseConv2D();
  micro_op_resolver.AddFullyConnected();
  micro_op_resolver.AddMaxPool2D();
  micro_op_resolver.AddSoftmax();
  
  
  //static tflite::AllOpsResolver micro_op_resolver;
  
  
  // Sets up an interpreter object.
  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  interpreter->AllocateTensors();

  // Obtain pointer to the model's input tensor.
  model_input_tensor = interpreter->input(0);

  //Error checking stuffs
  if ((model_input_tensor->dims->size != 4) || (model_input_tensor->dims->data[0] != 1) ||
      (model_input_tensor->dims->data[1] != 128) ||
      (model_input_tensor->dims->data[2] != kChannelNumber) ||
      (model_input_tensor->type != kTfLiteFloat32)) { 
    TF_LITE_REPORT_ERROR(error_reporter, "Bad input tensor parameters in model");
    return;
  }
  
  //Determine the input length of the input tensor //128 samples
  input_length = model_input_tensor->bytes / sizeof(float);
 
  //Setup the acceleromoter.
  TfLiteStatus setup_status = SetupAccelerometer(error_reporter);
  if (setup_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "Set up failed\n");
  }
  
  
}

void TF_PreformInference(void){
      
  bool got_Data = GetData(error_reporter, model_input_tensor->data.f, input_length,should_Clear_Buffer);
  should_Clear_Buffer = false;
  
  if (!got_Data){
    //printf("!got_Data\r\n");
    return;
  }
  
  // Run inference, and report any error.
  TfLiteStatus invoke_status = interpreter->Invoke();
        
    if (invoke_status != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on index: %d\n", begin_index);
        return;
    }
  
  float* model_Output = 0;
  
  model_Output = interpreter->output(0)->data.f;
     //uint8_t TEST[3] = {(model_Output[0]*100) ,(model_Output[1]*100),(model_Output[2]*100)};
      //printf("O1: %.3d O2: %.3d O3: %.3d\r\n",TEST[0],TEST[1],TEST[2]);

    //vTaskDelay(200 / portTICK_PERIOD_MS);
  
  int gesture_index = PredictGesture(interpreter->output(0)->data.f);
  
  //printf("%d\r\n", gesture_index);
  
  should_Clear_Buffer = gesture_index < 3;

  handleOutput(gesture_index);
  
}