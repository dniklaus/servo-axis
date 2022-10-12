/*
 * Axis.h
 *
 *  Created on: 17.01.2018
 *      Author: nid
 */

#pragma once

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
   * @param name Axis name.
   * @param isReversePosition true: Servo is mounted in reverse position (revert angle count direction), default: false (normal)
   */
  Axis(char* name, bool isReversePosition = false);

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
   * @brief Set the Reverse Position configuration parameter.
   * @param isReversePosition true: Servo is mounted in reverse position (revert angle count direction), false: normal
   */
  void setReversePosition(bool isReversePosition);

  /**
   * @brief Get the Is Reverse Position object
   * @return true Servo is mounted in reverse position (revert angle count direction)
   * @return false Servo is mounted in normal position
   */
  bool getIsReversePosition();

public:
  /**
   * @brief Set a particular angle the Servo shall be moving to with the particular speed.
   * @param targetAngle Angle to be set {-90 .. 90} [°]
   * @param speed {1..450} [°/s]
   */
  void goToTargetAngle(int targetAngle, unsigned int speed);

  /**
   * @brief Stop current action (if active).
   */
  void stop();

protected:
  /**
   * @brief Set the servo PWM through the AServoHal::setAngle() stepwise increasing to the target position.
   * details Called consecutively with m_velocityCtrlIntervalMillis
   */
  void doAngleControl();

public:
  /**
   * @brief Get current angle.
   * 
   * @return int Current angle [°].
   */
  int getAngle();

  /**
   * @brief Set current angle.
   * @details Go to angle using raw PWM control: servo motion with maximum speed.
   */
  void setAngle(int angle);

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
  bool m_isReversePosition;                     /// true: Servo is mounted in reverse position (revert angle count direction)
  int m_angle;                                  /// current angle position [°]
  int m_angleStepPerIteration;                  /// angle step per iteration used in doAngleControl() [°]
  static const unsigned long c_defaultAngleStepPerIteration;         /// [°]
  int m_targetAngle;                            /// current target angle [°]
  bool m_isTargetReached;
  unsigned long m_velocityCtrlIntervalMillis;   /// intervall consecutive doAngleControl() operations are carried out [ms]
  static const unsigned long c_defaultVelocityCtrlIntervalMillis;    /// [ms]
  SpinTimer* m_velocityControlTimer;
  ITargetReachedNotifier* m_targetReachedNotifier;

private:  // forbidden functions
  Axis();                             // default constructor
  Axis(const Axis& src);              // copy constructor
  Axis& operator = (const Axis& src); // assignment operator
};
