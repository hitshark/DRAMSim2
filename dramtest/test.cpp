#include <iostream>
#include "DRAMSim.h"

class Consumer
{
    public:
        void read_act(unsigned id, uint64_t addr, uint64_t cycle){
            std::cout<<"read callback at address = "<<addr<<", cycle = "<<cycle<<std::endl;
        }
        void write_act(unsigned id, uint64_t addr, uint64_t cycle){
            std::cout<<"write callback at address = "<<addr<<", cycle = "<<cycle<<std::endl;
        }
};

int main()
{
    typedef DRAMSim::MultiChannelMemorySystem mem_t;

    string dev = "ini/DDR3_micron_32M_8B_x8_sg15.ini";
    string sys = "system.ini.example";
    string pwd = "..";
    string logout = "./result";

    mem_t *mem = DRAMSim::getMemorySystemInstance(dev, sys, pwd, "", 2048, &logout);

    mem -> setCPUClockSpeed(0);

    Consumer consumer;
    DRAMSim::Callback_t *read_cb = new DRAMSim::Callback<Consumer, void, unsigned, uint64_t, uint64_t>(&consumer, &Consumer::read_act);
    DRAMSim::Callback_t *write_cb = new DRAMSim::Callback<Consumer, void, unsigned, uint64_t, uint64_t>(&consumer, &Consumer::write_act);
    mem->RegisterCallbacks(read_cb, write_cb, NULL);

    bool read_or_write = false;
    for(uint64_t addr = 0x10000000; addr < 0x20000000; addr+=0x40){
        read_or_write = !read_or_write;
        if(mem->willAcceptTransaction(addr)) mem->addTransaction(read_or_write, addr);
        mem->update();
    }

    return 0;
}
