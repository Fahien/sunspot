#include "Cursor.h"

Cursor::Cursor()
:	mPosition{ 0.0f, 0.0f }
,	mPreviousPosition{ 0.0f, 0.0f }
,	mOffset{ 0.0f, 0.0f }
,	mFirstCursor{ true }
,	mSensitivity{ .025f }
{}

void Cursor::setPosition(const float x, const float y)
{
	if (!mFirstCursor)
	{
		mPreviousPosition = mPosition;
	}
	mPosition.x = x;
	mPosition.y = y;
	if (mFirstCursor)
	{
		mPreviousPosition = mPosition;
		mFirstCursor = false;
	}
	mOffset.x = mPosition.x - mPreviousPosition.x;
	mOffset.y = mPosition.y - mPreviousPosition.y;
}