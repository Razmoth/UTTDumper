#include <rtti.h>
#include <utils.h>
#include <vector>
#include <format>
#include <thread>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <algorithm>

namespace UTTD::Unity {
	class RTTI_Unity2017_3 : public IRTTI  {
		struct Info {
			void* base;
			void* factory;
			char* className;
			char* classNamespace;
			char* module;
			uint32_t typeID;
			int32_t size;
			uint32_t typeIndex;
			uint32_t descendantCount;
			bool isAbstract;
			bool isSealed;
			bool isEditorOnly;
			bool isStripped;
			void* attributes;
			uint64_t attributeCount;
		};

	public:
		RTTI_Unity2017_3(void* ptr) {
			m_info = static_cast<Info*>(ptr);
		}

		std::shared_ptr<IRTTI> base() const { return m_info->base != nullptr ? std::make_shared<RTTI_Unity2017_3>(m_info->base) : nullptr; };
		std::string_view name() const { return m_info->className != nullptr ? std::string_view{ m_info->className } : std::string_view{}; };
		std::string_view nameSpace() const { return m_info->classNamespace != nullptr ? std::string_view{ m_info->classNamespace } : std::string_view{}; };
		std::string_view module() const { return m_info->module != nullptr ? std::string_view{ m_info->module } : std::string_view{}; }
		uint32_t typeID() const { return m_info->typeID; }
		int32_t size() const { return m_info->size; }
		uint32_t typeIndex() const { return m_info->typeIndex; }
		uint32_t descendantCount() const { return m_info->descendantCount; }
		bool isAbstract() const { return m_info->isAbstract; }
		bool isSealed() const { return m_info->isSealed; }
		bool isEditorOnly() const { return m_info->isEditorOnly; }
		bool isStripped() const { return m_info->isStripped; }
		void* attributes() const { return m_info->attributes; }
		uint64_t attributeCount() const { return m_info->attributeCount; }
		std::vector<std::shared_ptr<IRTTI>>& derived() { return m_derived; }
		void* ptr() { return m_info; }

	private:
		Info* m_info;
		std::vector<std::shared_ptr<IRTTI>> m_derived;
	};

	class RTTI_Unity5_5 : public IRTTI {
		struct Info {
			void* base;
			void* factory;
			char* className;
			char* classNamespace;
			uint32_t typeID;
			int32_t size;
			uint32_t typeIndex;
			uint32_t descendantCount;
			bool isAbstract;
			bool isSealed;
			bool isEditorOnly;
		};

	public:
		RTTI_Unity5_5(void* ptr) {
			m_info = static_cast<Info*>(ptr);
		}

		std::shared_ptr<IRTTI> base() const { return m_info->base != nullptr ? std::make_shared<RTTI_Unity5_5>(m_info->base) : nullptr; };
		std::string_view name() const { return m_info->className != nullptr ? std::string_view{ m_info->className } : std::string_view{}; };
		std::string_view nameSpace() const { return m_info->classNamespace != nullptr ? std::string_view{ m_info->classNamespace } : std::string_view{}; };
		std::string_view module() const { return std::string_view{}; }
		uint32_t typeID() const { return m_info->typeID; }
		int32_t size() const { return m_info->size; }
		uint32_t typeIndex() const { return m_info->typeIndex; }
		uint32_t descendantCount() const { return m_info->descendantCount; }
		bool isAbstract() const { return m_info->isAbstract; }
		bool isSealed() const { return m_info->isSealed; }
		bool isEditorOnly() const { return m_info->isEditorOnly; }
		bool isStripped() const { return false; }
		void* attributes() const { return nullptr; }
		uint64_t attributeCount() const { return 0; }
		std::vector<std::shared_ptr<IRTTI>>& derived() { return m_derived; }
		void* ptr() { return m_info; }

	private:
		Info* m_info;
		std::vector<std::shared_ptr<IRTTI>> m_derived;
	};

	class RTTI_Unity5_4 : public IRTTI {
		struct Info {
			void* base;
			void* factory;
			char* className;
			uint32_t typeID;
			int32_t size;
			uint32_t typeIndex;
			uint32_t descendantCount;
			bool isAbstract;
			bool isSealed;
			bool isEditorOnly;
		};

	public:
		RTTI_Unity5_4(void* ptr) {
			m_info = static_cast<Info*>(ptr);
		}

		std::shared_ptr<IRTTI> base() const { return m_info->base != nullptr ? std::make_shared<RTTI_Unity5_4>(m_info->base) : nullptr; };
		std::string_view name() const { return m_info->className != nullptr ? std::string_view{ m_info->className } : std::string_view{}; };
		std::string_view nameSpace() const { return std::string_view{}; };
		std::string_view module() const { return std::string_view{}; }
		uint32_t typeID() const { return m_info->typeID; }
		int32_t size() const { return m_info->size; }
		uint32_t typeIndex() const { return m_info->typeIndex; }
		uint32_t descendantCount() const { return m_info->descendantCount; }
		bool isAbstract() const { return m_info->isAbstract; }
		bool isSealed() const { return m_info->isSealed; }
		bool isEditorOnly() const { return m_info->isEditorOnly; }
		bool isStripped() const { return false; }
		void* attributes() const { return nullptr; }
		uint64_t attributeCount() const { return 0; }
		std::vector<std::shared_ptr<IRTTI>>& derived() { return m_derived; }
		void* ptr() { return m_info; }

	private:
		Info* m_info;
		std::vector<std::shared_ptr<IRTTI>> m_derived;
	};

	class RTTI_Unity5_2 : public IRTTI {
		struct Info {
			void* base;
			void* factory;
			uint32_t typeID;
			char* className;
			int32_t size;
			bool isAbstract;
		};

	public:
		RTTI_Unity5_2(void* ptr) {
			m_info = static_cast<Info*>(ptr);
		}

		std::shared_ptr<IRTTI> base() const { return m_info->base != nullptr ? std::make_shared<RTTI_Unity5_2>(m_info->base) : nullptr; };
		std::string_view name() const { return m_info->className != nullptr ? std::string_view{ m_info->className } : std::string_view{}; };
		std::string_view nameSpace() const { return std::string_view{}; };
		std::string_view module() const { return std::string_view{}; }
		uint32_t typeID() const { return m_info->typeID; }
		int32_t size() const { return m_info->size; }
		uint32_t typeIndex() const { return 0; }
		uint32_t descendantCount() const { return 0; }
		bool isAbstract() const { return m_info->isAbstract; }
		bool isSealed() const { return false; }
		bool isEditorOnly() const { return false; }
		bool isStripped() const { return false; }
		void* attributes() const { return nullptr; }
		uint64_t attributeCount() const { return 0; }
		std::vector<std::shared_ptr<IRTTI>>& derived() { return m_derived; }
		void* ptr() { return m_info; }

	private:
		Info* m_info;
		std::vector<std::shared_ptr<IRTTI>> m_derived;
	};

	bool RTTI::initialize() {
		if (m_version >= UTTD::Unity::s_version<5, 5>) {
			Info* info = static_cast<Info*>(m_ptr);

			m_types = std::vector<std::shared_ptr<IRTTI>>(info->count);
			for (int32_t i = 0; i < info->count; i++) {
				void* ptr = (&info->first)[i];
				m_types[i] = s_RTTI(ptr,  m_version);
			}
		}
		else {
			ClassIDToRTTI* classIDToRTTI = (ClassIDToRTTI*)m_ptr;

			m_types = std::vector<std::shared_ptr<IRTTI>>(MaxRuntimeTypeId);
			for (int32_t i = 0; i < MaxRuntimeTypeId; i++) {
				void* ptr = classIDToRTTI(i);
				m_types[i] = s_RTTI(ptr, m_version);
			}
		}

		for (const std::shared_ptr<IRTTI>& type : m_types) {
			std::shared_ptr<IRTTI> base = type->base();
			if (base.get() != nullptr) {
				std::vector<std::shared_ptr<IRTTI>>::iterator iter = std::find_if(m_types.begin(), m_types.end(), [&base](std::shared_ptr<IRTTI> const& other) { return other->fullName() == base->fullName(); });
				if (iter != m_types.end()) {
					std::vector<std::shared_ptr<IRTTI>>& derived = iter->get()->derived();
					derived.emplace_back(type);
				}
			}
		}

		return m_types.size() != 0;
	}

	static std::shared_ptr<IRTTI> s_RTTI(void* ptr, const Version& version) {
		if (version >= s_version<2017, 3>) {
			return std::make_shared<RTTI_Unity2017_3>(ptr);
		}
		else if (version >= s_version<5, 5>) {
			return std::make_shared<RTTI_Unity5_5>(ptr);
		}
		else if (version >= s_version<5, 4>) {
			return std::make_shared<RTTI_Unity5_4>(ptr);
		}
		else if (version >= s_version<5, 2>) {
			return std::make_shared<RTTI_Unity5_2>(ptr);
		}
		else {
			throw std::invalid_argument("unknown version !!");
		}
	}
}