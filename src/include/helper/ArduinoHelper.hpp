/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

 /*
  * File:   Arduino.hpp
  * Author: bbermann
  *
  * Created on 13 de Novembro de 2015, 12:20
  */

#ifndef WINDOWS

#ifndef ARDUINO_HPP
#define ARDUINO_HPP

#include "SerialConnector.hpp"
#include "../core/Core.hpp"

namespace Arduino {

	const char* pinOutHigh = "wroot SH";
	const char* pinOutLow = "wroot SL";

	SerialConnector serial_conn("/dev/ttyUSB0");

	void start() {
		serial_conn.start();
	}

	void stop() {
		serial_conn.stop();
	}

	void pinOut(bool on) {
		serial_conn.serialWrite(on ? pinOutHigh : pinOutLow);
	}
}

#endif /* ARDUINO_HPP */

#endif