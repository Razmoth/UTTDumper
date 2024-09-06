#include <engine.h>
#include <utils.h>
#include <format>
#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <toml++/toml.hpp>

#ifdef WIN32
#include <windows.h>

#define GetModuleBase(moduleName) reinterpret_cast<uintptr_t>(GetModuleHandleA(moduleName))

#else

static void* get_module_base(const char* moduleName)
{
	FILE* fp;
	long addr = 0;
	char* pch;
	char line[1024];

	fp = fopen("/proc/self/maps", "r");
	if (fp != NULL) {
		while (fgets(line, sizeof(line), fp)) {
			if (strstr(line, moduleName)) {
				pch = strtok(line, "-");
				addr = strtoul(pch, NULL, 16);
				if (addr == 0x8000)
					addr = 0;
				break;
			}
		}
		fclose(fp);
	}
	return (void*)addr;
}

#define GetModuleBase(moduleName) reinterpret_cast<uintptr_t>(get_module_base(moduleName))

#endif

namespace UTTD {
	void Engine::parse()
	{
		try {
			auto parent = m_path.parent_path();
			auto tomlPath = parent / filename;

			std::cout << std::format("Parsing config file {0}..", filename) << std::endl;

			toml::table table = toml::parse_file(tomlPath.string());

			toml::v3::node_view<toml::v3::node> engineNode = table["engine"];

			std::string name = engineNode["name"].value_or("");
			if (name.empty()) {
				throw std::runtime_error("no game is selected !!");
			}

			toml::v3::node_view<toml::v3::node> gameNode = table[name];

			std::string binary = gameNode["binary"].value_or("");
			if (binary.empty()) {
				throw std::logic_error("binary name is empty !!");
			}

			std::string outputDir = gameNode["output_dir"].value_or("");
			if (outputDir.empty()) {
#ifdef ANDROID 
				std::ifstream cmdline("/proc/self/cmdline");
				std::string pkgID;
				std::getline(cmdline, pkgID, '\0');

				std::filesystem::path output(std::format("/data/data/{0}/{1}", pkgID, name));
				outputDir = output.string();
#else
				std::filesystem::path output(parent / name);
				outputDir = output.string();
#endif
			}

			uint32_t delay = gameNode["delay"].value_or(0);
			UTTD::Unity::TransferInstruction transfer = gameNode["transfer"].value_or(UTTD::Unity::TransferInstruction::None);
			bool jsonDump = gameNode["json_dump"].value<bool>().value();
			bool textDump = gameNode["text_dump"].value_or(false);
			bool binaryDump = gameNode["binary_dump"].value_or(false);
			uintptr_t commonStringsBegin = gameNode["common_strings_begin"].value_or<uintptr_t>(0);
			uintptr_t commonStringsEnd = gameNode["common_strings_end"].value_or<uintptr_t>(0);
			uintptr_t rtti = gameNode["rtti"].value_or<uintptr_t>(0);
			uintptr_t typeTree = gameNode["type_tree"].value_or<uintptr_t>(0);
			uintptr_t typeTreeCtor = gameNode["type_tree_ctor"].value_or<uintptr_t>(0);
			uintptr_t produce = gameNode["produce"].value_or<uintptr_t>(0);

			toml::v3::node_view<toml::v3::node> versionNode = gameNode["version"];

			uintptr_t version = 0;
			if (versionNode.type() == toml::node_type::string) {
				m_version = std::make_unique<Unity::Version>(versionNode.value_or(""));
			}
			else if (versionNode.type() == toml::node_type::integer) {
				version = versionNode.value_or<uintptr_t>(0);
			}
			else {
				throw std::runtime_error(std::format("invalid version for key {0}", name));
			}

			m_options = Options{
				name,
				delay,
				binary,
				outputDir,
				transfer,
				jsonDump,
				textDump,
				binaryDump,
				version,
				commonStringsBegin,
				commonStringsEnd,
				rtti,
				typeTreeCtor,
				typeTree,
				produce
			};
		}
		catch (const toml::parse_error& err) {
			throw std::runtime_error(std::format("Error while parsing config file {0}: {1}", filename, err.description()));
		}
	}

	bool Engine::initialize()
	{
		try {
			std::cout << "Initializing engine..." << std::endl;

			uintptr_t base = GetModuleBase(m_options.binary.c_str());

			if (m_version.get() == nullptr && m_options.version != 0) {
				typedef const char* GetUnityVersion();

				GetUnityVersion* getUnityVersion = reinterpret_cast<GetUnityVersion*>(base + m_options.version);
				m_version = std::make_unique<Unity::Version>(getUnityVersion());
			}

			char* begin = reinterpret_cast<char*>(base + m_options.commonStringsBegin);
			char* end = reinterpret_cast<char*>(base + m_options.commonStringsEnd);

			m_commonString = std::make_unique<Unity::CommonString>(begin, end);

			void* rtti = reinterpret_cast<void*>(base + m_options.rtti);

			m_rtti = std::make_unique<Unity::RTTI>(rtti, *m_version);

			if (!m_rtti->initialize()) {
				std::cout << "Failed to initialize RTTI, aborting..." << std::endl;
				return false;
			}

			void* typeTree = reinterpret_cast<void*>(base + m_options.typeTree);
			void* typeTreeCtor = reinterpret_cast<void*>(base + m_options.typeTreeCtor);

			m_typeTree = std::make_unique<Unity::TypeTree>(typeTree, typeTreeCtor, *m_commonString, *m_version);

			void* produce = reinterpret_cast<void*>(base + m_options.produce);

			m_nativeObject = std::make_unique<Unity::NativeObject>(produce, *m_version);
		}
		catch (const std::exception& err) {
			throw std::runtime_error(std::format("Error while initializing engine: {0}", err.what()));
		}

		return true;
	}
}