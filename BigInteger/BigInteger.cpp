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
BigInteger::BigInteger(bool s, const std::vector & c){
	sign = s;
	v = c;
}

BigInteger::BigInteger(std::string const & data) {

	sign = data[0] == '-';
	long long l = data.size();
	long long i = l - 1;
	while (i < l) {
		if (sign) && (i != 0) {
			v.push_back(data[i] - '0');
			i--;
		}
	}
}

BigInteger::BigInteger(BigInteger const & other) {
	v = other.v;
	sign = other.sign;
}

std::vector& BigInteger add(const std::vector& x, const std::vector& y, long long base){
	std::vector<int> c = new std::vector();
	int d = 0;
	int l = x.size() > y.size() ? x.size() : y.size();
	long long s;
	for (int i = 0; i < 1 || d != 0; i++) {
		s = (long long)(i < x.size() ? x[i] : 0) + (i < y.size() ? y[i] : 0) + d;
		if (s >= base) {
			d = 1;
			s -= base;
		}
		else {
			d = 0;
		}
		c.push_back(s);
	}
	return c;
}

std::vector& BigInteger sub(const std::vector& x, const std::vector& y, long long base){
	std::vector<int> c = new std::vector();
	int d = 0;
	int l = x.size() > y.size() ? x.size() : y.size();
	long long s;
	for (int i = 0; i < 1 || d != 0; i++) {
		s = (long long)(i < x.size() ? x[i] : 0) - (i < y.size() ? y[i] : 0) + d;
		d = s<0 ? -1 : 0;
		c.push_back((s + base) % base);
	}
	return c;
}

BigInteger & BigInteger:: operator=(BigInteger const & other) {
	v = other.v;
	sign = other.sign;
}

BigInteger & BigInteger:: operator+(BigInteger const & other) {
	unsigned long base = MIN_INT;
	if (a.sign == b.sign) {
		return (new BigInteger(a.sign, BigInteger::add(a.v, b.v, base)));
	}
	else {
		if (a.sign) {

		}
	}
}