//-----------------
//  return values
//-----------------

#include <cstdint>

#ifndef RETURN_DOT_H
#define RETURN_DOT_H

typedef int32_t ret_t;

const ret_t RET_OK                   = 0;
const ret_t RET_GENERAL_ERROR        = 1;
const ret_t RET_BAD_PARAMETER_ARG_1  = 2;
const ret_t RET_BAD_PARAMETER_ARG_2  = 3;
const ret_t RET_BAD_PARAMETER_ARG_3  = 4;
const ret_t RET_BAD_PARAMETER_ARG_4  = 5;
const ret_t RET_ACCEPT               = 6;
const ret_t RET_REJECT               = 7;
const ret_t RET_DONE                 = 8;

#endif //RETURN_DOT_H
