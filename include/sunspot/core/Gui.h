#pragma once


namespace sunspot
{


class GlfwWindow;

class Gui
{
  public:
	Gui( const GlfwWindow& );
	~Gui();

	void NewFrame() const;

	void Render() const;
};


} // namespace sunspot
