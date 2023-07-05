//
// Created by Ivan Kalesnikau on 05.07.2023.
//

#ifndef MEMS_MIRRORCLEMEMS_HPP
#define MEMS_MIRRORCLEMEMS_HPP

#include "Types.hpp"
#include <AD5667.hpp>
#include <GPIO.hpp>
#include <Timer.hpp>

class MirrorcleMEMS {
public:
  enum class AXIS { X, Y };
  AD5667 ad5667_;

private:
  GPIO ENBL_GPIO_;
  Timer clkTimer_;

public:
  MirrorcleMEMS(const GPIO &ENBL_GPIO, const Timer &clkTimer,
                const AD5667 &ad5667);

  void start();

  void stop();

  void writeAxisSignal(const AXIS axis,const scalar signal);

  void foo();
};

#endif // MEMS_MIRRORCLEMEMS_HPP
