#pragma once

#include <vector>

namespace UTTD::Unity {
	struct CommonString {
		const char* begin;
		const char* end;

		CommonString(char* begin, char* end, bool direct) {
			this->begin = direct ? begin : *reinterpret_cast<char**>(begin);
			this->end = direct ? end : *reinterpret_cast<char**>(end);
		}
	};
}