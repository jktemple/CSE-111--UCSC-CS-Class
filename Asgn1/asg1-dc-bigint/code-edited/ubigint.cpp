// $Id: ubigint.cpp,v 1.6 2022-03-29 13:04:12-07 - - $

#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <string>
using namespace std;

#include "debug.h"
#include "ubigint.h"

ostream& operator<< (ostream& out, const vector<uint8_t>& vec) {
   if(vec.size() ==0){
       out << 0;
       return out;
   }
   int i = 0;
   for (auto itor = vec.rbegin(); itor != vec.rend(); ++itor) {
      out << int (*itor);
        if(i == 68){
            i = 0;
            out << "\\"<< "\n";
        } else {
            i++;
        }
   }
   return out;
}

ostream& operator<< (ostream& out, const ubigint& that) { 
   return out << "ubigint(" << that.uvalue << ")";
}

ubigint::ubigint (unsigned long that): uvalue (that) {
   DEBUGF ('~', this << " -> " << uvalue) 
   while(uvalue.size()>0){
       uvalue.pop_back();
   }
   while(that > 0){
                uvalue.push_back(that%10);
                that = that/10;
   }
}

ubigint::ubigint (const string& that): uvalue(0) {
   DEBUGF ('~', "that = \"" << that << "\"");
   for (char digit: that) {
      if (not isdigit (digit)) {
         throw invalid_argument ("ubigint::ubigint(" + that + ")");
      }
////FIXME////      uvalue = uvalue * 10 + digit - '0';
   }
    for(int i = that.length() -1; i >= 0 ; i--){
        uvalue.push_back(that[i]-'0');
    }
    while(uvalue.size() > 0 && uvalue.back() == 0){
                uvalue.pop_back();
        }

}

ubigint ubigint::operator+ (const ubigint& that) const {
    DEBUGF ('u', *this << "+" << that);
    ubigint uReturn = ubigint();
    auto thisItor = this->uvalue.begin();
    auto thatItor = that.uvalue.begin();
    uint8_t carry = 0;
    while(thisItor != uvalue.end() && thatItor != that.uvalue.end()){
        uint8_t temp = carry + *thisItor + *thatItor;
        carry = temp / 10;
        uReturn.uvalue.push_back(temp%10);
        thisItor++;
        thatItor++;
    }
    while(thisItor != this->uvalue.end()){
        uint8_t temp = carry + *thisItor ;
        carry = temp / 10;
        uReturn.uvalue.push_back(temp%10);
        thisItor++;
    }
    while(thatItor != that.uvalue.end()){
        uint8_t temp = carry + *thatItor;
        carry = temp / 10;
        uReturn.uvalue.push_back(temp%10);
        thatItor++;
    }
    if(carry > 0){
        uReturn.uvalue.push_back(carry);
    }
    while(uReturn.uvalue.size() > 0 && uReturn.uvalue.back() == 0){
        uReturn.uvalue.pop_back();
    }
   return uReturn;
}

ubigint ubigint::operator- (const ubigint& that) const {
   if (*this < that) throw domain_error ("ubigint::operator-(a<b)");
   ubigint uReturn = ubigint();
   //This must be > that
    auto thisItor = this->uvalue.begin();
    auto thatItor = that.uvalue.begin();
    uint8_t carry = 0;
    while(thisItor != uvalue.end() && thatItor != that.uvalue.end()){
        int temp = *thisItor - *thatItor - carry;
        if(temp < 0 ){
            uReturn.uvalue.push_back(temp+10);
            carry = 1;
        } else {
            uReturn.uvalue.push_back(temp);
            carry = 0;
        }
        thisItor++;
        thatItor++;
    }
    while(thisItor != this->uvalue.end()){
        int temp = *thisItor - carry;
        if(temp < 0 ){
            uReturn.uvalue.push_back(temp+10);
            carry = 1;
        } else {
            uReturn.uvalue.push_back(temp);
            carry = 0;
        }
        thisItor++;
    }
    while(uReturn.uvalue.size() > 0 && uReturn.uvalue.back() == 0){
        uReturn.uvalue.pop_back();
    }
    return uReturn;
////FIXME////   return ubigint (uvalue - that.uvalue);
}

ubigint ubigint::operator* (const ubigint& that) const {
////FIXME////   return ubigint (uvalue * that.uvalue);
    //cout<< uvalue << " times " << that.uvalue << "\n result: \n";
    ubigint uReturn = ubigint();
    for(uint16_t i = 0; i < uvalue.size()+that.uvalue.size(); i++){
        uReturn.uvalue.push_back(0);
    }
    //cout << "Filled with zeros\n" << uReturn.uvalue << "\n";
    //int x = 0;
    for(uint16_t i = 0; i < uvalue.size(); i++){
        uint16_t carry = 0;
        for(uint16_t j = 0; j < that.uvalue.size(); j++){
            //p[i+j] + u[i] * v[j] + carry
            uint16_t digit = uReturn.uvalue[i+j] + 
               (uvalue[i]*that.uvalue[j]) + carry;
            uReturn.uvalue[i+j] = digit % 10;
            carry = digit/10;
            //x++;
        }
        uReturn.uvalue[i + that.uvalue.size()] = carry;
    }
    //cout << "value of x\n" << x << "done\n";
    while(uReturn.uvalue.size() > 0 && uReturn.uvalue.back() == 0){
        uReturn.uvalue.pop_back();
    }
    return uReturn;
}

void ubigint::multiply_by_2() {
////FIXME////   uvalue *= 2;
    //auto thisItor = uvalue.begin();
    //cout << "multiplying " << uvalue << " by 2\n";
    uint8_t carry = 0;
    for(uint16_t i = 0; i < uvalue.size(); i++){
        uint8_t temp = uvalue[i]*2 + carry;
        carry = temp/10;
        uvalue[i]= temp%10;
    }
    if(carry > 0){
        uvalue.push_back(carry);
    }
    while(uvalue.size() > 0 && uvalue.back() == 0){
        uvalue.pop_back();
    }
    //cout <<"result: " << uvalue << "\n";
}

void ubigint::divide_by_2() {
////FIXME////   uvalue /= 2;
    //auto thisItor = uvalue.begin();
    //cout << "dividing " << uvalue << "  by 2\n"; 
    for(uint16_t i = 0; i < uvalue.size(); i++){
        uvalue[i] = uvalue[i]/2;
        if(i != (uvalue.size() - 1) && uvalue[i+1]%2 == 1){
              uvalue[i] += 5; 
        }
    }
    while(uvalue.size() > 0 && uvalue.back() == 0){
        uvalue.pop_back();
    }
    //cout<< "result: " << uvalue << "\n";
}


struct quo_rem { ubigint quotient; ubigint remainder; };
quo_rem udivide (const ubigint& dividend, const ubigint& divisor_) {
   // NOTE: udivide is a non-member function.
   ubigint divisor {divisor_};
   ubigint zero {0};
   if (divisor == zero) throw domain_error ("udivide by zero");
   ubigint power_of_2 {1};
   ubigint quotient {0};
   ubigint remainder {dividend}; // left operand, dividend
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (power_of_2 > zero) {
      if (divisor <= remainder) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   DEBUGF ('/', "quotient = " << quotient);
   DEBUGF ('/', "remainder = " << remainder);
   //cout << "quotient = " << quotient;
   //cout << "remainder = " << remainder;
   return {.quotient = quotient, .remainder = remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
    //cout << uvalue << "divided by " << that.uvalue << "\n";
    return udivide (*this, that).quotient;
}

ubigint ubigint::operator% (const ubigint& that) const {
   return udivide (*this, that).remainder;
}

bool ubigint::operator== (const ubigint& that) const {
   return uvalue == that.uvalue;
}

bool ubigint::operator< (const ubigint& that) const {
   if(*this==that){
      return false; 
   }
   //cout << uvalue << " < " << that.uvalue << "\n";
   if(uvalue.size() != that.uvalue.size()){
      if(uvalue.size() > that.uvalue.size()){
        //cout << "false 1\n";
         return false;
      } else {
         //cout << "true 1\n"; 
         return true;
      }
   } else {
    for(int i = uvalue.size() -1; i >= 0 ; i--){
        if(uvalue[i] != that.uvalue[i]){
            //cout <<  (uvalue[i] < that.uvalue[i]) << " 2\n";
            return uvalue[i] < that.uvalue[i];
        }
    }
   }
   //cout << "false end\n";
   return false;
}

void ubigint::print() const {
   DEBUGF ('p', this << " -> " << *this);
   cout << uvalue;
}

