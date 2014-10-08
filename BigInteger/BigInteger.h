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
	BigInteger(bool s, const std::vector<int> & c);


	BigInteger& operator += (BigInteger const & other);
	BigInteger& operator -= (BigInteger const & other);
	BigInteger& operator *= (BigInteger const & other);
	BigInteger& operator = (BigInteger const & other);

	operator std::string() const;
	
	std::vector<int> v;
	bool sign;
};

/*std::ostream & operator <<(std::ostream & out, BigInteger const & other) {
	return out << (std::string)i;
}
std::istream & operator >>(std::istream & iut, BigInteger & i){
std::string s;
	in >> s;
	i = BigInteger(s);
	return in;
}*/

BigInteger operator + (BigInteger const & a, BigInteger const & b);
BigInteger operator - (BigInteger const & a, BigInteger const& b);
BigInteger operator * (BigInteger const & a, BigInteger const& b);