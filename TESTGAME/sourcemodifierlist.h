#pragma once
#ifndef SOURCEMODIFIERLIST_H
#define SOURCEMODIFIERLIST_H

#include <vector>
#include <string>

struct Modifier
{
	const std::string source;
	const std::string key;
	float value;

	Modifier(std::string source, float value, std::string key = "") :
		source(source), value(value), key(key)
	{

	}

	Modifier& operator= (const Modifier& m)
	{
		(std::string&)source = m.source;
		(std::string&)key = m.key;
		value = m.value;
		return *this;
	}

};

enum MODITYPE {
	ADDITIVE,
	MULTIPLY,
};
struct SourceModifierList
{
	const MODITYPE type;

	const float base;

	std::vector<Modifier> modifiers;

	SourceModifierList(float base = 1.0f, MODITYPE type = MODITYPE::ADDITIVE) :
		base(base), type(type)
	{

	}

	void AddModifier(std::string source, float value, std::string key = "")
	{
		modifiers.push_back(Modifier(source, value, key));
	}

	void RemoveModifier(std::string source, std::string key = "")
	{
		for (auto m = modifiers.begin(); m != modifiers.end(); )
		{
			if (source == (*m).source && (key == (*m).key || key == ""))
				m = modifiers.erase(m);
			else
				++m;
		}
	}

	float CalcModifiers()
	{
		float v = base;
		for (auto m : modifiers)
		{
			if (type == MODITYPE::ADDITIVE)
				v += m.value;
			else
				v *= m.value;
		}
		return v;
	}

	float CalcModifiersFromSource(std::string source)
	{
		float v = base;
		for (auto m : modifiers)
		{
			if (source != m.source)
				continue;
			if (type == MODITYPE::ADDITIVE)
				v += m.value;
			else
				v *= m.value;
		}
		return v;
	}

	float CalcModifiersFromKey(std::string key)
	{
		float v = base;
		for (auto m : modifiers)
		{
			if (key != m.key)
				continue;
			if (type == MODITYPE::ADDITIVE)
				v += m.value;
			else
				v *= m.value;
		}
		return v;
	}

};

#endif // !SOURCEMODIFIERLIST_H
