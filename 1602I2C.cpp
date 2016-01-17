//
// Created by ZhengnanLee on 15/6/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <string>

#include "1602I2C.h"

bool BLEN = true; // BackLight ENable

I2CLED::I2CLED(int deviceID) {
    fd = wiringPiI2CSetup(deviceID);

//    wiringPiI2CWriteReg8 (fd, 0x0a, 0x84) ;  // IOCON - set BANK bit
//    wiringPiI2CWriteReg8 (fd, 0x05, 0x84) ;  // IOCON - set ODR in bank 0
//    wiringPiI2CWriteReg8 (fd, 0x00, 0x00) ;  // Port A -> Outputs

    send_command(0x33); // Must initialize to 8-line mode at first
    usleep(5000);
    send_command(0x32); // Then initialize to 4-line mode
    usleep(5000);
    send_command(0x28); // 2 Lines & 5*7 dots
    usleep(5000);
    send_command(0x0C); // Enable display without cursor
    usleep(5000);
    send_command(0x01); // Clear Screen
//    wiringPiI2CWrite(deviceID ,0x08);
    usleep(5000);
    send_command(0x00);
}

void I2CLED::Backlight(bool key) {
    if(true == key){
        wiringPiI2CWrite(fd, 0x08);
    }
    else wiringPiI2CWrite(fd, 0x00);
}

void I2CLED::write_word(int fd, int data) {
    int temp = data;
    if(true == BLEN) temp |= 0x08;
    else temp &= 0xF7;
    wiringPiI2CWrite(fd, temp);
}

void I2CLED::send_command(int command) {
    // Send bit7-4 firstly
    int buf = command & 0xF0;
    buf |= 0x04;// RS = 0, RW = 0, EN = 1
    write_word(fd, buf);
    usleep(2000);
    buf &= 0xFB;// Make EN = 0
    write_word(fd, buf);

    // Send bit3-0 secondly
    buf = (command & 0x0F) << 4;
    buf |= 0x04;// RS = 0, RW = 0, EN = 1
    write_word(fd, buf);
    usleep(2000);
    buf &= 0xFB;// Make EN = 0
    write_word(fd, buf);
}

void I2CLED::send_data(int data) {
    // Send bit7-4 firstly
    int buf = data & 0xF0;
    buf |= 0x05;// RS = 1, RW = 0, EN = 1
    write_word(fd, buf);
    usleep(2000);
    buf &= 0xFB;// Make EN = 0
    write_word(fd, buf);

    // Send bit3-0 secondly
    buf = (data & 0x0F) << 4;
    buf |= 0x05; // RS = 1, RW = 0, EN = 1
    write_word(fd, buf);
    usleep(2000);
    buf &= 0xFB;// Make EN = 0
    write_word(fd, buf);
}

void I2CLED::clear_lcd() {
    send_command(0x01);
}

void I2CLED::print_screen(int x, int y, std::string str) {
    if(x < 0) x = 0;
    if(x > 15) x = 15;
    if(x < 0) y = 0;
    if(y > 1) y = 1;

    int addr = 0x80 + 0x40 * y + x;
    send_command(addr);
	
	int i = 0;
	while(str[i]){
		send_data(str[i++]);
	}
	
    // while(*str) send_data(*str++);
}

inline std::string I2CLED::cmd_system(const char* command) {
  std::string result;
  FILE *fpRead;
  fpRead = popen(command, "r");
  char buf[1024];
  memset(buf, '\0', sizeof(buf));
  while (fgets(buf, 1024 - 1, fpRead) != NULL) {
    result += buf;
	// printf("%s\t", buf);
  }
  if (fpRead != NULL)
    pclose(fpRead);
  return result;
}
