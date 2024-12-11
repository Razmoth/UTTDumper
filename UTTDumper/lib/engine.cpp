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

#include <dlfcn.h>
#include <string>

static uintptr_t get_module_base_from_exports(const char* moduleName) {
	uintptr_t base;

	void* unitySendMessage = dlsym(RTLD_DEFAULT, "UnitySendMessage");

	Dl_info dlInfo;
	if (dladdr(unitySendMessage, &dlInfo) != 0) {
		base = reinterpret_cast<uintptr_t>(dlInfo.dli_fbase);
	}

	return base;
}

static uintptr_t get_module_base_from_maps(const char* moduleName) {
	std::ifstream maps("/proc/self/maps");

	char temp;
	ino_t inode;
	int32_t dev_major, dev_minor;
	uintptr_t begin, end, offset;
	std::string line, path, perms;
	while (std::getline(maps, line)) {
		std::istringstream ss(line);

		ss >> std::hex;
		ss >> begin >> temp >> end >> perms >> offset >> dev_major >> temp >> dev_minor;

		ss >> std::dec;
		ss >> inode;

		ss >> std::ws;
		if (std::getline(ss, path) && path.find(moduleName) != path.npos && inode == 0) {
			break;
		}
	}

	return begin;
}

static uintptr_t get_module_base(const char* moduleName) {
	uintptr_t base;

	base = get_module_base_from_exports(moduleName);
	if (base != 0) {
		return base;
	}

	std::cout << "Unable to find module base from exports, attempting to find from maps..." << std::endl;

	base = get_module_base_from_maps(moduleName);
	if (base != 0) {
		return base;
	}
	
	throw std::runtime_error(std::format("Unable to find base address for {0}", moduleName));
}

#define GetModuleBase(moduleName) get_module_base(moduleName)

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


			std::vector<uint32_t> exclude = {};
			toml::array* arr = gameNode["exclude"].as_array();
			if (arr->size() > 0 && arr->is_homogeneous<int64_t>()) {
				exclude.reserve(arr->size());
				for (const toml::node& i : *arr) {
					exclude.emplace_back(i.value_or<uint32_t>(0));
				}
			}

			uint32_t delay = gameNode["delay"].value_or(0);
			UTTD::Unity::TransferInstruction transfer = gameNode["transfer"].value_or(UTTD::Unity::TransferInstruction::None);
			bool jsonDump = gameNode["json_dump"].value<bool>().value();
			bool textDump = gameNode["text_dump"].value_or(false);
			bool binaryDump = gameNode["binary_dump"].value_or(false);
			uintptr_t commonStrings = gameNode["common_strings"].value_or<uintptr_t>(0);
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
				exclude,
				version,
				commonStrings,
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

			char* commonStrings = reinterpret_cast<char*>(base + m_options.commonStrings);

			m_commonString = std::make_unique<Unity::CommonString>(commonStrings);

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