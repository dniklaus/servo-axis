/*
 * Axis.cpp
 *
 *  Created on: 17.01.2018
 *      Author: nid
 */

#include <string.h>
#include <SpinTimer.h>
#include <ITargetReachedNotifier.h>
#include <AServoHal.h>
#include <Axis.h>

class VelocityControlTimerAction : public SpinTimerAction
{
private:
  Axis* m_axis;

public:
  VelocityControlTimerAction(Axis* axis)
  : m_axis(axis)
  { }

  void timeExpired()
  {
    if (0 != m_axis)
    {
      m_axis->doAngleControl();
    }
  }
};

//-----------------------------------------------------------------------------

const unsigned long Axis::c_defaultAngleStepPerIteration      = 1;
const unsigned long Axis::c_defaultVelocityCtrlIntervalMillis = 1000;

Axis::Axis(char* name)
: m_servoHal(0)
, m_name(new char[strlen(name)+1])
, m_angleMin(-90)
, m_angleMax(90)
, m_angle(0)
, m_angleStepPerIteration(c_defaultAngleStepPerIteration)
, m_targetAngle(0)
, m_isTargetReached(false)
, m_velocityCtrlIntervalMillis(c_defaultVelocityCtrlIntervalMillis)
, m_velocityControlTimer(new SpinTimer(0, new VelocityControlTimerAction(this), SpinTimer::IS_RECURRING))
, m_targetReachedNotifier(0)
{
  memset(m_name, 0, strlen(name)+1);
  strncpy(m_name, name, strlen(name));
}

Axis::~Axis()
{
  delete m_velocityControlTimer->action();
  m_velocityControlTimer->attachAction(0);

  delete m_velocityControlTimer;
  m_velocityControlTimer = 0;
  
  memset(m_name, 0, strlen(m_name)+1);
  delete [] m_name;
  m_name = 0;
}

void Axis::attachServoHal(AServoHal* servoHal)
{
  m_servoHal = servoHal;
  if (0 != m_servoHal)
  { 
    m_servoHal->attachAxis(this);
  }
}

AServoHal* Axis::servoHal()
{
  return m_servoHal;
}

void Axis::attachTargetReachedNotifier(ITargetReachedNotifier* targetReachedNotifier)
{
  m_targetReachedNotifier = targetReachedNotifier;
}

ITargetReachedNotifier* Axis::targetReachedNotifier()
{
  return m_targetReachedNotifier;
}

const char* Axis::name() const
{
 return m_name;
}

void Axis::goToTargetAngle(int targetAngle, unsigned int speed)
{
  m_isTargetReached = false;
  m_targetAngle = targetAngle;
  if (speed < 1)
  {
    speed = 1;
  }
  /// @see doAngleControl() gets consecutively called with this interval 
  m_velocityCtrlIntervalMillis = 1000u / speed;

  // let the control process run automatically, start the recurring timer
  m_velocityControlTimer->start(m_velocityCtrlIntervalMillis);
}

void Axis::stop()
{
  // stop the control process, cancel the timer
  m_velocityControlTimer->cancel();
}

void Axis::doAngleControl()
{
  int direction = 1;
  int deltaAngle = m_targetAngle - m_angle;
  if (0 > deltaAngle)
  {
    direction = -1;
  }
  else if (0 == deltaAngle)
  {
    direction = 0;
  }
  else
  {
    direction = 1;
  }
  int deltaOfThisIteration = direction * m_angleStepPerIteration;
  if ((deltaAngle * direction) < m_angleStepPerIteration)
  {
    deltaOfThisIteration = deltaAngle;
  }
  m_angle = m_angle + deltaOfThisIteration;
  if (0 == deltaAngle)
  {
    // Target angle reached, control process to be stopped
    m_velocityControlTimer->cancel();

    m_isTargetReached = true;

    if (0 != m_targetReachedNotifier)
    {
      m_targetReachedNotifier->notifyTargetReached(m_targetAngle);
    }
  }
  else if (0 != m_servoHal)
  {
    m_servoHal->setAngle(m_angle);
  }

}

int Axis::getAngle()
{
  return m_angle;
}

void Axis::setAngle(int angle)
{
  m_angle = angle;
  if (0 != m_servoHal)
  {
    m_servoHal->setAngle(m_angle);
  }
}

bool Axis::isBusy()
{
  return m_velocityControlTimer->isRunning();
}

bool Axis::isTargetReached()
{
  bool isTargetReached = m_isTargetReached;
  m_isTargetReached = false;
  return isTargetReached;
}
