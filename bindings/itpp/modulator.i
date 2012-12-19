%module(package="wireless.itpp") modulator

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
#include <itpp/base/vec.h>
#include <itpp/comm/modulator.h>
using namespace itpp;
%}

%import "base_vec.i"

namespace itpp
{

enum Soft_Method {
  LOGMAP,   //!< Log-MAP full calculation
  APPROX   //!< Approximate faster method
};

template <typename T>
class Modulator
{
public:
  Modulator();
  Modulator(const Vec<T>& symbols, const ivec& bits2symbols);
  virtual ~Modulator() {}

  virtual void set(const Vec<T>& symbols, const ivec& bits2symbols);

  virtual int bits_per_symbol() const { return k; }
  virtual Vec<T> get_symbols() const { return symbols; }
  virtual ivec get_bits2symbols() const { return bits2symbols; }

  virtual void modulate(const ivec& symbolnumbers, Vec<T>& output) const;
  virtual Vec<T> modulate(const ivec& symbolnumbers) const;

  virtual void demodulate(const Vec<T>& signal, ivec& output) const;
  virtual ivec demodulate(const Vec<T>& signal) const;

  virtual void modulate_bits(const bvec& bits, Vec<T>& output) const;
  virtual Vec<T> modulate_bits(const bvec& bits) const;

  virtual void demodulate_bits(const Vec<T>& signal, bvec& bits) const;
  virtual bvec demodulate_bits(const Vec<T>& signal) const;

  virtual void demodulate_soft_bits(const Vec<T>& rx_symbols, double N0,
                                    vec& soft_bits,
                                    Soft_Method method = LOGMAP) const;
  virtual vec demodulate_soft_bits(const Vec<T>& rx_symbols, double N0,
                                   Soft_Method method = LOGMAP) const;

  virtual void demodulate_soft_bits(const Vec<T>& rx_symbols,
                                    const Vec<T>& channel,
                                    double N0, vec& soft_bits,
                                    Soft_Method method = LOGMAP) const;
  virtual vec demodulate_soft_bits(const Vec<T>& rx_symbols,
                                   const Vec<T>& channel,
                                   double N0,
                                   Soft_Method method = LOGMAP) const;
};


// ----------------------------------------------------------------------
// Type definitions of Modulator_1D and Modulator_2D
// ----------------------------------------------------------------------

typedef Modulator<double> Modulator_1D;
%shared_ptr(Modulator<double>);
%template(Modulator_1D) Modulator<double>;

typedef Modulator<std::complex<double> > Modulator_2D;
%shared_ptr(Modulator<std::complex<double> >);
%template(Modulator_2D) Modulator<std::complex<double> >;

%shared_ptr(QAM);
class QAM : public Modulator<std::complex<double> >
{
public:
  QAM() {}
  QAM(int M) { set_M(M); }
  virtual ~QAM() { }
  void set_M(int M);

  void demodulate_bits(const cvec& signal, bvec& bits) const;
  bvec demodulate_bits(const cvec& signal) const;
};

%shared_ptr(PSK)
class PSK : public Modulator<std::complex<double> >
{
public:
  PSK() {}
  PSK(int M) { set_M(M); }
  virtual ~PSK() { }
  void set_M(int M);

  void demodulate_bits(const cvec& signal, bvec& bits) const;
  bvec demodulate_bits(const cvec& signal) const;
};

%shared_ptr(QPSK)
class QPSK : public PSK
{
public:
  //! Class Constructor
  QPSK(): PSK(4) {}
  //! Destructor
  virtual ~QPSK() {}

  virtual void demodulate_soft_bits(const cvec& rx_symbols, double N0,
                                    vec& soft_bits,
                                    Soft_Method method = LOGMAP) const;
  vec demodulate_soft_bits(const cvec& rx_symbols, double N0,
                           Soft_Method method = LOGMAP) const;

  virtual void demodulate_soft_bits(const cvec& rx_symbols,
                                    const cvec& channel, double N0,
                                    vec& soft_bits,
                                    Soft_Method method = LOGMAP) const;
  vec demodulate_soft_bits(const cvec& rx_symbols, const cvec& channel,
                           double N0, Soft_Method method = LOGMAP) const;
};


%shared_ptr(BPSK_c)
class BPSK_c : public PSK
{
public:
  BPSK_c(): PSK(2) {}
  virtual ~BPSK_c() {}

  void modulate_bits(const bvec& bits, cvec& output) const;
  cvec modulate_bits(const bvec& bits) const;
  void demodulate_bits(const cvec& signal, bvec& output) const;
  bvec demodulate_bits(const cvec& signal) const;

  virtual void demodulate_soft_bits(const cvec& rx_symbols, double N0,
                                    vec& soft_bits,
                                    Soft_Method method = LOGMAP) const;
  vec demodulate_soft_bits(const cvec& rx_symbols, double N0,
                           Soft_Method method = LOGMAP) const;

  virtual void demodulate_soft_bits(const cvec& rx_symbols,
                                    const cvec& channel, double N0,
                                    vec& soft_bits,
                                    Soft_Method method = LOGMAP) const;
  vec demodulate_soft_bits(const cvec& rx_symbols, const cvec& channel,
                           double N0, Soft_Method method = LOGMAP) const;
};


%shared_ptr(BPSK)
class BPSK : public Modulator<double>
{
public:
  BPSK(): Modulator<double>("1.0 -1.0", "0 1") {}
  virtual ~BPSK() {}

  void modulate_bits(const bvec& bits, vec& output) const;
  vec modulate_bits(const bvec& bits) const;
  void demodulate_bits(const vec& signal, bvec& output) const;
  bvec demodulate_bits(const vec& signal) const;

  virtual void demodulate_soft_bits(const vec& rx_symbols, double N0,
                                    vec& soft_bits,
                                    Soft_Method method = LOGMAP) const;
  vec demodulate_soft_bits(const vec& rx_symbols, double N0,
                           Soft_Method method = LOGMAP) const;

  virtual void demodulate_soft_bits(const vec& rx_symbols,
                                    const vec& channel, double N0,
                                    vec& soft_bits,
                                    Soft_Method method = LOGMAP) const;
  vec demodulate_soft_bits(const vec& rx_symbols, const vec& channel,
                           double N0, Soft_Method method = LOGMAP) const;
};

%shared_ptr(PAM_c)
class PAM_c : public Modulator<std::complex<double> >
{
public:
  PAM_c() {}
  PAM_c(int M) { set_M(M); }
  virtual ~PAM_c() {}
  void set_M(int M);

  void demodulate_bits(const cvec& signal, bvec& output) const;
  bvec demodulate_bits(const cvec& signal) const;

  virtual void demodulate_soft_bits(const cvec& rx_symbols, double N0,
                                    vec& soft_bits,
                                    Soft_Method method = LOGMAP) const;
  virtual vec demodulate_soft_bits(const cvec& rx_symbols, double N0,
                                   Soft_Method method = LOGMAP) const;
  virtual void demodulate_soft_bits(const cvec& rx_symbols,
                                    const cvec& channel, double N0,
                                    vec& soft_bits,
                                    Soft_Method method = LOGMAP) const;
  virtual vec demodulate_soft_bits(const cvec& rx_symbols,
                                   const cvec& channel, double N0,
                                   Soft_Method method = LOGMAP) const;
};

%shared_ptr(PAM)
class PAM : public Modulator<double>
{
public:
  PAM() {}
  PAM(int M) { set_M(M); }
  virtual ~PAM() {}
  void set_M(int M);
  void demodulate_bits(const vec& signal, bvec& output) const;
  bvec demodulate_bits(const vec& signal) const;
};

} // namespace itpp
 