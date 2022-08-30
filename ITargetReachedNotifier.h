#pragma once

class ITargetReachedNotifier
{
public:
  /**
   * @brief Notification on target reached event.
   * 
   * @param targetAngle Curent Angle position that was aimed for and that has been reached {-90 .. 90} [°]
   */
  virtual void notifyTargetReached(int targetAngle) = 0;

public:
  virtual ~ITargetReachedNotifier() { }

protected:
  ITargetReachedNotifier() { }

private:  // forbidden functions
  ITargetReachedNotifier(const ITargetReachedNotifier& src);              // copy constructor
  ITargetReachedNotifier& operator = (const ITargetReachedNotifier& src); // assignment operator
}; 
