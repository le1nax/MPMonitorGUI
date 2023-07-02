#include "../include/AvaObj.h"

AvaObj::AvaObj(unsigned short attribute_id = 0, unsigned short len= 0, unsigned short val= 0) :
    attribute_id{attribute_id}, length{len}, attribute_val{val}
{

} 