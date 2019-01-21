#include "include/preprocessor.h"

using simple::preproc;

int main() {
	preproc p("sample.txt");
	p.proc();
	system("pause");
}