#include "test_runner.h"

#include <ostream>
using namespace std;

#define PRINT_VALUES(out, x, y) out << (x) << endl << (y) << endl

int main() {
  TestRunner tr;
  tr.RunTest([] {
    ostringstream output;
    PRINT_VALUES(output, 5, "red belt");
    ASSERT_EQUAL(output.str(), "5\nred belt\n");

    output = ostringstream();
    if(false)
        PRINT_VALUES(output, 5, "red belt");
    ASSERT_EQUAL(output.str(), "");

    output = ostringstream();
    if(false)
        PRINT_VALUES(output, 5, "red belt");
    else
        PRINT_VALUES(output, 4, "blue belt");
    ASSERT_EQUAL(output.str(), "4\nblue belt\n");
  }, "PRINT_VALUES usage example");
  return 0;
}