#include <common_string.h>
#include <iostream>
#include <cstring>
#include <format>

namespace UTTD::Unity {

	CommonString::CommonString(char* ptr) : m_begin(ptr), m_end(ptr), m_strings() {
		size_t length, offset = 0, count = 0;
		while ((length = strlen(m_begin + offset))) {
			m_strings[offset] = std::string_view{ m_begin + offset };
			offset += length + 1;
			count++;
		}

		m_end = m_begin + offset;

		std::cout << std::format("Found {0} common strings.", count) << std::endl;
	}

	std::string_view CommonString::string(size_t offset) const {
		return m_strings.contains(offset) ? m_strings.at(offset) : std::string_view{};
	}
}