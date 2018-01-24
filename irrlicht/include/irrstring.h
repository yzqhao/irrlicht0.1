// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __IRR_STRING_H_INCLUDED__
#define __IRR_STRING_H_INCLUDED__

#include "irrTypes.h"

namespace irr
{
namespace core
{

//!	String class with some useful features.
/**	string<c8> and string<wchar_t> work both with unicode AND ascii,
so you can assign unicode to string<c8> and ascii to string<wchar_t> 
(and the other way round) if your ever would want to. 
Note that the conversation between both is not done using an encoding.

Known bugs:
Characters like '?, '? and '? are ignored in the functions make_upper, make_lower and equals_ignore_case

TODO:
substring
find
replace
*/
template <class T>
class string
{
public:

	//! Default constructor
	string()
	: allocated(1), used(1), array(0)
	{
		array = new T[1];
		array[0] = 0x0;
	}



	//! Constructor
	string(const string<T>& other)
	: allocated(0), used(0), array(0)
	{
		*this = other;
	}



	//! Constructor for unicode and ascii strings
	template <class B>
	string(const B* c)
	: allocated(0), used(0), array(0)
	{
		*this = c;
	}



	//! destructor
	~string()
	{
		delete [] array;
	}



	//! Assignment operator
	string<T>& operator=(const string<T>& other) 
	{
		if (this == &other)
			return *this;

		delete [] array;
		allocated = used = other.size()+1;
		array = new T[used];

		const T* p = other.c_str();
		for (s32 i=0; i<used; ++i, ++p)
			array[i] = *p;

		return *this;
	}



	//! Assignment operator for strings, ascii and unicode
	template <class B>
	string<T>& operator=(const B* c) 
	{
		if (!c)
		{
			array[0] = 0x0;
			return *this;
		}

		if ((void*)c == (void*)array)
			return *this;

		s32 len = 0;
		const B* p = c;
		while(*p)
		{
			++len;
			++p;
		}

		// we'll take the old string for a while, because the new string could be
		// a part of the current string.
		T* oldArray = array;

        allocated = used = len+1;
		array = new T[used];

		for (s32 l = 0; l<len+1; ++l)
			array[l] = (T)c[l];

		delete [] oldArray;
		return *this;
	}



	//! Add operator
	string<T>& operator+(const string<T>& other)
	{
		--used;

		s32 len = (s32)other.size() + 1;

		if (used + len > allocated)
			reallocate((s32)used + len);

		const T* p = other.c_str();
		for (s32 i=0; i<len; ++i, ++p)
			array[used+i] = *p;

		used = used+len;

		return *this;
	}



	//! Add operator for strings, ascii and unicode
	template <class B>
	string<T>& operator+(const B* c)
	{
		--used;

		s32 len = 0;
		const B* p = c;
		while(*p)
		{
			++len;
			++p;
		}

		++len;

		if (used + len > allocated)
			reallocate((s32)used + (s32)len);

		for (s32 l = 0; l<len; ++l)
			array[l+used] = (T)c[l];

		used = used + len;

		return *this;
	}



	//! Direct access operator
	T& operator [](const s32 index)  const
	{
		#ifdef _DEBUG
		if (index>=used)
			_asm int 3 // bad index
		#endif

		return array[index];
	}



	//! Comparison operator
	bool operator ==(const string<T>& other) const
	{
		for(s32 i1=0, i2=0; array[i1] && other.array[i2]; ++i1, ++i2)
			if (array[i1] != other.array[i2])
				return false;

		return used == other.used;
	}



	//! Is smaller operator
	bool operator <(const string<T>& other) const
	{
		for(s32 i1=0, i2=0; array[i1] && other.array[i2]; ++i1, ++i2)
			if (array[i1] != other.array[i2])
				return (array[i1] < other.array[i2]);

		return used < other.used;
	}



	//! Equals not operator
	bool operator !=(const string<T>& other) const
	{
		return !(*this == other);
	}


    
	//! Returns lenght of string
	//! \return Returns lenght of the string in characters.
	s32 size() const
	{
		return used-1;
	}



	//! Returns character string
	//! \return Returns pointer to C-style zero terminated string.
	const T* c_str() const
	{
		return array;
	}



	//! Makes the string lower case.
	void make_lower()
	{
		const T A = (T)'A';
		const T Z = (T)'Z';
		const T diff = (T)'a' - A;

		for (s32 i=0; i<used; ++i)
		{
			if (array[i]>=A && array[i]<=Z)
				array[i] += diff;
		}
	}



	//! Makes the string upper case.
	void make_upper()
	{
		const T a = (T)'a';
		const T z = (T)'z';
		const T diff = (T)'A' - a;

		for (s32 i=0; i<used; ++i)
		{
			if (array[i]>=a && array[i]<=z)
				array[i] += diff;
		}
	}



	//! Compares the string ignoring case.
	//! \param other: Other string to compare.
	//! \return Returns true if the string are equal ignoring case.
	bool equals_ignore_case(const string<T>& other) const
	{
		for(s32 i1=0, i2=0; array[i1] && other[i2]; ++i1, ++i2)
			if (toLower(array[i1]) != toLower(other[i2]))
				return false;

		return used == other.used;
	}



	//! Appends a character to this string
	//! \param character: Character to append.
	void append(T character)
	{
		if (used + 1 > allocated)
			reallocate((s32)used + 1);

		used += 1;

		array[used-2] = character;
		array[used-1] = 0;
	}


	//! Appends a string to this string
	//! \param other: String to append.
	void append(const string<T>& other)
	{
		--used;

		s32 len = other.size();
		
		if (used + len + 1 > allocated)
			reallocate((s32)used + (s32)len + 1);

		for (s32 l=0; l<len+1; ++l)
			array[l+used] = other[l];

		used = used + len + 1;
	}


	//! Appends a string of the lenght l to this string.
	//! \param other: other String to append to this string.
	//! \param lenght: How much characters of the other string to add to this one.
	void append(const string<T>& other, s32 lenght)
	{
		s32 len = other.size();

		if (len < lenght)
		{
			append(other);
			return;
		}

		len = lenght;
		--used;
		
		if (used + len > allocated)
			reallocate((s32)used + (s32)len);

		for (s32 l=0; l<len; ++l)
			array[l+used] = other[l];

		used = used + len;
	}


	//! Reserves some memory.
	//! \param count: Amount of characters to reserve.
	void reserve(s32 count)
	{
		if (count < allocated)
			return;

		reallocate(count);
	}

private:

	//! Returns a character converted to lower case
	T toLower(const T& t) const
	{
		if (t>=(T)'A' && t<=(T)'Z')
			return t + ((T)'a' - (T)'A');
		else
			return t;
	}

	//! Reallocate the array, make it bigger or smaler
	void reallocate(s32 new_size)
	{
		T* old_array = array;

		array = new T[new_size];
		allocated = new_size;
		
		s32 amount = used < new_size ? used : new_size;
		for (s32 i=0; i<amount; ++i)
			array[i] = old_array[i];

		if (allocated < used)
			used = allocated;
		
		delete [] old_array;
	}


	//--- member variables

	T* array;
	s32 allocated;
	s32 used;
};


//! Typedef for character strings
typedef string<c8> stringc;

//! Typedef for wide character strings
typedef string<wchar_t> stringw;


} // end namespace core
} // end namespace irr

#endif