#pragma once

class PlaceholderClass
{
public:
	explicit PlaceholderClass(int value);
	~PlaceholderClass() = default;

	int getValue() const;

private:
	int value;
};
