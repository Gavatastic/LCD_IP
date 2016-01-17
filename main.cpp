/* 
 * File:   main.cpp
 * Author: Gavin
 *
 * Created on 26 August 2015, 14:59
 * 
 * 
 * Compile: g++ -o LCD_IP main.cpp -L/u9sr/local/lib -lwiringPi -lwiringPiDev -lpthread -lm
 * 
 */

#include <cstdlib>
#include <unistd.h>
#include <string>

#include "1602I2C.h"
#include "1602I2C.cpp"

#include <stdio.h>      
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

const int deviceID = 0x27; // 1602 address

#define	MAX_NETWORK_ADAPTORS						3
#define	MAX_ADAPTOR_NAME_LENGTH						20



int main(int argc, char** argv) {

    I2CLED led(deviceID);
    
    
    char message0[17];
    char message1[17];
    
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    DIR *dir;
    struct dirent *dirent;
    char dev[16];      // Dev ID
    char devPath[128]; // Path to device
    char buf[256];     // Data from device
    char path[] = "/sys/bus/w1/devices"; 
    ssize_t numRead;
    
    
    // Read and report IP address
    
    getifaddrs(&ifAddrStruct);

        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            sprintf(message0,"%s:", ifa->ifa_name);
            sprintf(message1,"%s", addressBuffer);

            led.print_screen(0,0,string(message0)); 
            led.print_screen(0,1,string(message1));  
 
        } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            sprintf(message0,"%s:\n", ifa->ifa_name);
            sprintf(message1,"%s\n", addressBuffer);
            sprintf(message0,"%s:", ifa->ifa_name);
            sprintf(message1,"%s", addressBuffer);
        } 
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);

    led.Backlight(true); 
            
    sleep(10);
    
    led.Backlight(false);   
    
    
    return 0;
}

