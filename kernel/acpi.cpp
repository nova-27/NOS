#include "acpi.hpp"

namespace acpi {
    //RSDPの構造
    struct __attribute__((packed)) RSDP {
        char Signature[8];
        unsigned char Checksum;
        char OEMID[6];
        unsigned char Revision;
        unsigned int RsdtAddress;
        unsigned int Length;
        unsigned long long XsdtAddress;
        unsigned char Extended_Checksum;
        unsigned char Reserved[3];
    };

    //XSDTの構造
    struct __attribute__((packed)) XSDT {
        struct SDTH Header;
        struct SDTH *Entry[0];
    };

    struct XSDT *xsdt;
    int num_sdts;

    //初期化処理
    void init(void *rsdp) {
        xsdt = (struct XSDT *)((struct RSDP *)rsdp)->XsdtAddress;
        num_sdts = (xsdt->Header.Length - sizeof(struct SDTH)) / sizeof(struct SDTH *);
    }

    struct SDTH *get_sdt(char *sig) {
        for (int i = 0; i < num_sdts; i++) {
            bool is_equal = true;
            for(int j = 0; j < 4; j++) {
			    if(sig[j] != xsdt->Entry[i]->Signature[j]) is_equal = false;
            } 
            if(is_equal) {
                return xsdt->Entry[i];
            }
		}

        NULL;
    }
}