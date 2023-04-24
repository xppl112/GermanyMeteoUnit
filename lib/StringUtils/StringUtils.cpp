#include <Arduino.h>
#include "StringUtils.h"

String StringUtils::floatToString(float number, int widthAfterDot, int widthBeforeDot){
    return String(number, widthAfterDot);
}