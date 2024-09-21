#pragma once

#include <map>
#include <utility>
#include <string_view>

namespace UTTD::Unity {
	class CommonString {
	public:
		CommonString(char* ptr);

		std::string_view string(size_t offset) const;

		std::map<size_t, std::string_view> strings() const { return m_strings; }
		std::pair<char*, char*> range() const { return std::make_pair(m_begin, m_end); }
	private:
		char* m_begin;
		char* m_end;
		std::map<size_t, std::string_view> m_strings;
	};
}