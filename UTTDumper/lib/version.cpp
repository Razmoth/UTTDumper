#include <version.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iosfwd>

namespace UTTD::Unity {
	static constexpr char TypeToChar(VersionType type) {
		switch (type)
		{
		case UTTD::Unity::VersionType::Alpha:
			return 'a';
		case UTTD::Unity::VersionType::Beta:
			return 'b';
		case UTTD::Unity::VersionType::Final:
			return 'f';
		case UTTD::Unity::VersionType::Patch:
			return 'p';
		case UTTD::Unity::VersionType::Experimental:
			return 'x';
		default:
			throw std::invalid_argument("unknown type");
		}
	}

	static constexpr VersionType CharToType(char c) {
		switch (c)
		{
		case 'a':
			return UTTD::Unity::VersionType::Alpha;
		case 'b':
			return UTTD::Unity::VersionType::Beta;
		case 'f':
			return UTTD::Unity::VersionType::Final;
		case 'p':
			return UTTD::Unity::VersionType::Patch;
		case 'x':
			return UTTD::Unity::VersionType::Experimental;
		default:
			throw std::invalid_argument("unknown version char");
		}
	}

	Version::Version(const char* string) {
		char dot = 0, type = 0;
		std::stringstream ss(string);
		ss >> major >> dot >> minor >> dot >> patch >> type >> build;
		this->type = CharToType(type);
	}

	std::ostream& operator<<(std::ostream& os, const Version& version) {
		return os << version.major << '.' << version.minor << '.' << version.patch << TypeToChar(version.type) << version.build;
	}

	std::string Version::str() const {
		std::ostringstream ss;
		ss << *this;
		return std::move(ss).str();
	}
}