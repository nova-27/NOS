#include "pciManager.hpp"

pciManager::pciManager() {
  
}

/* PCI */
unsigned short pciManager::PCIConfigReadWord(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset)
{
  unsigned int address;
  unsigned int lbus = (unsigned int)bus;
  unsigned int lslot = (unsigned int)slot;
  unsigned int lfunc = (unsigned int)func;
  unsigned short tmp = 0;

  /* コンフィギュレーションアドレスを作成 */
  address = (unsigned int)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((unsigned int)0x80000000));

  /* アドレスの書き出し*/
  io_out32(0xCF8, address);
  /* データの読み込み */
  tmp = (unsigned short)((io_in32(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
  return(tmp);
}

unsigned short pciManager::PCICheckVendor(unsigned char bus, unsigned char slot)
{
  unsigned short vendor, device;
  /* 最初のコンフィギュレーションを読み込むテスト */
  /* ベンダーなし(0xFFFF)の場合、デバイスは存在しないことになる */
  if((vendor = PCIConfigReadWord(bus, slot, 0, 0)) != 0xFFFF)
  {
    device = PCIConfigReadWord(bus, slot, 0, 2);
  }
  return(device);
}