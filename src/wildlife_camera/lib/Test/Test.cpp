#include "Test.h"
#include <Arduino.h>

int test_square(int n) {
    Serial.println("Test square function");
    return n * n;
}