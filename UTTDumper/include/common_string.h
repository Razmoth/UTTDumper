#pragma once

#include <vector>

namespace UTTD::Unity {
	struct CommonString {
		const char* begin;
		const char* end;

		CommonString(char* begin, char* end) {
			this->begin = *reinterpret_cast<char**>(begin);
			this->end = *reinterpret_cast<char**>(end);
		}
	};
}