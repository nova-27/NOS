#define PORT 0x3f8   /* COM1 */

namespace serial_port {
    unsigned char inb(unsigned short port) {
        /* バイト(8bit)サイズでI/Oポートから値を得る
        ** ここではインラインアセンブラ(AT&T記法)を用いた
        */
        unsigned char val;
    __asm__ __volatile__ (
            "inb %1, %0\n\t" : "=a" (val) : "d" (port)
        );
        return val;
    }

    void outb(unsigned short port, unsigned char val) {
        /* バイト(8bit)サイズでI/Oポートに値を出力	*/
    __asm__ __volatile__ (
            "outb %1, %0" : : "d" (port), "a" (val)
        );
    }
 
void init_serial() {
   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}
 
void write_serial(char a) {
   while (is_transmit_empty() == 0);
 
   outb(PORT,a);
}
}