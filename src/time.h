#ifndef TIME_H
#define TIME_H

#include "common.h"

class Time {
  f32 m_previousTime;
  f32 m_currentTime;
  f32 m_delta;

public:
  /// Constructor
  Time();

  /// Getter for the previous time
  f32 PreviousTime() const { return m_previousTime; }

  /// Getter for the current time
  f32 CurrentTime() const { return m_currentTime; }

  /// Getter for the time delta
  f32 Delta() const { return m_delta; }

  /**
   * Stores the currentTime in the previous time, gets the new current time, and updates the delta.
   */
  void Step();

  /**
   * Sets the current time.
   * @param t: The time.
   */
  void SetTime(f32 t);
};

#endif // TIME_H
