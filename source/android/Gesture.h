#ifndef SST_GESTURE_H
#define SST_GESTURE_H


namespace sunspot
{


class Gesture
{
public:
	Gesture();
	~Gesture() {};

	inline void SetSensibility(const float sensibility) { mSensibility = sensibility; }

	virtual void Check() = 0;
protected:
	float mSensibility;

};


class TapGesture : public Gesture
{
public:
	TapGesture();
	~TapGesture() {}

	virtual void Check();

private:
	float mX;
	float mY;
};


}

#endif // SST_GESTURE_H
