
#ifndef MathHelper_h__
#define MathHelper_h__


class MathHelper
{

public:

	template<typename T>
	static T Clamp(const T&x, const T&low, const T&high)
	{
		return x < low ? low : (x > high ? high : x);
	}

};


#endif // MathHelper_h__