#pragma once

class Axis;

/**
 * @brief Abstract Servo HW abstraction layer class.
 * @details Implement setAngle in derived class for concrete HW access.
 */
class AServoHal
{
private:
  Axis* m_axis;

public:
  /**
   * Set a particular angle the Servo shall be set to.
   * @param angle Angle to be set {min .. max}
   */
  virtual void setAngle(int angle) = 0;

  void attachAxis(Axis* axis) { m_axis = axis; }
  Axis* axis() { return m_axis; }

  /**
   * @brief Get the Max Angle Limit
   * 
   * @return int [°]
   */
  int getMaxAngleLimit() { return m_maxAngleLimit; }

  /**
   * @brief Get the Max Angle Limit
   * 
   * @return int [°]
   */
  int getMinAngleLimit() { return m_minAngleLimit; }

protected:
  /**
   * @brief Construct a new AServoHal object.
   * @details Abstract: call this from concrete Servo HAL constructor.
   * 
   * @param maxAngleLimit Upper Servo angle limit [°] (Default: 90°).
   * @param minAngleLimit Lower Servo angle limit [°] (Default: -90°).
   */
  AServoHal(int maxAngleLimit = 90, int minAngleLimit = -90) 
  : m_maxAngleLimit(maxAngleLimit)
  , m_minAngleLimit(minAngleLimit)
  { }

public:
  virtual ~AServoHal() { }

private:
  int m_maxAngleLimit;  /// Upper Servo angle limit [°]
  int m_minAngleLimit;  /// Lower Servo angle limit [°]

private:  // forbidden functions
  AServoHal(const AServoHal& src);              // copy constructor
  AServoHal& operator = (const AServoHal& src); // assignment operator
};
