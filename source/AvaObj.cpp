#include "../include/AvaObj.h"

AvaObj::AvaObj(unsigned short attribute_id, unsigned short len, unsigned short val) :
    attribute_id{attribute_id}, length{len}, attribute_val{val}
{

} 