#include <iostream>
#include <string>
#include "Core.hpp"
#include "CPU.hpp"

////////////////////////CPU Class
CPU::CPU(){}

CPU::~CPU(){
    for(int i=0; i<Numcores; i++){
        delete CPU_Inst[i];  
        CPU_Inst[i] = nullptr;
    }

    delete[] CPU_Inst;
    CPU_Inst = nullptr;
}

//////helper functions
/*
    CITATION:
    The algorithms for core_w_leastwork() and core_w_mostwork() were derived using ChatGPT. Although I already
    had an algorithm written for these functions, the suggestion from ChatGPT was much shorter and efficient, therefore
    I decided to use it instead of what I previously had. Lines 29-46.
*/
Core* CPU::core_w_leastwork(){
    int index = 0; //start by assuming the first core has the least work
    for (int i = 1; i < Numcores; ++i) {
        if (CPU_Inst[i]->size < CPU_Inst[index]->size) {
            index = i; //update index to the core with smaller size, smaller index
        }
    }    
    return CPU_Inst[index];
}

Core* CPU::core_w_mostwork(){
    int index = 0; //start by assuming the first core has the least work, smaller index
    for (int i = 1; i < Numcores; ++i) {
        if (CPU_Inst[i]->size > CPU_Inst[index]->size) {
            index = i; //update index to the core with bigger size
        }
    }
    return CPU_Inst[index];
}

void CPU::pop(Core* core, std::string x){
    if(core->size==((core->capacity)/4)){
        core->resize_queue();
    }
    if(x=="Front"){//if popped from front, the first index of the array is incremented by 1 
       core->firstTask+=1;
    }
    else if(x=="Back"){//if popped from back, the last index of array is decremented by 1
        core->lastTask-=1;
    }
    core->size-=1;
}

void CPU::push(Core* core, int P_ID){
    if(core->size==core->capacity){
        core->resize_queue();
    }
    core->lastTask+=1;
    core->queue[core->lastTask]=P_ID;
    core->size+=1;
}

///Command Fucntions
void CPU::on(int N){
    if(CPU_Inst!=NULL){ //if CPU_Inst alr exists
        std::cout << "failure" << std::endl;
        return; 
    }

    //allocating  CPU Instance with N number of cores
    Numcores=N;
    CPU_Inst= new Core*[Numcores];
    for(int i = 0; i < Numcores; ++i) {
        CPU_Inst[i]=new Core(i); 
        CPU_Inst[i]->queue=new int[4];
        CPU_Inst[i]->capacity=4;
        CPU_Inst[i]->size=0;

    }
    std::cout << "success" << std::endl;
 
}

void CPU::spawn(int P_ID){
    if(P_ID>0){
        Core* core=core_w_leastwork();
        push(core, P_ID);
        std::cout << "core " << core->C_ID << " assigned task " << P_ID <<std::endl;
        return;
    }

    //invalid input
    std::cout << "failure" << std::endl; 
}

void CPU::run(int C_ID){
    if(C_ID<0 || C_ID>Numcores-1){
        std::cout << "failure" << std::endl;  //invalid input
        return;
    }
 
    //no tasks
    if(CPU_Inst[C_ID]->size==0){
        std::cout << "core "<< C_ID << " has no tasks to run " << std::endl;
    }

    //run task
    else{
        std::cout << "core "<< C_ID << " is running task "<< CPU_Inst[C_ID]->queue[CPU_Inst[C_ID]->firstTask] << std::endl; //task from front of queue
        pop(CPU_Inst[C_ID], "Front"); 
    }

    //assign it work from core with most tasks if size=0 after popping that task or core had no tasks
    if(CPU_Inst[C_ID]->size==0){
        Core* busycore=core_w_mostwork();
        if(core_w_mostwork()->size>0){ //only perform if the "busy core" has tasks 
            int stolenwork=busycore->queue[busycore->lastTask];//the work that will be replaced
            pop(busycore,"Back");//remove the work from the back of queue
            push(CPU_Inst[C_ID], stolenwork);//add it to this core's queue     
        }
    }
}

void CPU::sleep(int C_ID){
    if(C_ID<0 || C_ID>Numcores-1){
        std::cout << "failure" << std::endl;
        return; //invalid input
    }

    //core has no tasks
    else if(CPU_Inst[C_ID]->size==0){
        std::cout << "core "<< C_ID << " has no tasks to assign" << std::endl;
        return;
    }

    //distributing tasks  
    while(CPU_Inst[C_ID]->size>0){
        int task=CPU_Inst[C_ID]->queue[CPU_Inst[C_ID]->lastTask];//task from the back of queue
        Core* notbusycore= core_w_leastwork();

        //incase the core with the least amount of work is the same core going to sleep
        if(notbusycore->C_ID==C_ID){
            int newSmallestsize=notbusycore->size;//start by setting the new smallest size same notbusycore queue size
            bool findnewCore=false;

            while(findnewCore==false){
                for (int i=0; i<Numcores; i++){
                    if (CPU_Inst[i]->size==newSmallestsize && i!=C_ID){ //if the size is equal to the newsmallest size, and is NOT notbusycore 
                        findnewCore=true;
                        notbusycore=CPU_Inst[i];
                    }
                }
                newSmallestsize += 1;  //increase the size until a match is found       
            }
        }

        //remove tasks from core and assign it to core with least amount of work
        pop(CPU_Inst[C_ID], "Back");
        push(notbusycore, task); 

        std::cout << "task "<< task << " assigned to core "<< notbusycore->C_ID << " "; 
    } 
    std::cout << std::endl;     
}

void CPU::shutdown(){

    //first check if there is any tasks in CPU
    int totalnumTasks=0;
    for(int i=0; i<Numcores ; i++){
        totalnumTasks+=CPU_Inst[i]->size;
    }
    if(totalnumTasks==0){
        std::cout << "no tasks to remove" << std::endl;
        return;
    }

    //remove tasks from cores
    for(int i=0; i<Numcores ; i++){
        while(CPU_Inst[i]->size>0){
            std::cout << "deleting task "<< CPU_Inst[i]->queue[CPU_Inst[i]->firstTask] << " from core "<< i << " "; //called before next line since queue[j]=queue[j+1] after pop back
            pop(CPU_Inst[i],"Front");
        }
    }
    std::cout << std::endl;     
}

void CPU::size(int C_ID){
    if(C_ID<0 || C_ID>Numcores-1){
        std::cout << "failure" << std::endl;
    }
    else{
        std::cout << "size is "<< CPU_Inst[C_ID]->size << std::endl;
    }
}
void CPU::capacity(int C_ID){
    if(C_ID<0 || C_ID>Numcores-1){
        std::cout << "failure" << std::endl;
    }
    else{
        std::cout << "capacity is "<< CPU_Inst[C_ID]->capacity << std::endl;
    }
}

void CPU::exit(){
}

//////////////////////////Core Class
Core::Core(int index){
    C_ID=index;
}

Core::~Core(){
    delete[] queue;
    queue=nullptr;
}

void Core::resize_queue(){
    //we check if size=capacity, and if so we double capacity
    if(size==capacity){
        capacity=capacity*2;
    }

    //we check is size=1/4capacity, and if so half capacity
    else if(size<=(capacity/4)){
        capacity=capacity/2;    
    }

    //allocate new array
    int* new_queue=new int[capacity];
    int index_new_queue=0;
    for(int i=firstTask; i<lastTask+1; i++){
        new_queue[index_new_queue++]=queue[i];
    }

    delete[] queue;
    queue=new_queue;  
    firstTask=0;
    lastTask=size-1;
}
