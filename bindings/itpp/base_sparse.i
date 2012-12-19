%module(package="wireless.itpp") base_sparse

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

/////////////////
// Smart pointers
/////////////////
#define SWIG_SHARED_PTR_SUBNAMESPACE tr1
%include <std_shared_ptr.i>
%{
#include <tr1/memory>
%}


//////////////////
// General code-related
//////////////////
%{
#include <itpp/base/smat.h>
#include <itpp/base/svec.h>
#include <itpp/base/gf2mat.h>
using namespace itpp;
%}

%import "base_vec.i"

namespace itpp {

	template <class T>
	class Sparse_Vec
	{
	public:
	  Sparse_Vec();
	  Sparse_Vec(int sz, int data_init = 200);
	  Sparse_Vec(const Sparse_Vec<T> &v);
	  Sparse_Vec(const Vec<T> &v);
	  Sparse_Vec(const Vec<T> &v, T epsilon);
	  ~Sparse_Vec();
	  void set_size(int sz, int data_init = -1);
	  int size() const { return v_size; }
	  inline int nnz();
	  double density();
	  void set_small_element(const T& epsilon);
	  void remove_small_elements();
	  void resize_data(int new_size);
	  void compact();
	  void full(Vec<T> &v) const;
	  Vec<T> full() const;
	  T operator()(int i) const;
	  void set(int i, T v);
	  void set(const ivec &index_vec, const Vec<T> &v);
	  void set_new(int i, T v);
	  void set_new(const ivec &index_vec, const Vec<T> &v);
	  void add_elem(const int i, const T v);
	  void add(const ivec &index_vec, const Vec<T> &v);
	  void zeros();
	  void zero_elem(const int i);
	  void clear();
	  void clear_elem(const int i);
	  inline void get_nz_data(int p, T& data_out);
	  inline T get_nz_data(int p);
	  inline int get_nz_index(int p);
	  inline void get_nz(int p, int &idx, T &dat);
	  ivec get_nz_indices();
	  Sparse_Vec<T> get_subvector(int i1, int i2) const;
	  T sqr() const;
	};
	
	
	template <class T>
	class Sparse_Mat
	{
	public:
	  Sparse_Mat();
	  Sparse_Mat(int rows, int cols, int row_data_init = 200);
	  Sparse_Mat(const Sparse_Mat<T> &m);
	  Sparse_Mat(const Mat<T> &m);
	  Sparse_Mat(const Mat<T> &m, T epsilon);
	  ~Sparse_Mat();
	  void set_size(int rows, int cols, int row_data_init = -1);
	  int rows() const { return n_rows; }
	  int cols() const { return n_cols; }
	  int nnz();
	  double density();
	  void compact();
	  void full(Mat<T> &m) const;
	  Mat<T> full() const;
	  T operator()(int r, int c) const;
	  void set(int r, int c, T v);
	  void set_new(int r, int c, T v);
	  void add_elem(const int r, const int c, const T v);
	  void zeros();
	  void zero_elem(const int r, const int c);
	  void clear();
	  void clear_elem(const int r, const int c);
	  void set_submatrix(int r1, int r2, int c1, int c2, const Mat<T> &m);
	  void set_submatrix(int r, int c, const Mat<T>& m);
	  Sparse_Mat<T> get_submatrix(int r1, int r2, int c1, int c2) const;
	  Sparse_Mat<T> get_submatrix_cols(int c1, int c2) const;
	  void get_col(int c, Sparse_Vec<T> &v) const;
	  Sparse_Vec<T> get_col(int c) const;
	  void set_col(int c, const Sparse_Vec<T> &v);
	  Sparse_Mat<T> transpose() const;
	};
	
	%template(GF2vec_sparse_my) Sparse_Vec<bin>;
	%template(GF2mat_sparse) Sparse_Mat<bin>;
} // namespace itpp
//%ignore operator<<(std::ostream &os, const LDPC_Code &C);


%ignore GF2mat::operator=(const GF2mat &X);
%ignore operator*(const GF2mat &X, const GF2mat &Y);
%ignore operator*(const GF2mat &X, const bvec &y);
%ignore operator+(const GF2mat &X, const GF2mat &Y);
%ignore operator<<(std::ostream &os, const GF2mat &X);
%ignore operator<<(it_file &f, const GF2mat &X);
%ignore operator>>(it_ifile &f, GF2mat &X);
%ignore mult_trans(const GF2mat &X, const GF2mat &Y);


%include <itpp/base/gf2mat.h>
