#pragma once

class AvaObj
    {
        public:
            AvaObj(unsigned short attribute_id= 0, unsigned short length= 0, unsigned short attribute_val= 0);
            unsigned short attribute_id;
            unsigned short length;
            unsigned short attribute_val; //placeholder
    };
