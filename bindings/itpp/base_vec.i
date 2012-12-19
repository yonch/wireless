%module(package="wireless.itpp") base_vec

%import "std_string.i"

///////////////////////
// Exception handling
///////////////////////
%include exception.i

%exception {
	try {
		$action
	}
	SWIG_CATCH_STDEXCEPT
	catch (...) {
     SWIG_exception(SWIG_UnknownError, "Unknown exception");
    }
}

//////////////////
// General code-related
//////////////////
%{
#include <itpp/base/binary.h>
#include <itpp/base/vec.h>
%}

%ignore std::abs;
%ignore itpp::bin::operator=;
%ignore itpp::bin::operator!;
%ignore itpp::bin::operator short;
%ignore itpp::bin::operator int;
%ignore itpp::bin::operator float;
%ignore itpp::bin::operator double;
%ignore itpp::bin::operator <<;
%include <itpp/base/binary.h>

namespace itpp
{

class Factory;


// Declaration of Vec
template<class Num_T> class Vec;
// Declaration of Mat
template<class Num_T> class Mat;
// Declaration of bin
class bin;

//-----------------------------------------------------------------------------------
// Declaration of Vec Friends
//-----------------------------------------------------------------------------------


//! Inner (dot) product of two vectors v1 and v2
template<class Num_T>
Num_T dot(const Vec<Num_T> &v1, const Vec<Num_T> &v2);
//! Outer product of two vectors v1 and v2
template<class Num_T>
Mat<Num_T> outer_product(const Vec<Num_T> &v1, const Vec<Num_T> &v2,
                         bool hermitian = false);

//! Element-wise multiplication of two vectors
template<class Num_T>
Vec<Num_T> elem_mult(const Vec<Num_T> &a, const Vec<Num_T> &b);
//! Element-wise multiplication of three vectors
template<class Num_T>
Vec<Num_T> elem_mult(const Vec<Num_T> &a, const Vec<Num_T> &b,
                     const Vec<Num_T> &c);
//! Element-wise multiplication of four vectors
template<class Num_T>
Vec<Num_T> elem_mult(const Vec<Num_T> &a, const Vec<Num_T> &b,
                     const Vec<Num_T> &c, const Vec<Num_T> &d);

//! Element-wise multiplication of two vectors, storing the result in vector \c out
template<class Num_T>
void elem_mult_out(const Vec<Num_T> &a, const Vec<Num_T> &b,
                   Vec<Num_T> &out);
//! Element-wise multiplication of three vectors, storing the result in vector \c out
template<class Num_T>
void elem_mult_out(const Vec<Num_T> &a, const Vec<Num_T> &b,
                   const Vec<Num_T> &c, Vec<Num_T> &out);
//! Element-wise multiplication of four vectors, storing the result in vector \c out
template<class Num_T>
void elem_mult_out(const Vec<Num_T> &a, const Vec<Num_T> &b,
                   const Vec<Num_T> &c, const Vec<Num_T> &d,
                   Vec<Num_T> &out);

//! In-place element-wise multiplication of two vectors. Faster version of b = elem_mult(a,b).
template<class Num_T>
void elem_mult_inplace(const Vec<Num_T> &a, Vec<Num_T> &b);
//! Element-wise multiplication of two vectors, followed by summation of the resultant elements. Fast version of sum(elem_mult(a,b)).
template<class Num_T>
Num_T elem_mult_sum(const Vec<Num_T> &a, const Vec<Num_T> &b);


//! Elementwise division of two vectors
template<class Num_T>
Vec<Num_T> elem_div(const Vec<Num_T> &a, const Vec<Num_T> &b);
//! This function is deprecated. Please use operator/(Num_T, const Vec<Num_T &) instead.
template<class Num_T>
Vec<Num_T> elem_div(Num_T t, const Vec<Num_T> &v);
//! Elementwise division of two vectors, storing the result in vector \c out
template<class Num_T>
void elem_div_out(const Vec<Num_T> &a, const Vec<Num_T> &b, Vec<Num_T> &out);
//! Elementwise division of two vectors, followed by summation of the resultant elements. Fast version of sum(elem_div(a,b))
template<class Num_T>
Num_T elem_div_sum(const Vec<Num_T> &a, const Vec<Num_T> &b);

//! Append element \c a to the end of the vector \c v
template<class Num_T>
Vec<Num_T> concat(const Vec<Num_T> &v, Num_T a);
//! Concat element \c a to the beginning of the vector \c v
template<class Num_T>
Vec<Num_T> concat(Num_T a, const Vec<Num_T> &v);
//! Concat vectors \c v1 and \c v2
template<class Num_T>
Vec<Num_T> concat(const Vec<Num_T> &v1, const Vec<Num_T> &v2);
//! Concat vectors \c v1, \c v2 and \c v3
template<class Num_T>
Vec<Num_T> concat(const Vec<Num_T> &v1, const Vec<Num_T> &v2,
                  const Vec<Num_T> &v3);
//! Concat vectors \c v1, \c v2, \c v3 and \c v4
template<class Num_T>
Vec<Num_T> concat(const Vec<Num_T> &v1, const Vec<Num_T> &v2,
                  const Vec<Num_T> &v3, const Vec<Num_T> &v4);
//! Concat vectors \c v1, \c v2 \c v3, \c v4 and \c v5
template<class Num_T>
Vec<Num_T> concat(const Vec<Num_T> &v1, const Vec<Num_T> &v2,
                  const Vec<Num_T> &v3, const Vec<Num_T> &v4,
                  const Vec<Num_T> &v5);

//-----------------------------------------------------------------------------------
// Declaration of Vec
//-----------------------------------------------------------------------------------


template<class Num_T>
class Vec
{
public:
  //! The type of the vector values
  typedef Num_T value_type;

  //! Default constructor. An element factory \c f can be specified.
  explicit Vec(const Factory &f = DEFAULT_FACTORY);
  //! Constructor with size parameter. An element factory \c f can be specified.
  explicit Vec(int size, const Factory &f = DEFAULT_FACTORY);
  //! Copy constructor
  Vec(const Vec<Num_T> &v);
  //! Copy constructor, which takes an element factory \c f as an additional argument.
  Vec(const Vec<Num_T> &v, const Factory &f);
  //! Constructor taking a char string as input. An element factory \c f can be specified.
  Vec(const char *str, const Factory &f = DEFAULT_FACTORY);
  //! Constructor taking a string as input. An element factory \c f can be specified.
  Vec(const std::string &str, const Factory &f = DEFAULT_FACTORY);
  //! Constructor taking a C-array as input. Copies all data. An element factory \c f can be specified.
  Vec(const Num_T *c_array, int size, const Factory &f = DEFAULT_FACTORY);

  //! Destructor
  ~Vec();

  //! The size of the vector
  int length() const { return datasize; }
  //! The size of the vector
  int size() const { return datasize; }

  //! Set length of vector. if copy = true then keeping the old values
  void set_size(int size, bool copy = false);
  //! Set length of vector. if copy = true then keeping the old values
  void set_length(int size, bool copy = false) { set_size(size, copy); }
  //! Set the vector to the all zero vector
  void zeros();
  //! Set the vector to the all zero vector
  void clear() { zeros(); }
  //! Set the vector to the all one vector
  void ones();
  //! Set the vector equal to the values in the \c str string
  void set(const char *str);
  //! Set the vector equal to the values in the \c str string
  void set(const std::string &str);


  //! Index operator. First element is 0.
  Num_T &operator()(int i);
  //! Sub-vector with elements from \c i1 to \c i2. Index -1 indicates the last element.
  Vec<Num_T> operator()(int i1, int i2) const;
  //! Sub-vector with elements given by the list of indices \c indexlist
  //Vec<Num_T> operator()(const Vec<int> &indexlist) const;
  //! Sub-vector with elements with indexes where \c binlist is \c 1
  //Vec<Num_T> operator()(const Vec<bin> &binlist) const;

  //! Accessor-style method. First element is 0.
  const Num_T &get(int i) const;
  //! Get the elements from \c i1 to \c i2. Index -1 indicates the last element.
  Vec<Num_T> get(int i1, int i2) const;
  //! Get the elements given by the list of indices \c indexlist
  //Vec<Num_T> get(const Vec<int> &indexlist) const;
  //! Get the elements with indexes where \c binlist is \c 1
  //Vec<Num_T> get(const Vec<bin> &binlist) const;

  //! Modifier-style method. First element is 0.
  void set(int i, Num_T t);

  //! Matrix transpose. Converts to a matrix with the vector in the first row
  Mat<Num_T> transpose() const;
  //! Matrix transpose. Converts to a matrix with the vector in the first row
  Mat<Num_T> T() const { return this->transpose(); }
  //! Hermitian matrix transpose. Converts to a matrix with the conjugate of the vector in the first row
  Mat<Num_T> hermitian_transpose() const;
  //! Hermitian matrix transpose. Converts to a matrix with the conjugate of the vector in the first row
  Mat<Num_T> H() const { return this->hermitian_transpose(); }

  //! Addition of vector
  Vec<Num_T>& operator+=(const Vec<Num_T> &v);
  //! Addition of scalar
  Vec<Num_T>& operator+=(Num_T t);

  //! Subtraction of vector
  Vec<Num_T>& operator-=(const Vec<Num_T> &v);
  //! Subtraction of scalar
  Vec<Num_T>& operator-=(Num_T t);

  //! Multiply with a scalar
  Vec<Num_T>& operator*=(Num_T t);
  
  //! Elementwise division
  Vec<Num_T>& operator/=(Num_T t);
  //! Elementwise division
  Vec<Num_T>& operator/=(const Vec<Num_T> &v);

  //! Get the right \c nr elements from the vector
  Vec<Num_T> right(int nr) const;
  //! Get the left \c nr elements from the vector
  Vec<Num_T> left(int nr) const;
  //! Get the middle part of vector from \c start including \c nr elements
  Vec<Num_T> mid(int start, int nr) const;
  /*!
   * \brief Split the vector into two parts at element \c pos.
   *
   * Return the first part containing elements 0 to \c pos-1, and keep the
   * second part containing the remaining elements starting from \c pos
   * (empty vector if \c pos is equal to the length of the vector).
   */
  Vec<Num_T> split(int pos);
  //! Shift in element \c t at position 0 \c n times
  void shift_right(Num_T t, int n = 1);
  //! Shift in vector \c v at position 0
  void shift_right(const Vec<Num_T> &v);
  //! Shift out the \c n left elements and at the same time shift in the element \c t at last position \c n times
  void shift_left(Num_T t, int n = 1);
  //! Shift in vector \c v at last positions
  void shift_left(const Vec<Num_T> &v);

  //! This function is deprecated. Please use set_subvector(i, v) instead.
  void set_subvector(int i1, int i2, const Vec<Num_T> &v);
  //! Set subvector to elements of vector \c v starting from element \c i
  void set_subvector(int i, const Vec<Num_T> &v);
  //! Set subvector defined by indices \c i1 and \c i2 to \c t
  void set_subvector(int i1, int i2, Num_T t);
  //! An alias function of set_subvector(i, &v)
  void replace_mid(int i, const Vec<Num_T> &v);
  //! Insert element \c t before element with index \c i (0 <= i <= datasize)
  void ins(int i, Num_T t);
  //! Insert vector \c v before element with index \c i (0 <= i <= datasize)
  void ins(int i, const Vec<Num_T> &v);

  //! Elementwise equal to the scalar \c t
  Vec<bin> operator==(Num_T t) const;
  //! Elementwise not-equal to the scalar \c t
  Vec<bin> operator!=(Num_T t) const;
  //! Elementwise less than the scalar \c t
  Vec<bin> operator<(Num_T t) const;
  //! Elementwise less than and equal to the scalar \c t
  Vec<bin> operator<=(Num_T t) const;
  //! Elementwise greater than the scalar \c t
  Vec<bin> operator>(Num_T t) const;
  //! Elementwise greater than and equal to the scalar \c t
  Vec<bin> operator>=(Num_T t) const;

  //! Compare with vector \c v. Return false if sizes or values differ.
  bool operator==(const Vec<Num_T> &v) const;
  //! Compare with vector \c v. Return true if sizes or values differ.
  bool operator!=(const Vec<Num_T> &v) const;
};

%{
#include <sstream>
%}

%extend Vec {
	// Container implementation
	unsigned int __len__() const {return self->size();}
	
	Num_T __getitem__(unsigned int ind) {
		if(ind >= (unsigned int)self->size()) {
			throw(std::out_of_range("Index too large"));
		}
		return (*self)[ind];
	}
	
	void __setitem__(unsigned int ind, Num_T t) {self->set(ind,t);}
	void __delitem__(unsigned int ind) {self->del(ind);}
	

	// Addition
	Vec<Num_T> __add__(const Vec<Num_T> &v2) const {return (*self) + v2;}
	Vec<Num_T> __add__(Num_T t) const {return (*self) + t;}
	Vec<Num_T> __radd__(Num_T t) const {return t + (*self);}
	
	// Subtraction
	Vec<Num_T> __sub__(const Vec<Num_T> &v2) const {return (*self) - v2;}
	Vec<Num_T> __sub__(Num_T t) const {return (*self) - t;}
	Vec<Num_T> __rsub__(Num_T t) const {return t - (*self);}

	// Negation
	Vec<Num_T> __neg__() const {return (-(*self));}

	//! Inner (dot) product of two vectors v1 and v2
	Num_T __mul__(const Vec<Num_T> &v2) const {return (*self) * v2;}

	//! Multiplication of a vector and a scalar
	Vec<Num_T> __mul__(Num_T t) const {return (*self) * t;}
	Vec<Num_T> __rmul__(Num_T t) const {return t * (*self);}

	//! Division of all elements in \c v with \c t
	Vec<Num_T> __div__(Num_T t) const {return (*self) / t;}
	//! Division of \c t with all elements in \c v
	Vec<Num_T> __rdiv__(Num_T t) const {return t / (*self);}
	
	// String representation
	std::string __str__() {
		std::stringstream strstr(std::stringstream::out);
		strstr << (*self);
		return strstr.str();
	}
};
  //-----------------------------------------------------------------------------------
  // Type definitions of vec, cvec, ivec, svec, and bvec
  //-----------------------------------------------------------------------------------
  typedef Vec<double> vec;
  typedef Vec<std::complex<double> > cvec;
  typedef Vec<int> ivec;
  typedef Vec<short int> svec;
  typedef Vec<bin> bvec;

%template(vec) Vec<double>;
%template(ivec) Vec<int>;
%template(svec) Vec<short int>;
%template(cvec) Vec<std::complex<double> >;
%template(bvec) Vec<bin>;

} // namespace itpp
