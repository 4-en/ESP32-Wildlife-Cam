#include "Test.h"
#include <Arduino.h>
// tensorflow lite
//#include <TensorFlowLite_ESP32.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>

int test_square(int n) {
    Serial.println("Test square function");
    return n * n;
}
