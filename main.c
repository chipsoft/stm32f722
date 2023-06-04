/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "rt_test_root.h"
#include "oslib_test_root.h"

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palSetLine(LINE_LED1);
    chThdSleepMilliseconds(50);
    palSetLine(LINE_LED2);
    chThdSleepMilliseconds(50);
    palSetLine(LINE_LED3);
    chThdSleepMilliseconds(200);
    palClearLine(LINE_LED1);
    chThdSleepMilliseconds(50);
    palClearLine(LINE_LED2);
    chThdSleepMilliseconds(50);
    palClearLine(LINE_LED3);
    chThdSleepMilliseconds(200);
  }
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 3 using the driver default configuration.
   */
  sdStart(&SD3, NULL);
  canStart(&CAND1, NULL);

  /*
   * Creates the example thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    // if (palReadLine(LINE_BUTTON)) {
    //   test_execute((BaseSequentialStream *)&SD3, &rt_test_suite);
    //   test_execute((BaseSequentialStream *)&SD3, &oslib_test_suite);
    // }
    CANTxFrame txmsg;
    txmsg.IDE = CAN_IDE_STD;
    txmsg.SID = 0x123;
    txmsg.RTR = CAN_RTR_DATA;
    txmsg.DLC = 8;
    txmsg.data8[0] = 0x11;
    txmsg.data8[1] = 0x22;
    txmsg.data8[2] = 0x33;
    txmsg.data8[3] = 0x44;
    txmsg.data8[4] = 0x55;
    txmsg.data8[5] = 0x66;
    txmsg.data8[6] = 0x77;
    txmsg.data8[7] = 0x88;
    canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, TIME_MS2I(1000));
    chThdSleepMilliseconds(1000);
  }
}
