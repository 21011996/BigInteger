#include "stdafx.h"
#include "BigInteger.h"
#include <vector>
#include <iostream>

BigInteger::BigInteger(){
	sign = false;
	v.push_back(0);
}

BigInteger::BigInteger(int data){
	sign = data < 0;
	data = data < 0 ? -data : data;
	while (data > 0) {
		v.push_back(data % 10);
		data = data / 10;
	}

}
BigInteger::BigInteger(bool s, const std::vector<int> & c){
	sign = s;
	v = c;
}

BigInteger::BigInteger(std::string const & data) {

	sign = data[0] == '-';
	long long l = data.size();
	long long i = l - 1;
	while (i < l) {
		if ((sign) && (i != 0)) {
			v.push_back(data[i] - '0');
			i--;
		}
	}
}

BigInteger::BigInteger(BigInteger const & other) {
	v = other.v;
	sign = other.sign;
}

BigInteger & BigInteger:: operator=(BigInteger const & other) {
	v = other.v;
	sign = other.sign;
}

BigInteger& BigInteger::operator += (BigInteger const& other)
{
}
BigInteger& BigInteger::operator -= (BigInteger const & other)
{
}
BigInteger& BigInteger::operator*= (BigInteger const & other)
{
}
BigInteger operator + (BigInteger const & b)
{
}
BigInteger operator - (BigInteger const& b)
{
}
BigInteger operator * (BigInteger const& b)
{
}