#include "sample-lib/library.h"

int main(int /*argc*/, char** /*argv*/) {
	if (sample::fibbonaci<7>() != 13) {
		return 1;
	} else {
		return 0;	
	}
}