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
  void attachServoHal(AServoHal* servoHal);
  void attachTargetReachedNotifier(ITargetReachedNotifier* targetReachedNotifier);

  ITargetReachedNotifier* targetReachedNotifier();

  const char* name() const;

  /**
   * Set a particular angle the Servo shall be set to.
   * @param targetAngle Angle to be set {-90 .. 90}
   * @param velocity
   */
  void goToTargetAngle(int targetAngle, int velocity);
  void stop();

  void doAngleControl();

  int getAngle();

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
