/*
 * Axis.h
 *
 *  Created on: 17.01.2018
 *      Author: nid
 */

#ifndef SRC_AXIS_H_
#define SRC_AXIS_H_

class SpinTimer;
class AServoHal;
class ITargetReachedNotifier;

/**
 * @brief Servo Axis control.
 * 
 * This class helps to control the position of a simple servo.
 */
class Axis
{
  friend class VelocityControlTimerAction;

public:
  /**
   * @brief Construct a new Axis object.
   * 
   * @param name Axis name.
   */
  Axis(char* name);

  /**
   * @brief Destroy the Axis object.
   */
  virtual ~Axis();

public:
  /**
   * @brief Inject concrete Servo HW Abstraction Layer object, which actually will perform the physical action.
   * 
   * @param servoHal Concrete Servo HW Abstraction Layer object to be injected.
   */
  void attachServoHal(AServoHal* servoHal);

  /**
   * @brief Access to concrete Servo HW Abstraction Layer object.
   * 
   * @return Servo HW Abstraction Layer object.
   */
  AServoHal* servoHal();

  /**
   * @brief Inject concrete Target Reached Notifier object, which will perform the particular action when the target was reached.
   * 
   * @param targetReachedNotifier Target Reached Notifier object.
   */
  void attachTargetReachedNotifier(ITargetReachedNotifier* targetReachedNotifier);

  /**
   * @brief Access to concrete Target Reached Notifier object.
   * 
   * @return Target Reached Notifier object. 
   */
  ITargetReachedNotifier* targetReachedNotifier();

  /**
   * @brief Access to 
   * 
   * @return const char* 
   */
  const char* name() const;

  /**
   * @brief Set a particular angle the Servo shall be set to.
   * @param targetAngle Angle to be set {-90 .. 90} [°]
   * @param velocity {1..500}
   */
  void goToTargetAngle(int targetAngle, int velocity);

  /**
   * @brief Stop current action (if active).
   */
  void stop();

protected:
  void doAngleControl();

public:
  /**
   * @brief Get current angle.
   * 
   * @return int Current angle [°].
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
  int m_targetAngle;  /// current target angle
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
