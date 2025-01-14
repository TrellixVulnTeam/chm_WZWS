//****************************************************************************
// Product: Core_Health example
// Last Updated for Version: 6.1.2
// Date of the Last Update:  2018-03-08
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (C) Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Contact information:
// https://www.state-machine.com
// mailto:info@state-machine.com
//****************************************************************************
#ifndef bsp_h
#define bsp_h

namespace Core_Health {

class BSP {
public:
    enum { TICKS_PER_SEC = 100 };

    static void init(int argc, char **argv);
    static void displayPaused(uint8_t const paused);
    static void displayPhilStat(uint8_t const n, char_t const *stat);
    static void terminate(int16_t const result);

    static void randomSeed(uint32_t const seed); // random seed
    static uint32_t random(void);                // pseudo-random generator

    // for testing...
    static void wait4SW1(void);
    static void ledOn(void);
    static void ledOff(void);
};

} // namespace Core_Health

#endif // bsp_h
