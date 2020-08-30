#pragma once
#include "asmfunc.hpp"

struct pciDevice {
    unsigned char bus;
    unsigned char device;
    unsigned char func;

    unsigned short vendorID;
    unsigned short deviceID;

    unsigned char baseClass;
    unsigned char subClass;
    unsigned char interface;
};

class pciManager {
private:
    pciDevice *devices;
public:
    pciManager();
    unsigned short PCIConfigReadWord(unsigned char, unsigned char, unsigned char, unsigned char);
    unsigned short PCICheckVendor(unsigned char, unsigned char);
};