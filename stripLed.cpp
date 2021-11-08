/*    
    Copyright (c) 2021 EIIT S.A.

    @file          stripLed.cpp
    @project       ATE.2000210
    @version       2.2.0
    @date          July 2021
    @author        Rachid Souky
*/

#include "stripLed.h"

/***** StripLed frames *****/

//Enter Factory Mode
extern const char stl_factMode[10] = {0x00, 0xC4, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//Set RGB PWM
extern const char stl_rgbPWM[40] = {
    0x01, 0xFC,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00};
    
//Set RGB LUV
extern const char stl_rgbLUV[40] = {
    0x01, 0xFC,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00};

//Set Sync
extern const char stl_sync[10] = {0x00, 0x84, 0x00, 0x19, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00};

//Enter Programming Mode
extern const char stl_progMode[10] = {0x00, 0xE7, 0x00, 0xFE, 0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x00};

//Move Memory Pointer
extern const char stl_moveMemPointer[10] = {0x00, 0xE7, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//Read EEPROM
extern const char stl_readEEPROM[10] = {0x00, 0xE7, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//Read Response (12 Bytes)
extern const char stl_readResponse12[2] = {0x02, 0xEE};

//Module Data Response
extern const char stl_modData_0x4[14] = {0x0D, 0x32, 0x00, 0x00, 0x00, 0x37, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x10, 0x05};
extern const char stl_modData_0x5[14] = {0x0D, 0x32, 0x00, 0x00, 0x00, 0x37, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x85, 0xD8};
extern const char stl_modData_0x6[14] = {0x0D, 0x32, 0x00, 0x00, 0x00, 0x37, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x3D, 0x63};
extern const char stl_modData_0x7[14] = {0x0D, 0x32, 0x00, 0x00, 0x00, 0x37, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x31, 0x28};
extern const char stl_modData_0x8[14] = {0x0D, 0x32, 0x00, 0x00, 0x00, 0x37, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0xD2, 0xAB};
extern const char stl_modData_0x9[14] = {0x0D, 0x32, 0x00, 0x00, 0x00, 0x37, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x47, 0x76};
extern const char stl_modData_0xA[14] = {0x0D, 0x32, 0x00, 0x00, 0x00, 0x37, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0xFF, 0xCD};
extern const char stl_modData_0xB[14] = {0x0D, 0x32, 0x00, 0x00, 0x00, 0x37, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0xD0, 0x8B};
extern const char stl_modData_0xC[14] = {0x0D, 0x32, 0x00, 0x00, 0x00, 0x37, 0x09, 0x09, 0x09, 0x09, 0x00, 0x00, 0x24, 0x01};
extern const char stl_modData_0xD[14] = {0x0D, 0x32, 0x00, 0x00, 0x00, 0x37, 0x0A, 0x0A, 0x0A, 0x0A, 0x00, 0x00, 0xB1, 0xDC};

/***** StripLed functions *****/

void compose_rgbPWM100_frame(char *array, unsigned int arraySize,
    unsigned int slaveAdd, unsigned int ledNum, unsigned int rgbVal) {

    if(ledNum != 0) {
        if(rgbVal != 0) {
            int index = 0;
            index = 2+(6*(ledNum-1));       //Move index to corresponding Led
            if(rgbVal != 4) {
                index = index+(2*(rgbVal-1));   //Move index to corresponding RGB
                *(array+index) = 0x7F;
                *(array+index+1) = 0x00;
            }
            else if(rgbVal == 4) {
                for(int i=0; i<3; i++) {
                    *(array+index+(2*i)) = 0x7F;
                    *(array+index+(2*i)+1) = 0x00;
                }
            }
        }
    }
    else if(ledNum == 0) {
        if(rgbVal != 0) {
            if(rgbVal != 4) {
                int index = 0;
                index = 2+(2*(rgbVal-1));
                for(int i=0; i<6; i++) {
                    *(array+index) = 0x7F;
                    *(array+index+1) = 0x00;
                    index = index+6;
                }
            }
            else if(rgbVal == 4) {
                for(int i=0; i<18; i++) {
                    *(array+2+(2*i)) = 0x7F;
                    *(array+2+(2*i)+1) = 0x00;
                }
            }
        }
    }
    
    compose_melibu_full_frame(array, arraySize, slaveAdd);
}

void compose_rgbLUV100_frame(char *array, unsigned int arraySize,
    unsigned int slaveAdd, unsigned int ledNum, unsigned int L, unsigned int u, unsigned int v) {
        
    if(ledNum != 0) {
        int index = 0;
        index = 2+(6*(ledNum-1));       //Move index to corresponding Led
        *(array+index) = (char)((0xFF00 & L)>>8);
        *(array+index+1) = (char)(0x00FF & L);
        *(array+index+2) = (char)((0xFF00 & u)>>8);
        *(array+index+3) = (char)(0x00FF & u);
        *(array+index+4) = (char)((0xFF00 & v)>>8);
        *(array+index+5) = (char)(0x00FF & v);
    }
    else if(ledNum == 0) {       
        int index = 2;
        for(int i=0; i<6; i++) {
            *(array+index) = (char)((0xFF00 & L)>>8);
            *(array+index+1) = (char)(0x00FF & L);
            *(array+index+2) = (char)((0xFF00 & u)>>8);
            *(array+index+3) = (char)(0x00FF & u);
            *(array+index+4) = (char)((0xFF00 & v)>>8);
            *(array+index+5) = (char)(0x00FF & v);
            index = index+6;            
        }
    }
    compose_melibu_full_frame(array, arraySize, slaveAdd);
}

void compose_memPointer_frame(char *array, unsigned int arraySize, unsigned int slaveAdd, unsigned int memAddress) {
    
    *(array+4) = (char)((0xFF00 & memAddress)>>8);
    *(array+5) = (char)(0x00FF & memAddress);
    
    compose_melibu_full_frame(array, arraySize, slaveAdd);
}