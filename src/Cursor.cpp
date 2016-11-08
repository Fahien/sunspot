#include "Cursor.h"

Cursor::Cursor()
	: position_{ 0.0f, 0.0f }
	, previousPosition_{ 0.0f, 0.0f }
	, offset_{ 0.0f, 0.0f }
	, firstCursor_{ true }
	, sensitivity_{ .025f }
{}

void Cursor::setPosition(const float x, const float y)
{
	if (!firstCursor_) {
		previousPosition_ = position_;
	}
	position_.x = x;
	position_.y = y;
	if (firstCursor_) {
		previousPosition_ = position_;
		firstCursor_ = false;
	}
	offset_.x = position_.x - previousPosition_.x;
	offset_.y = position_.y - previousPosition_.y;
}