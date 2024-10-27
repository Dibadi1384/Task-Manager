#ifndef _CPU_HPP_
#define _CPU_HPP_

#include <iostream>
#include <string>

#include "Core.hpp"


class CPU {
  public:
    // Constructor
    CPU();

    // Destructor
    ~CPU();

    //Public member Functions
    void on(int N);
    void spawn(int P_ID);
    void run(int C_ID);
    void sleep(int C_ID);
    void shutdown();
    void size(int C_ID);
    void capacity(int C_ID);
    void exit();

  private:
    //Member Variables
    int Numcores=0;
    Core** CPU_Inst=nullptr;

    //Member Functions
    Core* core_w_leastwork(); //returns a pointer to the core with lowest amound of work
    Core* core_w_mostwork();
    void pop(Core* core, std::string x); //x is either front or back
    void push(Core* core, int P_ID);


};
#endif 