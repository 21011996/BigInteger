#include "big_integer.h"
#include <algorithm>
#include <iostream>
#include <string.h>

//BIG_INTEGER_OPTIMIZED_REMAKE

my_vector::my_vector()
{
	data = 0;
	allocated = 1;
}

big_integer::big_integer()
{
	small_data = 0;
	small = true;
	used = new uint32_t(1);
}

big_integer::big_integer(int a)
{
	small_data = a;
	small = true;
	used = new uint32_t(1);
}



my_vector::my_vector(int a)
{
	data = new int32_t[1];
	data[0] = a;
	allocated = 1;
}

big_integer::big_integer(string a):
big_integer(0)
	{
	bool negative = false;
	size_t start = 0;
	if (a[0] == '-'){
		negative = true;
		start++;
	}
	for (size_t i = start; i < a.size(); ++i)
	{
		int32_t t = a[i] - '0';
		*this *= 10;
		*this += big_integer(t);
	}
	if (negative)
	{
		*this = -*this;
	}
	used = new uint32_t(1);
}

big_integer::big_integer(big_integer const & other)
{
	++(*(other.used));
	small = other.small;
	if (small)
	{
		small_data = other.small_data;
	}
	else
	{
		big_int = other.big_int;
	}
	used = other.used;
}

big_integer::big_integer(big_integer const & other, unshare_t)
	: small(other.small)
	, big_int(other.big_int)
	, small_data(other.small_data)
	, used(new unsigned(1))
{}

my_vector::my_vector(my_vector const& other)
{
	allocated = other.allocated;
	data = new int32_t[allocated];
	memcpy(data, other.data, 4 * allocated);
}

big_integer big_integer::copy() const
{
	return big_integer(*this, unshare);
}


/*
big_integer::operator string() const
{
	string answer = "";
	for (int i = 0; i < allocated * 32; ++i){
		int pos = i / 32; //position
		int off = i % 32; //offset
		if ((data[pos]) & (1 << off)){
			answer += "1";
		}
		else {
			answer += "0";
		}
	}
	return answer;
}
*/

big_integer::operator string() const

{
	if (small)
	{
		string result = std::to_string(small_data);
		return result;
	}
	else
	{
		my_vector tmp(big_int);
		if (tmp != my_vector(0)){
			string answer;
			while (tmp != my_vector(0))
			{
				my_vector toAnswer = tmp % 10;
				string toans = std::to_string(toAnswer.data[0]);
				answer = toans + answer;
				tmp = tmp / (my_vector(10));
			}
			if (neg())
			{
				answer = "-" + answer;
			}
			return answer;
		}
		else{
			return "0";
		}
	}
}


big_integer::~big_integer()
{
	if (*used > 1)
	{
		--*used;
	}
	else
	{
		delete used;
	}
}

my_vector::~my_vector()
{
	delete[] data;
}


//инкременты
big_integer& big_integer::operator ++()
{
	big_integer a(*this);
	if (*used > 1)
	{
		a = copy();
	}
	if (small)
	{
		if (small_data < INT32_MAX)
		{
			++small_data;
		}
		else
		{
			small = false;
			big_int = my_vector(small_data);
			small_data = 0;
			big_int++;
		}
	}
	else
	{
		big_int++;
	}
	return *this;
}

my_vector& my_vector::operator ++ ()
{
	my_vector a(*this);
	bool was_positive = a.pos();
	bool increment = true;
	int i = 0;
	while (increment && i < a.allocated)
	{
		++a.data[i];
		if (a.data[i] != 0){
			increment = false;
		}
		++i;
	}

	if ( was_positive && a.neg()){//owerflow
		delete [] a.data;
		++a.allocated;
		a.data = new int32_t[a.allocated];
		memset(a.data, 0, 4 * a.allocated);
		a.data[a.allocated - 2] = 1<<31;
	}
	*this = a;
	return *this;
}

big_integer& big_integer::operator ++ (int)
{
	big_integer * temp = new big_integer(* this);
	++ (* this);
	return *temp;
}

my_vector& my_vector::operator ++ (int)
{
	my_vector * temp = new my_vector(*this);
	++(*this);
	return *temp;
}

//декременты
big_integer& big_integer::operator --()
{
	big_integer a(*this);
	if (*used > 1)
	{
		a = copy();
	}
	if (small)
	{
		if (small_data > INT32_MIN)
		{
			--small_data;
		}
		else
		{
			small = false;
			big_int = my_vector(small_data);
			small_data = 0;
			--big_int;
		}
	}
	else
	{
		--big_int;
	}
	return *this;
}

my_vector& my_vector::operator -- ()
{
	my_vector a(*this);
	bool was_negative = a.neg();
	bool decrement = true;
	int i = 0;
	while (decrement && i < a.allocated)
	{
		--a.data[i];
		if (a.data[i] != -1)
		{
			decrement = false;
		}
		++i;
	}

	if (was_negative && a.pos())
	{
		delete[] a.data;
		++a.allocated;
		a.data = new int32_t[a.allocated];
		memset(a.data, -1, 4 * a.allocated);
		a.data[a.allocated - 2] = a.data[a.allocated - 2] ^ (1 << 31);
	}
	a.shrink_to_fit();
	*this = a;
	return *this;
}

big_integer& big_integer::operator -- (int)
{
	big_integer * temp = new big_integer(*this);
	--(*this);
	return *temp;
}


//присваивание

//big_integer& big_integer::operator = (int a)
//{
//	*this = big_integer(a);
//	return *this;
//}

big_integer& big_integer:: operator = (big_integer const& other)
{
	if (!(*this == other))
	{

		if (*used == 1)
		{
			if (small)
			{
				small_data = 0;
			}
			else
			{
				delete[] big_int.data;
			}
			delete used;
		}
		else
		{
			--(*used);
		}
		small = other.small;
		if (small)
		{
			small_data = other.small_data;
		}
		else
		{
			big_int = other.big_int;
		}
		used = other.used;
		++(*used);
	}
	return *this;
}

my_vector& my_vector::operator = (my_vector const & other)
{
	if (&other != this){
		delete[] data;
		allocated = other.allocated;
		data = new int[allocated];
		memcpy(data, other.data, 4 * allocated);
	}
	return *this;
}


//сравнение

bool operator < (my_vector & fir, my_vector & sec)
{
	if (fir.neg() && sec.pos())
	{
		return true;
	}
	else if (fir.pos() && sec.neg())
	{
		return false;
	}
	else
	{
		my_vector a (fir);
		my_vector b (sec);
		a.fitSize(b);

		for (int i = a.allocated * 32 - 2; i >= 0; --i)
		{
			int pos = i / 32; //position
			int off = i % 32; //offset

				if (((a.data[pos]) & (1 << off)) && !((b.data[pos]) & (1 << off)))
				{
					return false;
				}
				else if (!((a.data[pos]) & (1 << off)) && ((b.data[pos]) & (1 << off)))
				{
					return true;
				}


		}
		return false;
	}
}

bool operator < (big_integer& fir, big_integer& sec)
{
	if (fir.small && sec.small)
	{
		return (fir.small_data < sec.small_data);
	}
	else
	{
		if (fir.small && !sec.small)
		{
			return (my_vector(fir.small_data) < sec.big_int);
		}
		else if (!fir.small && sec.small)
		{
			return (fir.big_int < my_vector(sec.small_data));
		}
		else
		{
			return (fir.big_int < sec.big_int);
		}
	}
}

bool operator > (my_vector & a, my_vector & b)
{
	return b < a;
}

bool operator >(big_integer & a, big_integer & b)
{
	return b < a;
}

bool operator >= (big_integer & a, big_integer & b)
{
	return !(a < b);
}

bool operator <= (big_integer & a, big_integer & b)
{
	return !(b < a);
}

bool operator >= (my_vector & a, my_vector & b)
{
	return !(a < b);
}

bool operator == (big_integer const & a, big_integer const & b)
{
	if (a.small && b.small)
	{
		return a.small_data == b.small_data;
	}
	else
	{
		my_vector tmp1(0);
		my_vector tmp2(0);
		if (a.small)
		{
			tmp1 = my_vector(a.small_data);
		}
		else
		{
			tmp1 = a.big_int;
		}
		if (b.small)
		{
			tmp2 = my_vector(b.small_data);
		}
		else
		{
			tmp2 = b.big_int;
		}
		tmp1.fitSize(tmp2);
		return memcmp(tmp1.data, tmp2.data, 4 * tmp1.allocated) == 0;

	}
}

bool operator == (my_vector& a, my_vector& b)
{
	my_vector tmp1(a);
	my_vector tmp2(b);
	tmp1.fitSize(tmp2);
	return memcmp(tmp1.data, tmp2.data, 4 * tmp1.allocated) == 0;
}

bool operator != (big_integer & a, big_integer & b)
{
	return !(a == b);
}

bool operator != (my_vector& a, my_vector& b)
{
	return !(a == b);
}



//унарные операции


big_integer& big_integer::operator + ()
{
	return *this;
}

big_integer big_integer::operator - () const
{
	big_integer tmp(*this);
	if (small)
	{
		// TODO
		// big_integer tmp(*this);
		// tmp.small_data = - small_data;
		// return tmp;
		tmp.small_data = - small_data;
	}
	else
	{
		--(tmp);
		tmp = ~tmp;
	}
	return tmp;
}

my_vector my_vector:: operator - () const
{
	my_vector tmp = *this;
	--tmp;
	return ~tmp;
}

//ѕобитовые операции:

big_integer operator & (big_integer const& a, big_integer const & b)
{
	big_integer tmp(b);
	return tmp &= a;
}



big_integer operator | (big_integer const& a, big_integer const & b)
{
	big_integer tmp(b);
	return tmp |= a;
}

big_integer& big_integer::operator |= (big_integer const& b)
{
	if (small && b.small)
	{
		small_data |= b.small_data;
	}
	else
	{
		my_vector tmp1(0);
		if (small)
		{
			tmp1 = my_vector(small_data);
		}
		else
		{
			tmp1 = big_int;
		}
		my_vector tmp2(0);
		if (b.small)
		{
			tmp2 = my_vector(b.small_data);
		}
		else
		{
			tmp2 = b.big_int;
		}
		tmp1 |= tmp2;
		big_int = tmp1;
	}
	return *this;
}

big_integer operator ^ (big_integer const& a, big_integer const & b)
{
	big_integer tmp(b);
	return tmp ^= a;
}

my_vector& my_vector::operator ~ ()
{
	for (int i = 0; i < allocated; ++i)
	{
		data[i] = ~data[i];
	}
	return *this;
}

big_integer big_integer::operator~()
{
	big_integer tmp (*this);
	if (small)
	{
		tmp.small_data = ~tmp.small_data;
	}
	else
	{
		tmp.big_int = ~tmp.big_int;
	}
	return tmp;
}

big_integer& big_integer::operator ! ()
{
	return ~ (* this);
}



//побитовые сдвиги

big_integer operator >> (big_integer & a, int const b)
{
	big_integer tmp(a);
	return tmp >>= b;
}

big_integer& big_integer::operator >>= (int a)
{
	big_integer answer(*this);
	if (small)
	{
		answer.small_data >>= a;
	}
	else
	{
		answer.big_int >>= a;
	}
	*this = answer;
	return *this;
}

my_vector& my_vector::operator >>= (int a)
{
	my_vector answer(0);
	if (neg()){
		--answer;
	}
	answer.reallocate(allocated);
	for (int i = a; i < allocated * 32; ++i)
	{
		int pos = i / 32; //position
		int off = i % 32;

		int j = i - a;
		int jPos = j / 32;
		int jOff = j % 32;

		if (data[pos] & (1 << off))
		{
			answer.data[jPos] = (answer.data[jPos] | (1 << jOff));
		}
		else if (neg())
		{
			answer.data[jPos] = (answer.data[jPos] ^ (1 << jOff));
		}
	}
	answer.shrink_to_fit();
	*this = answer;
	return *this;
}

big_integer operator << (big_integer & a, int const b)
{
	big_integer tmp(a);
	return tmp <<= b;
}

my_vector operator << (my_vector & a, int const b)
{
	my_vector tmp(a);
	return tmp <<= b;
}

big_integer& big_integer::operator <<= (int a)
{
	big_integer answer(*this);
	if (small)
	{
		answer.small_data <<= a;
	}
	else
	{
		answer.big_int <<= a;
	}
	*this = answer;
	return *this;
}

my_vector& my_vector::operator <<= (int a)
{
	my_vector answer(0);
	answer.reallocate(allocated);
	for (int i = 0; i < allocated * 32 - a; ++i)
	{
		int pos = i / 32; //position
		int off = i % 32;

		int j = i + a;
		int jPos = j / 32;
		int jOff = j % 32;

		if (data[pos] & (1 << off))
		{
			answer.data[jPos] = (answer.data[jPos] | (1 << jOff));
		}
	}
	answer.shrink_to_fit();
	*this = answer;
	return *this;

}


//
//јрифметика
//


//сложение

big_integer operator + (big_integer const& a, big_integer const & b) 
{
	big_integer tmp(a);
	return tmp += b;
}

my_vector operator + (my_vector const& a, my_vector const& b)
{
	my_vector tmp(a);
	return tmp += b;
}

big_integer& big_integer::operator += (big_integer const& other)
{
	if (small && other.small && (((long)small_data + (long)other.small_data < (long)INT32_MAX) && ((long)small_data + (long)other.small_data >(long)INT32_MIN)))
	{
		small_data += other.small_data;
	}
	else
	{
		if (small)
		{
			small = false;
			big_int = my_vector(small_data);
		}
		my_vector oth(0);
		if (other.small)
		{
			oth = my_vector(other.small_data);
		}
		else
		{
			oth = other.big_int;
		}
		big_int += oth;
	}
	return *this;
}

my_vector& my_vector::operator += (my_vector const& other)
{
	my_vector th (*this);
	my_vector oth (other);
	th.fitSize(oth);
	my_vector answer(0);
	answer.fitSize(oth);
	bool nextOverflow = false;
	for (int i = 0; i < oth.allocated * 32; ++i)
	{
		int pos = i / 32; //position
		int off = i % 32; //offset

		if (!((th.data[pos]) & (1 << off)) && !((oth.data[pos]) & (1 << off)) && !nextOverflow)
		{
			answer.data[pos] &= ~(1 << off);
			nextOverflow = false;
		}
		else if ((!((th.data[pos]) & (1 << off)) && !((oth.data[pos]) & (1 << off)) && nextOverflow) ||
			(!((th.data[pos]) & (1 << off)) && ((oth.data[pos]) & (1 << off)) && !nextOverflow) ||
			(((th.data[pos]) & (1 << off)) && !((oth.data[pos]) & (1 << off)) && !nextOverflow))
		{
			answer.data[pos] |= (1 << off);
			nextOverflow = false;
		}
		else if ((!((th.data[pos]) & (1 << off)) && ((oth.data[pos]) & (1 << off)) && nextOverflow) ||
			(((th.data[pos]) & (1 << off)) && ((oth.data[pos]) & (1 << off)) && !nextOverflow) ||
			(((th.data[pos]) & (1 << off)) && !((oth.data[pos]) & (1 << off)) && nextOverflow))
		{
			answer.data[pos] &= ~(1 << off);
			nextOverflow = true;
		}
		else
		{
			answer.data[pos] |= (1 << off);
			nextOverflow = true;
		}
	}

	if ((neg() == oth.neg()) && (neg() != answer.neg()))
	{
		(answer.allocated)++;
		int * newData = new int[answer.allocated];
		if (neg())
		{
			memcpy(newData, answer.data, 4 * answer.allocated - 1);
			newData[answer.allocated - 1] = -1;
		}
		else
		{
			memcpy(newData, answer.data, 4 * answer.allocated - 1);
			newData[(answer.allocated) - 1] = 0;
		}
		delete[] answer.data;
		answer.data = newData;
	}
	answer.shrink_to_fit();
	*this = answer;
	return *this;
}

//вычитание 

big_integer operator - (big_integer const& a, big_integer const & b)
{
	big_integer tmp (a);
	return tmp -= b;
}

my_vector operator - (my_vector const& a, my_vector const & b)
{
	my_vector tmp(a);
	return tmp -= b;
}

big_integer& big_integer::operator -= (big_integer const & other)
{
	*this += -other;
	return *this;
}

my_vector& my_vector::operator -= (my_vector const & other)
{
	my_vector tmp = -other;
	*this += tmp;
	return *this;
}

//умножение

big_integer operator * (big_integer const& a, big_integer const & b)
{
	big_integer tmp(a);
	return tmp *= b;
}

big_integer& big_integer::operator *= (big_integer const& other)
{
	big_integer a;
	if (*used > 1)
	{
		a = copy();
	}
	else
	{
		a = *this;
	}
	int alen = 0;
	int tmp = a.small_data;
	while (abs(tmp) > 0)
	{
		alen++;
		tmp /= 10;
	}
	int blen = 0;
	tmp = other.small_data;
	while (abs(tmp) > 0)
	{
		blen++;
		tmp /= 10;
	}
	if (a.small && other.small && (alen + blen) < 9)
	{
		a.small_data *= other.small_data;
	}
	else
	{
		if (a.small)
		{
			a.small = false;
			a.big_int = my_vector(a.small_data);
		}
		my_vector oth(0);
		if (other.small)
		{
			oth = my_vector(other.small_data);
		}
		else
		{
			oth = other.big_int;
		}
		a.big_int *= oth;
	}
	*this = a;

	
//	string to_debug = (string)a;


	return *this;
}

my_vector& my_vector::operator *= (my_vector const& other)
{
	my_vector a(*this);
	my_vector b (other);
	if (a.neg())
	{
		a = -a;
	}
	if (b.neg())
	{
		b = -b;
	}
	my_vector answer(0);
	answer.reallocate(a.allocated + b.allocated);

	uint32_t overflow = 0;
	for (int i = 0; i < a.allocated; i++)
	{
		uint32_t x = a.data[i];
		for (int j = 0; j < b.allocated; j++)
		{
			uint32_t y = b.data[j];
			uint64_t ans = (uint64_t)x * (uint64_t)y;
			uint32_t toAns = (uint32_t)(ans);
			my_vector plus (0);
			plus.reallocate(i + j + 2);
			plus.data[plus.allocated - 2] = toAns;
			answer += plus;
			plus.data[plus.allocated - 2] = overflow;
			answer += plus;
			overflow = (uint32_t)(ans >> 32);
		}
		my_vector plus(0);
		plus.reallocate(i + b.allocated + 2);
		plus.data[plus.allocated - 2] = overflow;
		answer = answer + plus;
		overflow = 0;
	}
	if ((pos() && other.neg()) || (neg() && other.pos()))
	{
		answer = -answer;
	}
	answer.shrink_to_fit();
	*this = answer;
	return *this;
}

//ƒеление

big_integer operator / (big_integer const& a, big_integer const & b)
{
	big_integer tmp(a);
	return tmp /= b;
}

my_vector operator / (my_vector const& a, my_vector const& b)
{
	my_vector tmp(a);
	return tmp /= b;
}

my_vector& my_vector::operator /= (my_vector const& other)
{
	my_vector res(0);
	my_vector oth(other);
	divide(oth, res, my_vector(0));
	*this = res;
	return *this;
}

big_integer& big_integer::operator /= (big_integer const& other)
{
	if (small && other.small)
	{
		small_data /= other.small_data;
	}
	else
	{
		my_vector oth(0);
		my_vector res(0);
		my_vector tmp(0);
		if (small)
		{
			tmp = my_vector(small_data);
		}
		else
		{
			tmp = big_int;
		}
		if (other.small)
		{
			oth = my_vector(other.small_data);
		}
		else
		{
			oth = other.big_int;
		}
		tmp.divide(oth, res, my_vector(0));
		small = false;
		big_int= res;
	}
	return *this;
}


big_integer operator % (big_integer const& a, big_integer const & b)
{
	big_integer tmp(a);
	return tmp %= b;
}

my_vector operator % (my_vector const& a, my_vector const & b)
{
	my_vector tmp(a);
	return tmp %= b;
}

my_vector& my_vector::operator %= (my_vector const& other)
{
	my_vector rest(0);
	my_vector oth(other);
	divide(oth, my_vector(0), rest);
	*this = rest;
	return *this;
}

big_integer& big_integer::operator %= (big_integer const& other)
{
	if (small && other.small)
	{
		small_data %= other.small_data;
	}
	else
	{
		my_vector oth(0);
		my_vector rest(0);
		my_vector tmp(0);
		if (small)
		{
			tmp = my_vector(small_data);
		}
		else
		{
			tmp = big_int;
		}
		if (other.small)
		{
			oth = my_vector(other.small_data);
		}
		else
		{
			oth = other.big_int;
		}
		tmp.divide(oth, my_vector(0), rest);
		small = false;
		big_int = rest;
	}
	return *this;
}


void my_vector::divide(my_vector const & oth, my_vector& res, my_vector& rest)
{
	my_vector num (*this);
	my_vector div (oth);
	if (num.neg())
	{
		num = -num;
	}
	if (div.neg())
	{
		div = -div;
	}

	if (num < div)
	{
		rest = num;
		return;
	}
	res.reallocate(num.allocated);
	div.fitSize(num);
	my_vector tmp(0);
	int first_one_position = div.allocated * 32 - 1;
	while (!(div.data[first_one_position / 32] & (1 << (first_one_position % 32))))
	{
		--first_one_position;
	}
	++first_one_position;
	for (int i = 32 * res.allocated - 1 - first_one_position; i >= 0; --i)
	{
		tmp = div << i;
		if (num >= tmp)
		{
			num = num - tmp;
			res.data[i / 32] = (res.data[i / 32] | (1 << (i % 32)));
		}
	}
	if (pos() && oth.neg())
	{
		res = -res;
	}
	else if (neg() && oth.pos())
	{
		num = -num;
		res = -res;
	}
	rest = num;
	res.shrink_to_fit();
	rest.shrink_to_fit();
}

big_integer& big_integer::operator&= (big_integer const & other)
{
	if (small && other.small)
	{
		small_data &= other.small_data;
		return *this;
	}
	else
	{
		big_integer tmp(*this);
		my_vector tmp2(0);
		if (small)
		{
			tmp.big_int = my_vector(small_data);
		}
		else
		{
			tmp.big_int = big_int;
		}
		if (other.small)
		{
			tmp2 = my_vector(other.small_data);
		}
		else
		{
			tmp2 = other.big_int;
		}
		tmp.big_int &= tmp2;
		*this = tmp;
		return *this;
	}
}


my_vector& my_vector::operator &= (my_vector const & other)
{
	my_vector tmp (other);
	my_vector tmpth (*this);
	tmpth.fitSize(tmp);
	my_vector answer(0);
	answer.allocated = tmp.allocated;
	for (int i = 0; i < allocated; ++i)
	{
		answer.data[i] = tmpth.data[i] & tmp.data[i];
	}
	answer.shrink_to_fit();
	*this = answer;
	return *this;
}

/*big_integer& big_integer::operator|= (big_integer const & other)
{
	if (small && other.small)
	{
		small_data |= other.small_data;
		return *this;
	}
	else
	{
		big_integer tmp(*this);
		my_vector tmp2;
		if (small)
		{
			tmp.big_int = my_vector(small_data);
		}
		else
		{
			tmp.big_int = big_int;
		}
		if (other.small)
		{
			tmp2 = my_vector(other.small_data);
		}
		else
		{
			tmp2 = other.big_int;
		}
		tmp.big_int |= tmp2;
		return tmp;
	}
}*/

my_vector& my_vector::operator |= (my_vector const & other)
{
	my_vector tmp (other);
	my_vector tmpth (*this);
	tmpth.fitSize(tmp);
	my_vector answer(0);
	answer.allocated = tmp.allocated;
	for (int i = 0; i < allocated; ++i)
	{
		answer.data[i] = tmpth.data[i] | tmp.data[i];
	}
	answer.shrink_to_fit();
	*this = answer;
	return *this;
}

big_integer& big_integer::operator^= (big_integer const & other)
{
	if (small && other.small)
	{
		small_data ^= other.small_data;
		return *this;
	}
	else
	{
		big_integer tmp(*this);
		my_vector tmp2(0);
		if (small)
		{
			tmp.big_int = my_vector(small_data);
		}
		else
		{
			tmp.big_int = big_int;
		}
		if (other.small)
		{
			tmp2 = my_vector(other.small_data);
		}
		else
		{
			tmp2 = other.big_int;
		}
		tmp.big_int ^= tmp2;
		*this = tmp;
		return *this;
	}
}

my_vector& my_vector::operator ^= (my_vector const & other)
{
	my_vector tmp (other);
	my_vector tmpth (*this);
	tmpth.fitSize(tmp);
	my_vector answer(0);
	answer.allocated = tmp.allocated;
	for (int i = 0; i < allocated; ++i)
	{
		answer.data[i] = tmpth.data[i] ^ tmp.data[i];
	}
	answer.shrink_to_fit();
	*this = answer;
	return *this;
}






//выравнивание размеров двух my_vector-ов
void my_vector::fitSize(my_vector & other)
{

	if (allocated > other.allocated)
	{
		other.reallocate(allocated);
	}
	else if (allocated < other.allocated)
	{
		reallocate(other.allocated);
	}
}

//ѕровер€ет, €вл€етс€ ли число положительным
bool big_integer::pos() const
{
	if (small)
	{
		return small_data > 0;
	}
	else
	{
		return big_int.pos();
	}
}

bool my_vector::neg() const 
{
	if ((data[allocated - 1]) & (1 << 31)) {
		return true;
	}
	else
	{
		return false;
	}
}

//ѕровер€ет, €вл€етс€ ли число отрицательным

bool big_integer::neg() const
{
	if (small)
	{
		return small_data < 0;
	}
	else
	{
		return big_int.neg();
	}
}

bool my_vector::pos() const 
{
	if ((data[allocated - 1]) & (1 << 31)) {
		return false;
	}
	else
	{
		return true;
	}
}

//»зменение размера числа
void my_vector::reallocate(int new_allocated)
{
	bool is_positive = pos();
	
	int * new_data = new int32_t[new_allocated];
	if (is_positive)
	{
		memset(new_data, 0, 4 * new_allocated);
	}
	else
	{
		memset(new_data, -1, 4 * new_allocated);
	}
	memcpy(new_data, data, 4 * allocated);
	allocated = new_allocated;
	delete[] data;
	data = new_data;
}

void my_vector::shrink_to_fit()
{
	int must_be_deleted = 0;
	if (pos())
	{
		for (int i = allocated - 1; i > 0; --i)
		{
			if ((data[i] == 0) && !(data[i - 1] & (1 << 31)))
			{
				++must_be_deleted;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		for (int i = allocated - 1; i > 0; --i)
		{
			if ((data[i] == -1) && (data[i - 1] & (1 << 31)))
			{
				++must_be_deleted;
			}
			else
			{
				break;
			}
		}
	}
	if (must_be_deleted > 0)
	{
		int new_allocated = allocated - must_be_deleted;
		int32_t * new_data = new int32_t[new_allocated];
		memcpy(new_data, data, 4 * new_allocated);
		allocated = new_allocated;
		delete[] data;
		data = new_data;
	}
}




my_vector operator | (my_vector const& a, my_vector const&  b)
{
	my_vector tmp(a);
	return tmp |= b;
}

my_vector operator & (my_vector const& a, my_vector const& b)
{
	my_vector tmp(a);
	return tmp &= b;
}

my_vector operator ^ (my_vector const& a, my_vector const& b)
{
	my_vector tmp(a);
	return tmp ^= b;
}

std::ostream& operator << (std::ostream& s, big_integer const& a){
	s << (string)a;
	return s;
}

std::string to_string(big_integer const& a)
{
	return (string)a;
}
