//
// Created by Ivan Kalesnikau on 05.07.2023.
//

#include "MirrorcleMems.hpp"
MirrorcleMEMS::MirrorcleMEMS(const GPIO &ENBL_GPIO, const Timer &clkTimer,
                             const AD5667 &ad5667)
    : ENBL_GPIO_(ENBL_GPIO), clkTimer_(clkTimer), ad5667_(ad5667) {}

void MirrorcleMEMS::start() {
  clkTimer_.writePWMDuty(static_cast<scalar>(0.5));
  clkTimer_.startPWM();
  ENBL_GPIO_.on();
}
void MirrorcleMEMS::stop() {
  ENBL_GPIO_.off();
  clkTimer_.stopPWM();
}
void MirrorcleMEMS::writeAxisSignal(const MirrorcleMEMS::AXIS axis,
                                    const scalar signal) {
  AD5667::CHANNEL dacChannel =
      (axis == AXIS::X) ? AD5667::CHANNEL::A : AD5667::CHANNEL::B;
  ad5667_.sendSignal(AD5667::COMMAND::WRITE_UPDATE_ALL, dacChannel, signal);
}
void MirrorcleMEMS::foo() {
  //  ad5667_.CLR_GPIO().on();
  //  HAL_Delay(1);
  //  ad5667_.CLR_GPIO().off();
  //  printf("%hu\n", AD5667::OPERATING_MODE::PULL_DOWN_10K);
  //  ad5667_.setupPower(AD5667::CHANNEL::BOTH,
  //  AD5667::POWER_SETUP_MODE::NORMAL); ad5667_.fullReset(); HAL_Delay(500);
  ad5667_.LDAC_GPIO().on();
  ad5667_.sendData(AD5667::COMMAND::LDAC, AD5667::CHANNEL::BOTH, 3);
  //  ad5667_.sendData(AD5667::COMMAND::RESET, AD5667::CHANNEL::BOTH, 1);
}