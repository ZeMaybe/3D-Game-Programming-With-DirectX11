#ifndef GameTimer_h__
#define GameTimer_h__


class GameTimer
{
public:
	GameTimer();

	float TotalTime()const;  // in seconds
	float DeltaTime()const; // in seconds

	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop();  // Call when paused.
	void Tick();  // Call every frame.

private:
	double mSecondsPerCount = 0.0;
	double mDeltaTime = -1.0;

	__int64 mBaseTime = 0;
	__int64 mPausedTime = 0;
	__int64 mStopTime;
	__int64 mPrevTime = 0;
	__int64 mCurrTime = 0;

	bool mStopped = false;
};


#endif // GameTimer_h__
