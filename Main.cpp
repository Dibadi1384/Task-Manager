#include <iostream>
#include <string>
#include "Core.hpp"
#include "CPU.hpp"

int main(){
    CPU *c = new CPU();
    
    std::string  input;
    while (std::cin>>input){
        if (input == "ON"){
            int N;
            std::cin >> N ;
            c->on(N);
        } 
        else if (input == "SPAWN"){
            int P_ID;
            std::cin >> P_ID;
            c->spawn(P_ID);
        }
        else if (input == "RUN"){
            int C_ID;
            std::cin >> C_ID;
            c->run(C_ID);
        }
        else if (input == "SLEEP"){
            int C_ID;
            std::cin >> C_ID;
            c->sleep(C_ID);
        }
        else if (input == "SHUTDOWN"){
            c->shutdown();
        }
        else if (input == "SIZE"){
            int C_ID;
            std::cin >> C_ID;
            c->size(C_ID);
        }
        else if (input == "CAPACITY"){
            int C_ID;
            std::cin >> C_ID;
            c->capacity(C_ID);
        }
        else if (input == "EXIT"){
            c->exit();
        }
    }

    delete c;

    return 0;
}