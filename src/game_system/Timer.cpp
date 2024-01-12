#include "Timer.h"

Timer::Timer()
	: isRunning_(false)
	, timeLeft_(0)
{
}

void Timer::Update(const double delta)
{
	if (isRunning_)
	{
		timeLeft_ -= delta;
		if (timeLeft_ <= 0)
		{
			isRunning_ = false;
			callback_();
		}
	}
}

void Timer::Start(const double duration)
{
	timeLeft_ = duration;
	isRunning_ = true;
}

void Timer::SetCalback(std::function<void()> callback)
{
	callback_ = callback;
}
