#pragma once

#include <version.h>
#include <rtti.h>
#include <memory>
#include <bitset>

namespace UTTD::Unity {
	enum class CreationMode {
		Default = 0,
		FromNonMainThread = 1,
		DefaultNoLock = 2,
	};

	enum class Hide {
		None,
		HideInHierarchy			= 1 << 0,
		HideInInspector			= 1 << 1,
		DontSaveInEditor		= 1 << 2,
		NotEditable				= 1 << 3,
		DontSaveInBuild			= 1 << 4,
		DontUnloadUnusedAsset	= 1 << 5,
		DontSave				= 52,
		HideAndDontSave			= 61
	};

	class INativeObject {
	protected:
		template<size_t R, size_t L, size_t N>
		constexpr std::bitset<N> range(std::bitset<N> b) const
		{
			static_assert(R <= L && L <= N, "invalid bitrange");
			b >>= R;
			b <<= (N - L + R);
			b >>= (N - L);
			return b;
		}
	public:
		virtual int32_t instanceID() const = 0;
		virtual uint32_t memLabel() const = 0;
		virtual uint8_t temporary() const = 0;
		virtual Hide hide() const = 0;
		virtual bool isPersistent() const = 0;
		virtual uint32_t cachedTypeIndex() const = 0;
		virtual void* ptr() = 0;
	};

	class NativeObject {
	protected:
		const int32_t MemLabel = 0x38;

		typedef void* Produce3_5(uint32_t typeID, int32_t instanceID, int32_t memLabel, CreationMode creationMode);
		typedef void* Produce5_5(void* typePtr, int32_t instanceID, int32_t memLabel, CreationMode creationMode);
		typedef void* Produce2017_2(void* typeInPtr, const void* typeOutPtr, int32_t instanceID, int32_t memLabel, CreationMode creationMode);
	public:
		NativeObject(void* ptr, const Version& version) :
			m_ptr(ptr),
			m_version(version) {

		}
		
		std::shared_ptr<INativeObject> produce(IRTTI& rtti, int32_t instanceID, CreationMode creationMode) const;
	private:
		void* m_ptr;
		const Version& m_version;
	};

	static std::shared_ptr<INativeObject> s_nativeObject(void* ptr, const Version& version);
}