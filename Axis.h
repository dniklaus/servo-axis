/*
 * Axis.h
 *
 *  Created on: 17.01.2018
 *      Author: nid
 */

#ifndef SRC_AXIS_H_
#define SRC_AXIS_H_

class ITargetReachedNotifier;
class Axis;

class AServoHal
{
private:
  Axis* m_axis;

public:
  /**
   * Set a particular angle the Servo shall be set to.
   * @param angle Angle to be set {-90 .. 90}
   */
  virtual void setAngle(int angle) = 0;

  void attachAxis(Axis* axis) { m_axis = axis; }
  Axis* axis() { return m_axis; }

public:
  virtual ~AServoHal() { }

protected:
  AServoHal() { }

private:  // forbidden functions
  AServoHal(const AServoHal& src);              // copy constructor
  AServoHal& operator = (const AServoHal& src); // assignment operator
};

class SpinTimer;

class Axis
{
public:
  Axis(const char* name);
  virtual ~Axis();

public:
  /**
   * @brief Inject concrete Servo HW Abstraction object, which actually will perform the physical action.
   * 
   * @param servoHal Concrete Servo HW Abstraction object to be injected.
   */
  void attachServoHal(AServoHal* servoHal);

  /**
   * @brief Inject concrete Target Reached Action object, which will perform the particular action when the target was reached.
   * 
   * @param targetReachedNotifier Concrete Target Reached Action object to be injected.
   */
  void attachTargetReachedNotifier(ITargetReachedNotifier* targetReachedNotifier);

  /**
   * @brief 
   * 
   * @return ITargetReachedNotifier* 
   */
  ITargetReachedNotifier* targetReachedNotifier();

  const char* name() const;

  /**
   * Set a particular angle the Servo shall be set to.
   * @param targetAngle Angle to be set {-90 .. 90}
   * @param velocity {1..500}
   */
  void goToTargetAngle(int targetAngle, int velocity);

  /**
   * @brief Stop current action (if active).
   */
  void stop();

  void doAngleControl();

  /**
   * @brief Get current the angle.
   * 
   * @return int Current angle.
   */
  int getAngle();

  /**
   * @brief Get status of the axis.
   * 
   * @return true Axis is busy (going to target angle).
   * @return false Axis is idle.
   */
  bool isBusy();

  /**
   * @brief Get Target status of the axis.
   * 
   * @return true Axis has reached the ordered target position.
   * @return false Axis is not (yet) at the ordered target position.
   */
  bool isTargetReached();

private:
  AServoHal* m_servoHal;
  char* m_name;
  int m_angleMin;
  int m_angleMax;
  int m_angle;
  int m_velocity;
  int m_targetAngle;
  bool m_isTargetReached;
  unsigned long m_velocityCtrlIntervalMillis;
  static unsigned long s_defaultVelocityCtrlIntervalMillis;
  SpinTimer* m_velocityControlTimer;
  ITargetReachedNotifier* m_targetReachedNotifier;

private:  // forbidden functions
  Axis();                             // default constructor
  Axis(const Axis& src);              // copy constructor
  Axis& operator = (const Axis& src); // assignment operator
};

#endif /* SRC_AXIS_H_ */
