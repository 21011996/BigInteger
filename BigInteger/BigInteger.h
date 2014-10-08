#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

class BigInteger {
public:
	BigInteger(BigInteger && other);
	BigInteger(int data);
	BigInteger(std::string const & data);
	BigInteger(BigInteger const & other);
	BigInteger();
	BigInteger(bool s, const std::vector & c);
	BigInteger operator+ (BigInteger const & right) const;
	BigInteger operator- (BigInteger const & right) const;
	BigInteger operator* (BigInteger const & right) const;

	static std::vector add(const std::vector& x, const std::vector&y, long long base);
	static std::vector sub(const std::vector& x, const std::vector&y, long long base);
	operator std::string() const;
	BigInteger & operator=(BigInteger const & other);

	vector<int> v;
	bool sign;
};

std::ostream & operator <<(std::ostream & out, BigInteger const & other) {
	return out << (std::string)i;
}
std::istream & operator >>(std::istream & iut, BigInteger & i){
std::string s;
	in >> s;
	i = BigInteger(s);
	return in;
}