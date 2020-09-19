namespace serial_port {
    extern unsigned char inb(unsigned short);

    extern void outb(unsigned short, unsigned char);
 
    extern void init_serial();

    extern int is_transmit_empty();
 
    extern void write_serial(char a);
}