#ifndef _Core_HPP_
#define _Core_HPP_

#include <iostream>
#include <string>

class Core {
  public:
    // Constructor
    Core(int index);

    // Destructor
    ~Core();

    //Public variables
    int C_ID, capacity, size;
    int lastTask=-1;
    int firstTask=0;
    int* queue=nullptr;

    void resize_queue();
};
#endif 