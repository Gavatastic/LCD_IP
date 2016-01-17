//
// Created by ZhengnanLee on 15/6/26.
//

#include <string>

#ifndef I2C_1602I2C_H
#define I2C_1602I2C_H

static int fd;

class I2CLED{
public:
    I2CLED(int deviceID);
    void Backlight(bool key);
    void write_word(int fd, int data);
    void send_command(int command);
    void send_data(int data);
    void clear_lcd();
    void print_screen(int x, int y, std::string str);
	std::string cmd_system(const char* command);
};


#endif //I2C_1602I2C_H
