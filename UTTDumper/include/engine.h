#pragma once

#include <rtti.h>
#include <version.h>
#include <typetree.h>
#include <common_string.h>
#include <native_object.h>
#include <filesystem>
#include <string_view>

namespace UTTD {
	struct Options {
		std::string name;
		uint32_t delay;
		std::string binary;
		std::string outputDirectory;
		Unity::TransferInstruction transfer = Unity::TransferInstruction::SerializeGameRelease;
		bool jsonDump = true;
		bool textDump = true;
		bool binaryDump = true;
		uintptr_t version = 0;
		uintptr_t commonStringsBegin = 0;
		uintptr_t commonStringsEnd = 0;
		uintptr_t rtti = 0;
		uintptr_t typeTreeCtor = 0;
		uintptr_t typeTree = 0;
		uintptr_t produce = 0;
	};

	class Engine {
	protected:
		static constexpr std::string_view filename = "config.toml";

	public:
		Engine(std::filesystem::path path) : 
			m_path(path) {

		}

		void parse();
		bool initialize();

		const Options& options() const { return m_options; }
		const Unity::Version& version() const { return *m_version; }
		const Unity::CommonString& commonString() const { return *m_commonString; }
		const Unity::RTTI& rtti() const { return *m_rtti; }
		const Unity::TypeTree& typeTree() const { return *m_typeTree; }
		const Unity::NativeObject& nativeObject() const { return *m_nativeObject; }
	private:
		Options m_options;
		std::filesystem::path m_path;

		std::unique_ptr<Unity::Version> m_version;
		std::unique_ptr<Unity::CommonString> m_commonString;
		std::unique_ptr<Unity::RTTI> m_rtti;
		std::unique_ptr<Unity::TypeTree> m_typeTree;
		std::unique_ptr<Unity::NativeObject> m_nativeObject;
	};
}