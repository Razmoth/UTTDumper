#include <native_object.h>
#include <rtti.h>

namespace UTTD::Unity {
	class NativeObject_Unity5_0 : public INativeObject {
		struct Info {
			void* virtualFunctionTabel;
			int32_t instanceID;
			std::bitset<32> bits;
		};

	public:
		NativeObject_Unity5_0(void* ptr) {
			if (!ptr) throw std::invalid_argument("empty object ptr !!");
			m_info = static_cast<Info*>(ptr);
		}

		int32_t instanceID() const { return m_info->instanceID; }
		MemLabelId memLabel() const { return MemLabelId{ (int32_t)range<0, 11>(m_info->bits).to_ulong() }; }
		uint8_t temporary() const { return (unsigned char)range<11, 12>(m_info->bits).to_ulong(); }
		Hide hide() const { return static_cast<Hide>(range<12, 18>(m_info->bits).to_ulong()); }
		bool isPersistent() const { return range<18, 19>(m_info->bits).test(0); }
		uint32_t cachedTypeIndex() const { return range<19, 29>(m_info->bits).to_ulong(); }
		void* ptr() { return static_cast<void*>(m_info); }
	private:
		Info* m_info;
	};

	std::shared_ptr<INativeObject> NativeObject::produce(IRTTI& rtti, int32_t instanceID, CreationMode creationMode) const {
		if (rtti.isAbstract())
			return nullptr;

		void* ptr = nullptr;
		if (m_version < s_version<3, 5>) {
			throw std::runtime_error("version not supported !!"); //TODO: add support ?
		}
		else if (m_version < s_version<5, 5>) {
			Produce3_5* produce = (Produce3_5*)m_ptr;
			ptr = produce(rtti.typeID(), instanceID, MemLabel, creationMode);
		}
		else if (m_version < s_version<2017, 2>) {
			Produce5_5* produce = (Produce5_5*)m_ptr;
			ptr = produce(rtti.ptr(), instanceID, MemLabel, creationMode);
		}
		else if (m_version < s_version<2023, 1, 0, 'a', 2>) {
			Produce2017_2* produce = (Produce2017_2*)m_ptr;
			ptr = produce(rtti.ptr(), rtti.ptr(), instanceID, MemLabel, creationMode);
		}

		return ptr == nullptr ? nullptr : s_nativeObject(ptr, m_version);
	}

	static std::shared_ptr<INativeObject> s_nativeObject(void* ptr, const Version& version) {
		if (version < s_version<5, 0>) {
			throw std::runtime_error("version not supported !!"); //TODO: add support?
		}
		else {
			return std::make_shared<NativeObject_Unity5_0>(ptr);
		}
	}
}
