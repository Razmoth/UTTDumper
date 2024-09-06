#pragma once

#include <string>

namespace UTTD::Unity {
	enum VersionType { Alpha, Beta, Final, Patch, Experimental };

	static constexpr char TypeToChar(VersionType type);
	static constexpr VersionType CharToType(char c);

	struct Version {
		int major;
		int minor;
		int patch;
		VersionType type;
		int build;

		Version(const char* string);
		constexpr Version(int32_t major, int32_t minor, int32_t patch, VersionType type, int32_t build) :
			major(major),
			minor(minor),
			patch(patch),
			type(type),
			build(build) {

		}

		bool operator==(const Version& other) const = default;
		std::strong_ordering operator<=>(const Version& other) const {
			if (major != other.major)
				return major <=> other.major;

			if (minor != other.minor)
				return minor <=> other.minor;

			if (patch != other.patch)
				return patch <=> other.patch;

			if (type != other.type)
				return type <=> other.type;

			return major <=> other.major;
		}
		
		friend std::ostream& operator<<(std::ostream& os, const Version& version);

		std::string str() const;
	};

	template <int32_t major = 0, int32_t minor = 0, int32_t patch = 0, VersionType type = Final, int32_t build = 0>
	static constexpr Version s_version = Version{ major, minor, patch, type, build };
}