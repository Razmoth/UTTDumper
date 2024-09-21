#pragma once

#include <version.h>
#include <transfer.h>
#include <native_object.h>
#include <common_string.h>
#include <memory>
#include <vector>

namespace UTTD::Unity {
	enum class NodeType : uint8_t {
		None,
		IsArray						= 1 << 0,
		IsManagedReference			= 1 << 1,
		IsManagedReferenceRegistry	= 1 << 2,
		IsArrayOfRefs				= 1 << 3,
	};

	enum class TransferMeta {
		None,
		HideInEditor							= 1 << 0,
		NotEditable								= 1 << 4,
		StrongPPtr								= 1 << 6,
		TreatIntegerValueAsBoolean				= 1 << 8,
		SimpleEditor							= 1 << 11,
		DebugProperty							= 1 << 12,
		AlignBytes								= 1 << 14,
		AnyChildUsesAlignBytesFlag				= 1 << 15,
		IgnoreWithInspectorUndo					= 1 << 16,
		EditorDisplaysCharacterMap				= 1 << 18,
		IgnoreInMetaFiles						= 1 << 19,
		TransferAsArrayEntryNameInMetaFiles		= 1 << 20,
		TransferUsingFlowMappingStyle			= 1 << 21,
		GenerateBitwiseDifferences				= 1 << 22,
		DontAnimate								= 1 << 23,
		TransferHex64							= 1 << 24,
		CharPropertyMask						= 1 << 25,
		DontValidateUTF8						= 1 << 26,
		FixedBuffer								= 1 << 27,
		DisallowSerializedPropertyModification	= 1 << 28,
	};

	template <typename T>
	struct DynamicArray {
		T* data;
		int32_t label; //TODO: template
		size_t size;
		size_t capacity;
	};

	class ITypeTreeNode {
	public:
		virtual int16_t version() const = 0;
		virtual uint8_t level() const = 0;
		virtual NodeType nodeType() const = 0;
		virtual uint32_t typeStrOffset() const = 0;
		virtual uint32_t nameStrOffset() const = 0;
		virtual std::string_view type() const = 0;
		virtual std::string_view name() const = 0;
		virtual int32_t byteSize() const = 0;
		virtual int32_t index() const = 0;
		virtual TransferMeta meta() const = 0;
		virtual void* ptr() = 0;
	};

	class ITypeTree {
	protected:
		const int32_t MemLabel = 0x53;
	public:
		ITypeTree(const CommonString& commonString) : m_commonString(commonString) { }

		virtual const std::vector<std::shared_ptr<ITypeTreeNode>>& nodes() const = 0;
		virtual DynamicArray<uint8_t> stringBuffer() const = 0;
		virtual DynamicArray<uint32_t> offsets() const = 0;
		virtual void* ptr() = 0;

		virtual void populate() = 0;

		std::string_view string(uint32_t offset) const {
			if (offset > INT_MAX) {
				return m_commonString.string(offset & INT_MAX);
			};

			return std::string_view{ (char*)(stringBuffer().data + offset) };
		}
	private:
		const CommonString& m_commonString;
	};

	class TypeTree {
	protected:
		typedef unsigned char GetTypeTree(void* object, TransferInstruction flags, void* typetree);
		typedef void GenerateTypeTree(void* object, void* typetree, TransferInstruction flags);

	public:
		TypeTree(void* ptr, void* ctor, const CommonString& commonString, const Version& version) :
			m_ptr(ptr),
			m_ctorPtr(ctor),
			m_version(version),
			m_commonString(commonString) {

		}

		std::shared_ptr<ITypeTree> typeTree(INativeObject& object, TransferInstruction flags) const;
	private:
		void* m_ptr;
		void* m_ctorPtr;
		const Version& m_version;
		const CommonString& m_commonString;
	};

	static std::shared_ptr<ITypeTree> s_typeTree(void* ptr, const CommonString& commonString, const Version& version);
}