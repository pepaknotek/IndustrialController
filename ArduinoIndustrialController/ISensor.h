#pragma once
// interface pro senzor (v c++ se interface pise jako trida)
// -- kopie z existujiciho repa, beze zmeny, jen aby byla slozka samostatne pouzitelna
class ISensor
{
public:
	virtual ~ISensor() = default; // destruktor interface
	virtual double readSensor() = 0;
};
