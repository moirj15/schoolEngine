#ifndef TIME_H
#define TIME_H

#include "common.h"

class Time {
  f32 m_previousTime;
  f32 m_currentTime;
  f32 m_delta;

public:
  Time();

  f32 PreviousTime() const { return m_previousTime; }

  f32 CurrentTime() const { return m_currentTime; }

  f32 Delta() const { return m_delta; }

  void Step();

  void SetTime(f32 t);
};

#endif // TIME_H
