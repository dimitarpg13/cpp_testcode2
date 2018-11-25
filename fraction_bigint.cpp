#include <limits>
#include <cstdlib>
#include <type_traits>
#include <cstring>
#include <iostream>

enum ErrorCondition {
  None=0,
  Overflow=1,
  NaN=2,
  Inf=4
};

typedef int SGN_t;
typedef unsigned int PRIM_t;
typedef unsigned long long BUF_t;
class Unsigned128 {
public:
   constexpr Unsigned128(PRIM_t iv=0) : vals_{iv, 0, 0, 0} {
       static_assert( std::is_integral<PRIM_t>::value , "PRIM_t is not an integral type" );
       static_assert( ! std::is_signed<PRIM_t>::value , "PRIM_t is signed integral type" );
       static_assert( std::is_integral<BUF_t>::value , "BUF_t is not an integral type" );
       static_assert( ! std::is_signed<BUF_t>::value , "BUF_t is signed integral type" );
       static_assert( 2 * sizeof (PRIM_t) == sizeof (BUF_t) , "BUF_t is not 2 times the size of PRIM_t" );
   }

   constexpr Unsigned128(PRIM_t v1, PRIM_t v2, PRIM_t v3, PRIM_t v4) : vals_{v1, v2, v3, v4} 
   {

       static_assert( std::is_integral<PRIM_t>::value , "PRIM_t is not an integral type" );
       static_assert( ! std::is_signed<PRIM_t>::value , "PRIM_t is signed integral type" );
       static_assert( std::is_integral<BUF_t>::value , "BUF_t is not an integral type" );
       static_assert( ! std::is_signed<BUF_t>::value , "BUF_t is signed integral type" );
       static_assert( 2 * sizeof (PRIM_t) == sizeof (BUF_t) , "BUF_t is not 2 times the size of PRIM_t");
   }

   constexpr Unsigned128(const Unsigned128& src) : vals_{src.vals_[0], src.vals_[1], src.vals_[2], src.vals_[3]} 
   {

       static_assert( std::is_integral<PRIM_t>::value , "PRIM_t is not an integral type" );
       static_assert( ! std::is_signed<PRIM_t>::value , "PRIM_t is signed integral type" );
       static_assert( std::is_integral<BUF_t>::value , "BUF_t is not an integral type" );
       static_assert( ! std::is_signed<BUF_t>::value , "BUF_t is signed integral type" );
       static_assert( 2 * sizeof (PRIM_t) == sizeof (BUF_t) , "BUF_t is not 2 times the size of PRIM_t");
   }
   Unsigned128(Unsigned128&& src) = default;

   Unsigned128& operator=(const Unsigned128& src);

   Unsigned128& operator*=(const Unsigned128& src);
  
   Unsigned128& operator/=(const Unsigned128& src);
  
   Unsigned128& operator+=(const Unsigned128& src);

   Unsigned128& operator-=(const Unsigned128& src);

   Unsigned128 operator*(const Unsigned128& src) const;

   Unsigned128 operator+(const Unsigned128& src) const;

   Unsigned128 operator-(const Unsigned128& src) const;
  
   Unsigned128 operator/(const Unsigned128& src) const;

   template <typename T>
      Unsigned128 operator*(const T& src) const;

   template <typename T>
      Unsigned128 operator/(const T& src) const;

   template <typename T>
      Unsigned128 operator+(const T& src) const;

   template <typename T>
      Unsigned128 operator-(const T& src) const;

   Unsigned128 operator%(const Unsigned128& src) const;

   bool operator>(const Unsigned128& src) const;

   bool operator<(const Unsigned128& ) const;

   bool operator>=(const Unsigned128& ) const;

   bool operator<=(const Unsigned128& ) const;

   bool operator==(const Unsigned128& other) const;

   bool operator!=(const Unsigned128& ) const;

   friend std::ostream & operator << (std::ostream &out, const Unsigned128& );
   static const Unsigned128& max_val();
   static const Unsigned128& min_val(); 
private:
   static const Unsigned128 max_val_;
   static const Unsigned128 min_val_;
   static constexpr int el_count_ = 4;
   PRIM_t vals_[el_count_];
};

const Unsigned128 Unsigned128::max_val_ = Unsigned128(-1, -1, -1, -1);
const Unsigned128 Unsigned128::min_val_ = Unsigned128(0);

typedef unsigned long long VAL_t;
typedef int ERR_t;

template <typename T> constexpr
T min_limits(std::false_type is_integral) {
  return T::min_val();
}

template <typename T> constexpr
T min_limits(std::true_type is_integral) {
  return std::numeric_limits<T>::min();
}

template <typename T> constexpr
T min_limits() {
  return min_limits<T>(std::is_integral<T>());
}

template <typename T> constexpr
T max_limits(std::false_type is_integral) {
  return T::max_val(); 
}

template <typename T> constexpr
T max_limits(std::true_type is_integral) {
  return std::numeric_limits<T>::max();
}

template <typename T> constexpr
T max_limits() {
  return max_limits<T>(std::is_integral<T>());
}


template <typename T1, typename T2> inline constexpr
   ERR_t MultiOverflow(const T1& a, const T2& b) {
      ERR_t err = None;
      auto max_lim = max_limits<T1>() > max_limits<T2>() ? max_limits<T1>() : max_limits<T2>(); 

      if (a > max_lim / b)
         err |= Overflow;
      else if (b > max_lim / a)
	 err |= Overflow;
      return err;
 }


template <typename T1, typename T2> inline constexpr
   ERR_t AddOverflow(const T1& a, const T2& b) {
      ERR_t err = None;
      auto max_lim = max_limits<T1>() > max_limits<T2>() ? max_limits<T1>() : max_limits<T2>();

      if (a > max_lim - b)
	 err |= Overflow;
      else if (b > max_lim - a)
	 err |= Overflow;
      return err;
 }



Unsigned128& Unsigned128::operator=(const Unsigned128& src) {
   for (int i = 0; i < el_count_; i++)
       vals_[i] = src.vals_[i];

   return *this;
}

Unsigned128& Unsigned128::operator*=(const Unsigned128& src) {
   bool val0_overflow=false;
   ERR_t res0 = MultiOverflow(vals_[0], src.vals_[0]);
   if (res0 & Overflow) 
     val0_overflow=true;
   vals_[0] ^= src.vals_[0];
   bool val1_overflow=false;
   ERR_t res1;
   res1 = MultiOverflow(vals_[1], src.vals_[1]);
   if (res1 & Overflow)
      val1_overflow=true;
   else {
      //auto sum1 = vals
   }
   //TO DO: finish

   return *this;
}

Unsigned128& Unsigned128::operator/=(const Unsigned128& src) {
    for (int i = 0; i < el_count_; i++)
       vals_[i] /= src.vals_[i];

    return *this;
}

Unsigned128& Unsigned128::operator+=(const Unsigned128& src) {
    for (int i = 0; i < el_count_; i++)
       vals_[i] += src.vals_[i];

    return *this;
}

Unsigned128& Unsigned128::operator-=(const Unsigned128& src) {
    for (int i = 0; i < el_count_; i++)
       vals_[i] -= src.vals_[i];

    return *this;
}

Unsigned128 Unsigned128::operator*(const Unsigned128& src) const {
    return Unsigned128(*this)*=src; 
}

Unsigned128 Unsigned128::operator+(const Unsigned128& src) const {
    return Unsigned128(*this)+=src;
}

Unsigned128 Unsigned128::operator-(const Unsigned128& src) const {
    return Unsigned128(*this)-=src;
}

Unsigned128 Unsigned128::operator/(const Unsigned128& src) const {
    return Unsigned128(*this)/=src;
}

template <typename T>
Unsigned128 Unsigned128::operator*(const T& src) const {
    return Unsigned128(*this)*=src;
}

template <typename T>
Unsigned128 Unsigned128::operator/(const T& src) const {
    return Unsigned128(*this)/=src;
}

template <typename T>
Unsigned128 Unsigned128::operator+(const T& src) const {
    return Unsigned128(*this)+=src;
}

template <typename T>
Unsigned128 Unsigned128::operator-(const T& src) const {
    return Unsigned128(*this)-=src;
}

Unsigned128 Unsigned128::operator%(const Unsigned128& src) const {
    return Unsigned128(0);
}

bool Unsigned128::operator>(const Unsigned128& src) const {
    return true;
}

bool Unsigned128::operator<(const Unsigned128& ) const {
    return true;
}

bool Unsigned128::operator>=(const Unsigned128& ) const {
    return true;
}

bool Unsigned128::operator<=(const Unsigned128& ) const {
    return true;
}

bool Unsigned128::operator==(const Unsigned128& other) const {
    for (int i = 0; i < el_count_; i++)
    {
       if (vals_[i] != other.vals_[i])
	 return false;
    } 
    return true;
}

bool Unsigned128::operator!=(const Unsigned128& ) const {
      
   return true;
}


class Fraction {

public:

   Fraction(const Fraction& ) = default;
   // the move semanitcs with Fraction is not really useful
   // with the current impl since all members of Fraction are primitive types
   // but this can change if use our own class for VAL_t accomodating larger than 64 bit ints.
   // In such case we will need to replace std::numeric_limits<T>::max()
   // and std::numeric_limits<T>::min() with our own impl which handles class types 
   Fraction(Fraction&& ) = default;

   constexpr Fraction(const VAL_t&, const VAL_t&, const SGN_t sgn, const ERR_t = None);

   Fraction& operator=(const Fraction& );

   Fraction& operator=(Fraction&& other) noexcept;

   Fraction& operator*=(const Fraction& );

   template <typename T>
      Fraction& operator*=(const T);

   const Fraction operator*(const Fraction& ) const;

   template <typename T> constexpr
      const Fraction operator*(const T) const;

   bool operator==(const Fraction& );
   
   bool IsPositive() { return sgn_ > SGN_t(0); };
   SGN_t Sign() { return sgn_; };
   bool IsOverflow() { return sgn_ & Overflow; };
   bool IsNaN() { return sgn_ & NaN; };
   bool IsInf() { return sgn_ & Inf; };

   friend std::ostream & operator << (std::ostream &out, const Fraction& );
   template <typename T>
      friend Fraction  operator * (const T, const Fraction&);

private:

   template <typename T> constexpr
      const Fraction Multiply(const T, std::false_type is_signed) const;
   template <typename T> constexpr
      const Fraction Multiply(const T, std::true_type is_signed) const;
   template <typename T> constexpr
      Fraction& MultiplyInto(const T, std::false_type is_signed);
   template <typename T> constexpr
      Fraction& MultiplyInto(const T, std::true_type is_signed);
   VAL_t Gcd ( const VAL_t a, const VAL_t b ) const;
   template <typename T> constexpr
      SGN_t Sgn(const T x, std::false_type is_signed) const; 
   template <typename T> constexpr
      SGN_t Sgn(const T x, std::true_type is_signed) const;
   template <typename T> constexpr
      SGN_t Sgn(const T x) const;
   VAL_t num_, denom_;
   SGN_t sgn_;
   ERR_t err_;
};

template <typename T> inline constexpr
  SGN_t Fraction::Sgn(const T x, std::false_type is_signed) const {
     return T(0) < x;
}

template <typename T> inline constexpr
  SGN_t Fraction::Sgn(const T x, std::true_type is_signed) const {
      return (T(0) < x) - (x < T(0));
}

template <typename T> inline constexpr
   SGN_t Fraction::Sgn(const T x) const {
      return Sgn(x, std::is_signed<T>());
}

constexpr Fraction::Fraction(const VAL_t& num, const VAL_t& denom, const SGN_t sgn, const ERR_t err) : 
     num_(num), denom_(denom), sgn_(sgn), err_(err)

{
   if (denom != VAL_t(0)) {
      auto gcd = Gcd(num, denom);
      if (gcd > VAL_t(1))
      {
         num_ /= gcd;
         denom_ /= gcd;
      }
   } else {
      // it is either NaN, +Inf or -Inf
      if (num != VAL_t(0)) {
          err_ |= Inf;
      } else {
          err_ |= NaN;
      }

   }
};


Fraction& Fraction::operator=(const Fraction& other) {

   num_ = other.num_;
   denom_ = other.denom_;
   return *this;
};


// the move semanitcs with Fraction is not really useful
// with the current impl since all members of Fraction are primitive types
// but this can change if use our own class for VAL_t accomodating larger than 64 bit ints
Fraction& Fraction::operator=(Fraction&& other) noexcept {

   num_ = std::move(other.num_);
   denom_ = std::move(other.denom_);
   return *this;

}

Fraction& Fraction::operator*=(const Fraction& other) {
   if (err_ != ERR_t(None) || other.err_ != ERR_t(None)) {
      err_ |= other.err_;
      if ( err_ & Overflow )
         num_ = std::numeric_limits<VAL_t>::max();
      if ( err_ & Inf ) {
         num_ = VAL_t(1);
         denom_ = VAL_t(0);
      }
      if ( err_ & NaN ) {
         num_ = VAL_t(0);
         denom_ = VAL_t(0);
      }
      sgn_ *= other.sgn_;
      return *this; 
   }

   VAL_t num_prod = VAL_t(0), denom_prod = VAL_t(0);
   // optimization 1 for the case when 
   // num_ % other.denom_ = 0 and/or other.num_ % denom_ = 0 
   // in those cases we can avoid unnecessary multiplication(s) 
   // and thus avoiding overflow errors for large fractions
   bool optimization1=false;
   if (num_ % other.denom_ == VAL_t(0))
   {
      num_prod = num_ / other.denom_;
      optimization1=true;
   }
   if (other.num_ % denom_ == VAL_t(0))
   {
      auto fact = other.num_ / denom_;
      if (num_prod != VAL_t(0)) {
          auto res = MultiOverflow(num_prod, fact);
          if (res & Overflow) {
              num_prod = std::numeric_limits<VAL_t>::max();
              denom_prod = denom_; 
              err_ |= Overflow;
          } else {              
              num_prod *= fact;
              denom_prod = VAL_t(1);
          }
      } else {
          auto res = MultiOverflow(num_, fact);
          if (res & Overflow) {
             num_prod = std::numeric_limits<VAL_t>::max();
             denom_prod = denom_;
             err_ |= Overflow;
          } else {
             num_prod = num_ * fact;
             denom_prod = other.denom_;
          }          
      }
      optimization1=true;
   }
   else
   {
      if (num_prod != VAL_t(0))
      {
          auto res = MultiOverflow(num_prod, other.num_);
          if (res & Overflow) {
             num_prod = std::numeric_limits<VAL_t>::max();
             denom_prod = denom_;
             err_ |= Overflow;
          } else {
             num_prod *= other.num_;
             denom_prod = denom_;
          }
          optimization1=true;
      }
   }

   // optimization 2 for the case when 
   // other.denom_ % num_ = 0 and/or denom_ % other.num_ = 0 
   // in those cases we can avoid unnecessary multiplication(s) 
   // and thus avoiding overflow errors for large fractions
   // NOTE: optimization 1 and optimization 2 are mutually exclusive
   if (!optimization1) {
      if (other.denom_ % num_  == VAL_t(0))
         denom_prod = other.denom_ / num_ ;
      if (denom_ % other.num_ == VAL_t(0))
      {
         auto fact = denom_ / other.num_;
         if (denom_prod != VAL_t(0)) {
             auto res = MultiOverflow(denom_prod, fact);
             if (res & Overflow) {
                 denom_prod = std::numeric_limits<VAL_t>::max();
                 num_prod = num_; 
                 err_ |= Overflow;
             } else {              
                 denom_prod *= fact;
                 num_prod = VAL_t(1);
             }
         } else {
             auto res = MultiOverflow(other.denom_, fact);
             if (res & Overflow) {
                denom_prod = std::numeric_limits<VAL_t>::max();
                num_prod = num_;
                err_ |= Overflow;
             } else {
                num_prod = num_ * fact;
                denom_prod = other.denom_;
             }          
         }
      }
      else
      {
         if (denom_prod != VAL_t(0))
         {
             auto res = MultiOverflow(denom_prod, other.denom_);
             if (res & Overflow) {
                denom_prod = std::numeric_limits<VAL_t>::max();
                num_prod = num_;
                err_ |= Overflow;
             } else {
                num_prod *= other.num_;
                denom_prod = denom_;
             }
         }
      }
   } /* !optimization1 */

   // either optimization1 or optimization2 has been performed
   // no need to proceed to the general case which uses
   // two multiplications which might overflow
   if (num_prod != VAL_t(0) || denom_prod != VAL_t(0)) {
        num_ = num_prod;
        denom_ = denom_prod;
        return *this;
   }

   if (num_prod == VAL_t(0) && denom_prod == VAL_t(0)) {
      auto n_res = MultiOverflow(num_, other.num_); 
      if (n_res & Overflow) { 
         num_ = std::numeric_limits<VAL_t>::max();
         err_ |= Overflow;
      }
      auto d_res = MultiOverflow(denom_, other.denom_);
      if (d_res & Overflow) {
         denom_ = std::numeric_limits<VAL_t>::max();
         err_ |= Overflow;
      }
      if ((n_res & Overflow) || (d_res & Overflow))
         return *this;
      num_prod = num_*other.num_;
      denom_prod = denom_*other.denom_;
   }
   if (!(err_ & Overflow)) {
      auto gcd = Gcd( num_prod, denom_prod );
      if (gcd > VAL_t(1)) {
         num_ = num_prod / gcd;
         denom_ = denom_prod / gcd;
      }
      else {
         num_ = num_prod;
         denom_ = denom_prod;
      }
   }
   else {
      num_ = num_prod;
      denom_ = denom_prod;
   }
   return *this;
}


template <typename T> constexpr
   Fraction& Fraction::MultiplyInto(const T v, std::false_type is_signed) {
   static_assert(std::is_integral<T>::value, "Integral type required.");
   if (v == T(0)) {
      num_ = VAL_t(0);
      denom_ = VAL_t(1);
      sgn_ = SGN_t(0);
      return *this;
   }
   VAL_t num_prod = num_ * v;
   auto res = MultiOverflow(num_, v);
   if (res & Overflow) {
      num_ = std::numeric_limits<VAL_t>::max(); 
      return *this;
   }
   auto gcd = Gcd( num_prod, denom_ );
   if (gcd > VAL_t(1)) {
      num_ = num_prod / gcd;
      denom_ /= gcd;
   } 

   return *this;
}

template <typename T> constexpr
   Fraction& Fraction::MultiplyInto(const T v, std::true_type is_signed) {
   static_assert(std::is_integral<T>::value, "Integral type required.");
   if (v == T(0)) {
      num_ = VAL_t(0);
      denom_ = VAL_t(1);
      sgn_ = SGN_t(0);
      return *this;
   }
   sgn_ *= Sgn(v);
   auto abs_v = std::abs(v);
   VAL_t num_prod = num_ * abs_v;;
   auto res = MultiOverflow(num_, abs_v);
   if (res & Overflow) {
      num_ = std::numeric_limits<VAL_t>::max(); 
      return *this;
   }
   auto gcd = Gcd( num_prod, denom_ );
   if (gcd > VAL_t(1)) {
      num_ = num_prod / gcd;
      denom_ /= gcd;
   } 

   return *this;
}

template <typename T>
Fraction& Fraction::operator*=(const T v) {

   static_assert(std::is_integral<T>::value, "Integral type required.");
   return MultiplyInto(v, std::is_signed<T>());
}

const Fraction Fraction::operator*(const Fraction& other) const {

   VAL_t num_prod = VAL_t(0), denom_prod = VAL_t(0);
   ERR_t err = err_;
   SGN_t sgn = sgn_ * other.sgn_;
   if (other.err_ != ERR_t(None) || err_ != ERR_t(None)) {
      err |= other.err_;
      err |= err_;
      if ( err & Overflow )
         num_prod = std::numeric_limits<VAL_t>::max();
      if ( err & Inf ) {
         num_prod = VAL_t(1);
         denom_prod = VAL_t(0);
      }
      if ( err & NaN ) {
         num_prod = VAL_t(0);
         denom_prod = VAL_t(0);
      }
      return Fraction (num_prod, denom_prod, sgn, err );
   }

   // optimization 1 for the case when 
   // num_ % other.denom_ = 0 and/or other.num_ % denom_ = 0 
   // in those cases we can avoid unnecessary multiplication(s) 
   // and thus avoiding overflow errors for large fractions
   bool optimization1=false;
   if (num_ % other.denom_ == VAL_t(0))
   {
      num_prod = num_ / other.denom_;
      optimization1=true;
   }
   if (other.num_ % denom_ == VAL_t(0))
   {
      auto fact = other.num_ / denom_;
      if (num_prod != VAL_t(0)) {
          auto res = MultiOverflow(num_prod, fact);
          if (res & Overflow) {
              num_prod = std::numeric_limits<VAL_t>::max();
              denom_prod = denom_; 
              err |= Overflow;
          } else {              
              num_prod *= fact;
              denom_prod = VAL_t(1);
          }
      } else {
          auto res = MultiOverflow(num_, fact);
          if (res & Overflow) {
             num_prod = std::numeric_limits<VAL_t>::max();
             denom_prod = denom_;
             err |= Overflow;
          } else {
             num_prod = num_ * fact;
             denom_prod = other.denom_;
          }          
      }
      optimization1=true;
   }
   else
   {
      if (num_prod != VAL_t(0))
      {
          auto res = MultiOverflow(num_prod, other.num_);
          if (res & Overflow) {
             num_prod = std::numeric_limits<VAL_t>::max();
             denom_prod = denom_;
             err |= Overflow;
          } else {
             num_prod *= other.num_;
             denom_prod = denom_;
          }
          optimization1=true;
      }
   }

   // optimization 2 for the case when 
   // other.denom_ % num_ = 0 and/or denom_ % other.num_ = 0 
   // in those cases we can avoid unnecessary multiplication(s) 
   // and thus avoiding overflow errors for large fractions
   // NOTE: optimization 1 and optimization 2 are mutually exclusive
   if (!optimization1) {
      if (other.denom_ % num_  == VAL_t(0))
         denom_prod = other.denom_ / num_ ;
      if (denom_ % other.num_ == VAL_t(0))
      {
         auto fact = denom_ / other.num_;
         if (denom_prod != VAL_t(0)) {
             auto res = MultiOverflow(denom_prod, fact);
             if (res & Overflow) {
                 denom_prod = std::numeric_limits<VAL_t>::max();
                 num_prod = num_; 
                 err |= Overflow;
             } else {              
                 denom_prod *= fact;
                 num_prod = VAL_t(1);
             }
         } else {
             auto res = MultiOverflow(other.denom_, fact);
             if (res & Overflow) {
                denom_prod = std::numeric_limits<VAL_t>::max();
                num_prod = num_;
                err |= Overflow;
             } else {
                num_prod = num_ * fact;
                denom_prod = other.denom_;
             }          
         }
      }
      else
      {
         if (denom_prod != VAL_t(0))
         {
             auto res = MultiOverflow(denom_prod, other.denom_);
             if (res & Overflow) {
                denom_prod = std::numeric_limits<VAL_t>::max();
                num_prod = num_;
                err |= Overflow;
             } else {
                num_prod *= other.num_;
                denom_prod = denom_;
             }
         }
      }
   } /* !optimization1 */

   // either optimization1 or optimization2 has been performed
   // no need to proceed to the general case which uses
   // two multiplications
   if (num_prod != VAL_t(0) || denom_prod != VAL_t(0)) {
       return Fraction (num_prod, denom_prod, sgn, err );
   }

   err = Overflow;
   auto n_res = MultiOverflow(num_, other.num_);
   auto d_res = MultiOverflow(denom_, other.denom_);
   if (n_res & Overflow) {
      num_prod = std::numeric_limits<VAL_t>::max();
      denom_prod = denom_;
   }
   if (d_res & Overflow) {
      denom_prod = std::numeric_limits<VAL_t>::max();
      if (!(n_res & Overflow))
         num_prod = num_;
   }
   if (!(n_res & Overflow) && !(d_res & Overflow)) { 
      num_prod = num_ * other.num_;
      denom_prod = denom_ * other.denom_;
      err = None;
   }
   return Fraction (num_prod, denom_prod, sgn, err );
}

template <typename T> constexpr
      const Fraction Fraction::Multiply(const T v, std::false_type is_signed) const {

   static_assert(std::is_integral<T>::value, "Integral type required.");
   VAL_t num_prod = std::numeric_limits<VAL_t>::max();
   ERR_t err = None;
  
   auto n_res = MultiOverflow(num_, v);
   if (n_res & Overflow) { 
       err = Overflow;
   }
   else 
       num_prod = num_ * v;
   return Fraction ( num_prod, denom_, sgn_, err );

}

template <typename T> constexpr
      const Fraction Fraction::Multiply(const T v, std::true_type is_signed) const {

   static_assert(std::is_integral<T>::value, "Integral type required.");
   VAL_t num_prod = std::numeric_limits<VAL_t>::max();;
   ERR_t err = None;
  
   auto abs_v = std::abs(v);
   auto n_res = MultiOverflow(num_, abs_v);
   if (n_res & Overflow) { 
       err = Overflow;
   }
   else 
       num_prod = num_ * abs_v;
   return Fraction ( num_prod, denom_, sgn_ * Sgn(v), err );

}

template <typename T> constexpr
   const Fraction Fraction::operator*(const T v) const 
{
   static_assert(std::is_integral<T>::value, "Integral type required.");
   return Multiply(v, std::is_signed<T>());
}

bool Fraction::operator==(const Fraction& other) {
   if (err_ == None && other.err_ == None) {
     if (num_ == other.num_ && denom_ == other.denom_ && sgn_ == other.sgn_)
         return true;
     else
         return false;
   }
   else
      return false;
}

// euclid algorithm for finding greatest common divisor
VAL_t Fraction::Gcd(const VAL_t a , const VAL_t b ) const {
   if (b == VAL_t(0)) 
       return a; 
   if (a == VAL_t(0))
       return b;
   if (a == b)
       return a;
   if (a > b)
      return Gcd(b, a % b);
   else
      return Gcd(a, b % a); 
}

template <typename T>
   Fraction  operator*(const T v, const Fraction& f) {

    static_assert(std::is_integral<T>::value, "Integral type required.");
    return f*v;
}


std::ostream & operator << (std::ostream &out, const Unsigned128& f) {
   //TO DO: finish this method
   return out;
}

std::ostream & operator << (std::ostream &out, const Fraction& f) {
   if (f.err_ == None) {
      if (f.sgn_ > SGN_t(0))
        out << f.num_ << "/" << f.denom_ << std::endl;
      else
        out << "-" << f.num_ << "/" << f.denom_ << std::endl;
   }
   else {
      if (f.err_ & Overflow)
        out << "Overflow! ";
      if (f.err_ & NaN)
        out << "NaN! ";
      if (f.err_ & Inf) {
         if (f.sgn_ >= SGN_t(0))
            out << "+Inf! ";
         else 
            out << "-Inf! ";
      }
      out << std::endl;
   }
   return out;
}

int main(int argc, char * argv[]) {
   Fraction f(100, 3, -1);
   auto f2 = f * 3L;
   std::cout << "f2 = " << f2;
   auto f3 = 5 * f2;
   std::cout << "f3 = " << f3;
   Fraction f4(500, 1, -1);
   if (f4 == f3) {
      std::cout << "f3 is equal to f4" << std::endl;
   }
   else
   {
      std::cout << "f4 is different from f3" << std::endl;
   }
   auto fact = 100000000ULL;
   auto f5 = fact * f4; 
   std::cout << "f5 = " << f5;
   unsigned short fact2 = 2;
   auto f6 =  f5 * fact2;
   std::cout << "f6 = " << f6;
   Fraction f7(std::move(f6));
   std::cout << "f7 = " << f7;
   auto f8 = f6 * f7;
   std::cout << "f8=" << f8;
   f8 *= -2L;
   std::cout << "f8 before multplication with 4ULL: " << f8;
   f8 *= 4ULL;
   std::cout << "f8 after mutlplication with 4ULL: " << f8;
   Fraction f9(20, 3, 1);
   Fraction f10(9, 10, 1);
   auto f11 = f9 * f10;
   std::cout << "f11=" << f11;
   Fraction f12(3, 20, 1);
   Fraction f13(10, 9, 1);
   auto f14 = f12*f13;
   std::cout << "f14=" << f14;
   return 0;

}
