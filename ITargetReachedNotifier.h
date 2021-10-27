#pragma once

class ITargetReachedNotifier
{
public:
  virtual void notifyTargetReached(int targetAngle) = 0;

public:
  virtual ~ITargetReachedNotifier() { }

protected:
  ITargetReachedNotifier() { }

private:  // forbidden functions
  ITargetReachedNotifier(const ITargetReachedNotifier& src);              // copy constructor
  ITargetReachedNotifier& operator = (const ITargetReachedNotifier& src); // assignment operator
}; 
