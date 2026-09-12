#pragma once
// interface pro senzor (v c++ se interface píše jako třída)
class ISensor 
{
public: 
	virtual ~ISensor() = default; // destruktor interface
	virtual double readSensor() = 0;
};