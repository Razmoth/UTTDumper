#include <version.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iosfwd>

namespace UTTD::Unity {
	Version::Version(const char* string) {
		char dot = 0;
		std::stringstream ss(string);
		ss >> major >> dot >> minor >> dot >> patch >> type >> build;
	}

	std::ostream& operator<<(std::ostream& os, const Version& version) {
		return os << version.major << '.' << version.minor << '.' << version.patch << version.type << version.build;
	}

	std::string Version::str() const {
		std::ostringstream ss;
		ss << *this;
		return std::move(ss).str();
	}
}