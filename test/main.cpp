#define FB_DEBUG
#include "fb/debug.hpp"
#include <iostream>

void foo() {
	auto l = FB_DEBUG_SCOPED_LOGGER();
	l.print("hi", " ", "there");
}

void bar() {
	auto l = FB_DEBUG_SCOPED_LOGGER();
	foo();
}

void baz() {
	
}

int main(int argc, char const* argv[]) {
	auto l = FB_DEBUG_SCOPED_LOGGER();
	bar();

	return 0;
}
