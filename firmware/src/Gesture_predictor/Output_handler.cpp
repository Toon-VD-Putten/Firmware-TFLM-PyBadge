#include "definitions.h"
#include "Output_handler.hpp"

void handleOutput(int gesture_index){
    switch(gesture_index){
      case 0:
          printf("WING WING WING WING WING WING WING\r\n");
          printf("WING WING WING WING WING WING WING\r\n");
          printf("WING WING WING WING WING WING WING\r\n");
          
          vTaskDelay(1000 / portTICK_PERIOD_MS);
          break;
      case 1:
          printf("RING RING RING RING RING RING RING\r\n");
          printf("RING RING RING RING RING RING RING\r\n");
          printf("RING RING RING RING RING RING RING\r\n");
          
          
          vTaskDelay(1000 / portTICK_PERIOD_MS);
          break;
      case 2:
          printf("SLOPE SLOPE SLOPE SLOPE SLOPE SLOPE\r\n");
          printf("SLOPE SLOPE SLOPE SLOPE SLOPE SLOPE\r\n");
          printf("SLOPE SLOPE SLOPE SLOPE SLOPE SLOPE\r\n");
          
          vTaskDelay(1000 / portTICK_PERIOD_MS);
          break;
      case 3:
          printf("UNKNOWN UNKNOWN UNKNOWN UNKNOWN UNKNOWN\r\n");
          
          break;
      default:
          break;   
  }
    return;
}

