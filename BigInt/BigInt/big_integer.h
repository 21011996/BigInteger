#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H
#include <string>
#include <cstdint>
#include "my_vector.h"
using namespace std;

class big_integer
{
	struct unshare_t {} unshare;

public:
	big_integer();
	big_integer(int);
	explicit big_integer(string a);
	big_integer(big_integer const & other);
	big_integer(big_integer const & other, unshare_t);
	~big_integer();
	
	big_integer& operator ++ ();
	big_integer& operator ++ (int);
	big_integer& operator -- ();
	big_integer& operator -- (int);

	big_integer& operator = (big_integer const & other);
//	big_integer& operator = (int);
	
	big_integer& operator + ();
	big_integer operator - () const ;
	big_integer operator ~ ();

	big_integer& operator += (big_integer const & other);
	big_integer& operator -= (big_integer const & other);
	big_integer& operator *= (big_integer const & other);
	big_integer& operator /= (big_integer const & other);
	big_integer& operator %= (big_integer const & other);

	big_integer& operator &= (big_integer const & other);
	big_integer& operator |= (big_integer const & other);
	big_integer& operator ^= (big_integer const & other);

	big_integer& operator >>= (int other);
	big_integer& operator <<= (int other);

	big_integer& operator ! ();
	
	void fitSize(big_integer & other); // TODO: private
	void reallocate(int new_allocate);
	void shrink_to_fit();
	void divide(big_integer const& other, big_integer& res, big_integer& rest);
	bool neg() const;
	bool pos() const;
	operator string() const; // TODO: remove

	friend bool operator < (big_integer &, big_integer &);
	friend bool operator == (big_integer const &, big_integer const &);
	friend std::ostream& operator << (std::ostream & , big_integer const & );

	
private:
	bool small;
	my_vector big_int;
	int small_data;
	
	mutable unsigned int * used;

	big_integer big_integer::copy() const;
};

big_integer operator + (big_integer const & a, big_integer const & b);
big_integer operator - (big_integer const & a, big_integer const& b);
big_integer operator * (big_integer const & a, big_integer const& b);
big_integer operator / (big_integer const & a, big_integer const& b);
big_integer operator % (big_integer const & a, big_integer const& b);

big_integer operator & (big_integer const & a, big_integer const& b);
big_integer operator | (big_integer const & a, big_integer const& b);
big_integer operator ^ (big_integer const & a, big_integer const& b);

big_integer operator >> (big_integer & a, int b);
big_integer operator << (big_integer & a, int b);

bool operator < (big_integer const & a, big_integer const & b);
bool operator > (big_integer  & a, big_integer & b);
bool operator <= (big_integer & a, big_integer & b);
bool operator >= (big_integer & a, big_integer & b);
bool operator == (big_integer const & a, big_integer const & b);
bool operator != (big_integer & a, big_integer & b);

std::string to_string(big_integer const& a);
std::ostream& operator << (std::ostream& s, big_integer const& a);

#endif // big_integer_H
