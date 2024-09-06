#pragma once

#include <version.h>
#include <vector>
#include <memory>
#include <format>
#include <string_view>

namespace UTTD::Unity {
	class IRTTI {
	public:
		virtual std::shared_ptr<IRTTI> base() const = 0;
		virtual std::string_view name() const = 0;
		virtual std::string_view nameSpace() const = 0;
		virtual std::string_view module() const = 0;
		virtual uint32_t typeID() const = 0;
		virtual int32_t size() const = 0;
		virtual uint32_t typeIndex() const = 0;
		virtual uint32_t descendantCount() const = 0;
		virtual bool isAbstract() const = 0;
		virtual bool isSealed() const = 0;
		virtual bool isEditorOnly() const = 0;
		virtual bool isStripped() const = 0;
		virtual void* attributes() const = 0;
		virtual uint64_t attributeCount() const = 0;
		virtual std::vector<std::shared_ptr<IRTTI>>& derived() = 0;
		virtual void* ptr() = 0;

		std::string fullName() const {
			if (nameSpace().empty() || name().empty()) {
				return std::string(name());
			}
			else {
				return std::format("{0}.{1}", nameSpace(), name());
			}
		}
	};

	class RTTI {
	protected:
		static const int16_t MaxRuntimeTypeId = 2000;

		typedef void* ClassIDToRTTI(int32_t classID);

		struct Info { 
			int32_t count; 
			void* first; 
		};
	public:
		RTTI(void* ptr, const Version& version) :
			m_ptr(ptr),
			m_version(version) {

		}

		bool initialize();
		const std::vector<std::shared_ptr<IRTTI>>& types() const { return m_types; }
	private:
		void* m_ptr;
		const Version& m_version;
		std::vector<std::shared_ptr<IRTTI>> m_types;
	};

	static std::shared_ptr<IRTTI> s_RTTI(void* ptr, const Version& version);
}