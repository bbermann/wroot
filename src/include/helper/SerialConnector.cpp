/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef WINDOWS

#include "SerialConnector.hpp"
#include "../core/Core.hpp"

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

SerialConnector::SerialConnector(const char* portname) {
    portname_ = portname;
}

SerialConnector::SerialConnector(const SerialConnector& orig) {

}

SerialConnector::~SerialConnector() {

}

void SerialConnector::start() {
    fd = open(portname_.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        Core::error("error " + to_string(errno) + " opening " + portname_.c_str() + ": " + strerror(errno));
        return;
    }

    setInterfaceAttribs(fd, B115200, 0); // set speed to 115,200 bps, 8n1 (no parity)
    setBlocking(fd, 0); // set no blocking

    //char buf [100];
    //int n = read(fd, buf, sizeof buf); // read up to 100 characters if ready to read
}

void SerialConnector::stop() {
    close(fd);
}

void SerialConnector::serialWrite(const char* str) {
    write(fd, str, sizeof (str));
    usleep((7 + 25) * 100); // sleep enough to transmit the 7 plus
    // receive 25:  approx 100 uS per char transmit
}

int SerialConnector::setInterfaceAttribs(int fd, int speed, int parity) {
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0) {
        Core::error("error " + to_string(errno) + " from tcgetattr");
        return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK; // disable break processing
    tty.c_lflag = 0; // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0; // no remapping, no delays
    tty.c_cc[VMIN] = 0; // read doesn't block
    tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD); // ignore modem controls,
    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag &= ~HUPCL; //disable hang-up-on-close to avoid Arduino's auto-reset.

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        Core::error("error " + to_string(errno) + " from tcsetattr");
        return -1;
    }
    return 0;
}

void SerialConnector::setBlocking(int fd, int should_block) {
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0) {
        Core::error("error " + to_string(errno) + " from tcgetattr");
        return;
    }

    tty.c_cc[VMIN] = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
        Core::error("error " + to_string(errno) + " setting term attributes");
}

#endif