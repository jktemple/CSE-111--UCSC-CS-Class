// $Id: quorem-trace.cpp,v 1.27 2022-01-17 21:11:06-08 - - $

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <libgen.h>
#include <vector>
using namespace std;

using ulong = unsigned long;
using uupair = pair<ulong,ulong>;
constexpr size_t fieldwidth = 15;

void trace_title (bool showquorem = true) {
   cout << setw (fieldwidth) << "divisor";
   cout << setw (fieldwidth) << "powerof2";
   if (showquorem) {
      cout << setw (fieldwidth) << "quotient";
      cout << setw (fieldwidth) << "remainder";
   }
   cout << endl;
}

void trace (ulong divisor, ulong powerof2,
            ulong quotient, ulong remainder, bool showquorem = true) {
   cout << setw (fieldwidth) << divisor;
   cout << setw (fieldwidth) << powerof2;
   if (showquorem) {
      cout << setw (fieldwidth) << quotient;
      cout << setw (fieldwidth) << remainder;
   }
   cout << endl;
}


uupair divide (ulong dividend, ulong divisor) {
   if (divisor == 0) throw domain_error ("divide(_,0)");
   ulong powerof2 = 1;
   trace_title (false);
   trace (divisor, powerof2, 0, 0, false);
   while (divisor < dividend) {
      divisor *= 2;
      powerof2 *= 2;
      trace (divisor, powerof2, 0, 0, false);
   }
   ulong quotient = 0;
   ulong remainder = dividend;
   trace_title();
   trace (divisor, powerof2, quotient, remainder);
   while (powerof2 > 0) {
      if (divisor <= remainder) {
         remainder -= divisor;
         quotient += powerof2;
      }
      divisor /= 2;
      powerof2 /= 2;
      trace (divisor, powerof2, quotient, remainder);
   }
   return uupair (quotient, remainder);
}


ostream& operator<< (ostream& out, const uupair& pair) {
   out << pair.first << " (rem " << pair.second << ")";
   return out;
}

int main (int argc, char** argv) {
   if (argc < 3) {
      cout << "Usage: " << basename (argv[0])
           << " dividend divisor ..." << endl;
   }else {
      for (int argi = 1; argi < argc - 1; argi += 2) {
         ulong dividend = atol (argv[argi]);
         ulong divisor = atol (argv[argi + 1]);
         cout << endl << "Evaluating: "
              << dividend << " / " << divisor << ":" << endl;
         try {
            uupair result = divide (dividend, divisor);
            cout << "Result: "
                 << dividend << " / " << divisor
                 << " = " << result << endl;
            uupair tested = uupair (dividend / divisor,
                                    dividend % divisor);
            if (tested != result) {
              cout << "........ wrong " << tested << endl;
            }
         }catch (domain_error& error) {
            cout << "........ domain_error: " << error.what() << endl;
         }
      }
   }
}

//TEST// ./quorem-trace  100 11 329 23 963 71 >quorem-trace.out1 2>&1
//TEST// ./quorem-trace  123 0 76543 123 >quorem-trace.out2 2>&1
//TEST// mkpspdf quorem-trace.ps quorem-trace.cpp quorem-trace.out?

