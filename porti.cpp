#include <iostream>
#include <stdint.h>


int is_big_endian(void)
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1; 
}

uint32_t swapByteOrder(uint32_t ui)
{
    ui = (ui >> 24) |
         ((ui<<8) & 0x00FF0000) |
         ((ui>>8) & 0x0000FF00) |
         (ui << 24);
    return ui;
}

uint32_t read_packet_length(std::istream& s)
{
    uint32_t len;
    s.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (!is_big_endian()) 
        len = swapByteOrder(len);
    std::cerr << 'r' << len << ' ';
    std::cerr.flush();
    return len;
}

std::ostream&  write_packet_length(std::ostream& s, uint32_t len)
{
    std::cerr << 'w' << len << ' ';
    std::cerr.flush();

    if (!is_big_endian()) 
        len = swapByteOrder(len);
    s.write(reinterpret_cast<char*>(&len), sizeof(len));
    return s;
}



int main(void)
{
    // Helps to detect std::cin closing
    std::cin.exceptions ( std::istream::failbit | std::istream::badbit );

    // Read buffer's length
    while(true)
    {
        // read len, 4 bytes
        uint32_t len = read_packet_length(std::cin);

        // read data, len bytes
        char* buf = new char[len];
        std::cin.read(buf, len);

        write_packet_length(std::cout, len);
        std::cout.write(buf, len);
        std::cout.flush();
        delete[] buf;
    }
    return 0;
}
