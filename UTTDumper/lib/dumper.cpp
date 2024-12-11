#include <dumper.h>
#include <transfer.h>
#include <version.h>
#include <md4.h>
#include <info.h>
#include <utils.h>
#include <utility>
#include <iostream>
#include <format>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace UTTD
{
	void Dumper::execute() {
		std::cout << std::format("Starting dumper. UnityVersion {}", m_engine.version().str()) << std::endl;
		std::filesystem::create_directories(m_engine.options().outputDirectory);

		Unity::TransferInstruction release = m_engine.options().transfer | Unity::TransferInstruction::SerializeGameRelease;
		Unity::TransferInstruction editor = m_engine.options().transfer & ~Unity::TransferInstruction::SerializeGameRelease;

		m_info = std::make_unique<Info>(m_engine, release, editor);

		if (m_engine.options().jsonDump) {
			dumpClassesJson();
			dumpHashesJson();
			dumpInfoJson();
		}

		if (m_engine.options().textDump) {
			dumpRTTI();
			dumpStruct(release);
		}

		if (m_engine.options().binaryDump) {
			dumpStringData(m_engine.commonString());
			dumpStructData(release);
		}

		std::cout << "Success" << std::endl;
	}

	void Dumper::dumpStringData(const Unity::CommonString& commonString) const {
		std::filesystem::path outDirectory(m_engine.options().outputDirectory);
		std::ofstream os(outDirectory / "strings.dat", std::ios::binary);

		std::cout << "Writing common string buffer..." << std::endl;

		std::pair<char*, char*> range = commonString.range();
		os.write(range.first, range.second - range.first - 1);
	}

	void Dumper::dumpInfoJson() const {
		std::cout << "Writing information json..." << std::endl;

		std::filesystem::path outDirectory(m_engine.options().outputDirectory);
		std::ofstream os(outDirectory / "info.json");

		nlohmann::ordered_json json;

		to_json(json, *m_info);

		os << std::setfill('\t') << std::setw(1) << json;
	}

	void Dumper::dumpClassesJson() const {
		std::cout << "Writing classes.json..." << std::endl;

		std::filesystem::path outDirectory(m_engine.options().outputDirectory);
		std::ofstream os(outDirectory / "classes.json");

		std::vector<std::shared_ptr<UTTD::InfoClass>> classes = m_info->classes;

		nlohmann::ordered_json json;

		for (size_t i : sorted(classes, [&classes](size_t a, size_t b) { return classes[a]->typeID < classes[b]->typeID; })) {
			const InfoClass& cls = *classes[i];

			json[std::to_string(cls.typeID)] = cls.name;
		}

		os << std::setw(2) << json << std::endl;
	}

	void Dumper::dumpHashesJson() const {
		std::cout << "Writing hashes.json..." << std::endl;

		std::filesystem::path outDirectory(m_engine.options().outputDirectory);
		std::ofstream os(outDirectory / "hashes.json");

		std::vector<std::shared_ptr<UTTD::InfoClass>> classes = m_info->classes;

		nlohmann::ordered_json json;

		for (size_t i : sorted(classes, [&classes](size_t a, size_t b) { return classes[a]->typeID < classes[b]->typeID; })) {
			const InfoClass& cls = *classes[i];

			if (!cls.isAbstract && cls.releaseRootNode != nullptr) {
				std::stringstream ss;

				std::shared_ptr<MD4> md4 = std::make_shared<MD4>();
				cls.releaseRootNode->hash(md4);
				std::vector<uint8_t> hash = md4->digest();
				for (int i = 0; i < hash.size(); i++) {
					ss << std::format("{:02X}", hash[i]);
				}
				
				json[cls.name] = ss.str();
			}	
		}

		os << std::setw(2) << json << std::endl;
	}

	void Dumper::dumpRTTI() const {
		std::cout << "Writing RTTI..." << std::endl;

		std::filesystem::path outDirectory(m_engine.options().outputDirectory);
		std::ofstream os(outDirectory / "RTTI.dump");

		std::vector<std::shared_ptr<UTTD::InfoClass>> classes = m_info->classes;

		for (size_t i : sorted(classes, [&classes](size_t a, size_t b) { return classes[a]->typeID < classes[b]->typeID; })) {
			const InfoClass& cls = *classes[i];

			os << std::format("PersistentTypeID {}\n", cls.typeID);
			os << std::format("    Name {}\n", cls.name);
			os << std::format("    Namespace {}\n", cls.nameSpace);
			os << std::format("    Module {}\n", cls.module);
			os << std::format("    Base {}\n", cls.base);
			os << std::format("    DescendantCount {}\n", cls.descendantCount);
			os << std::format("    IsAbstract {}\n", cls.isAbstract ? "True" : "False");
			os << std::format("    IsSealed {}\n", cls.isSealed ? "True" : "False");
			os << std::format("    IsStripped {}\n", cls.isStripped ? "True" : "False");
			os << std::format("    IsEditorOnly {}\n", cls.isEditorOnly ? "True" : "False");
			os << std::endl;
		}
	}

	void Dumper::dumpStruct(Unity::TransferInstruction transfer) const {
		std::cout << "Writing structure information dump..." << std::endl;

		std::filesystem::path outDirectory(m_engine.options().outputDirectory);
		std::ofstream os(outDirectory / "structs.dump", std::ios::binary);

		std::vector<std::shared_ptr<UTTD::InfoClass>> classes = m_info->classes;

		int32_t typeCount = 0;
		for (size_t i : sorted(classes, [&classes](size_t a, size_t b) { return classes[a]->typeID < classes[b]->typeID; })) {
			InfoClass type = *classes[i];
			InfoClass iter = type;

			std::cout << std::format("[{0}] Child: {1}::{2}, {3}, {4}", typeCount, iter.nameSpace, iter.name, iter.module, iter.typeID) << std::endl;
			std::cout << std::format("[{0}] Getting base type...", typeCount) << std::endl;

			std::string inheritance;
			while (true) {
				inheritance += iter.name;

				if (iter.base.empty()) {
					break;
				}

				inheritance += " <- ";
				std::vector<std::shared_ptr<InfoClass>>::iterator base = std::find_if(classes.begin(), classes.end(), [&iter](std::shared_ptr<InfoClass> const& other) { return other->name == iter.base; });
				if (base != classes.end()) {
					iter = *base->get();
				}
			}

			os << std::format("\n// classID{{{0}}}: {1}\r\n", type.typeID, inheritance);
			iter = type;

			while (iter.isAbstract) {
				os << std::format("// {0} is abstract\r\n", iter.name);

				if (iter.base.empty()) {
					break;
				}

				std::vector<std::shared_ptr<InfoClass>>::iterator base = std::find_if(classes.begin(), classes.end(), [&iter](std::shared_ptr<InfoClass> const& other) { return other->name == iter.base; });
				if (base != classes.end()) {
					iter = *base->get();
				}
			}

			std::cout << std::format("[{0}] Base: {1}::{2}, {3}, {4}", typeCount, iter.nameSpace, iter.name, iter.module, iter.typeID) << std::endl;

			std::shared_ptr<InfoNode> tree = (int32_t)(transfer & Unity::TransferInstruction::SerializeGameRelease) != 0 ? iter.releaseRootNode : iter.editorRootNode;
			if (tree.get() != nullptr) {
				dumpNodes(*tree, os);
			}

			typeCount++;
		}
	}

	void Dumper::dumpStructData(Unity::TransferInstruction transfer) const {
		std::cout << "Writing structure information..." << std::endl;

		std::filesystem::path outDirectory(m_engine.options().outputDirectory);
		std::ofstream os(outDirectory / "structs.dat", std::ios::binary);

		os << m_engine.version().str();
		os <= (uint8_t)0;
		os <= (int32_t)7;
		os <= (uint8_t)1; // hasTypeTrees 

		auto countPos = os.tellp();

		std::vector<std::shared_ptr<Unity::IRTTI>> types = m_engine.rtti().types();

		int32_t typeCount = 0;
		os <= typeCount;
		for (size_t i : sorted(types, [&types](size_t a, size_t b) { return types[a]->typeID() < types[b]->typeID(); })) {
			std::shared_ptr<Unity::IRTTI> type = types[i];
			std::shared_ptr<Unity::IRTTI> iter;

			std::cout << std::format("[{0}] Child: {1}::{2}, {3}, {4}", typeCount, type->nameSpace(), type->name(), type->module(), type->typeID()) << std::endl;
			std::cout << std::format("[{0}] Getting base type...", typeCount) << std::endl;

			iter.swap(type);

			while (iter->isAbstract()) {
				std::shared_ptr<Unity::IRTTI> base = iter->base();

				if (base.get() == nullptr) {
					break;
				}

				iter.swap(base);
			}

			std::cout << std::format("[{0}] Base: {1}::{2}, {3}, {4}", typeCount, iter->nameSpace(), iter->name(), iter->module(), iter->typeID()) << std::endl;

			if (iter->typeID() == 116) continue; // MonoManager
			
			if (contains(m_engine.options().exclude, iter->typeID())) {
				std::cout << std::format("[{0}] Type {1} is excluded, skipping...", typeCount, iter->name()) << std::endl;
				continue;
			}

			std::cout << std::format("[{0}] Producing native object...", typeCount) << std::endl;
			std::shared_ptr<Unity::INativeObject> object = m_engine.nativeObject().produce(*iter, 0, Unity::CreationMode::Default);

			if (object.get() == nullptr) {
				continue;
			}

			std::cout << std::format("[{0}] Produced object {1}. Persistent = {2}.", typeCount, object->instanceID(), object->isPersistent()) << std::endl;
			std::cout << std::format("[{0}] Generating type tree...", typeCount) << std::endl;

			std::shared_ptr<Unity::ITypeTree> tree = m_engine.typeTree().typeTree(*object, transfer);

			std::cout << std::format("[{0}] Getting GUID...", typeCount) << std::endl;
			os <= iter->typeID();
			for (int32_t j = 0, n = iter->typeID() < 0 ? 0x20 : 0x10; j < n; j++) {
				os <= (uint8_t)0;
			}
				
			dumpBinary(*tree, os);

			typeCount++;
		}

		os.seekp(countPos);
		os <= typeCount;
	}

	static void dumpNodes(const InfoNode& node, std::ofstream& stream) {
		for (int32_t i = 0; i < node.level; i++) {
			stream << '\t';
		}

		stream << std::format("{0} {1} // ByteSize{{{2}}}, Index{{{3}}}, Version{{{4}}}, IsArray{{{5}}}, MetaFlag{{{6}}}\r\n", 
			node.type, 
			node.name, 
			std::format("{:x}", (uint32_t)node.byteSize), 
			std::format("{:x}", node.index),
			std::format("{:x}", node.version),
			std::format("{:x}", node.nodeType),
			std::format("{:x}", node.meta)
		);

		if (!node.subNodes.empty()) {
			for (std::shared_ptr<InfoNode> sub : node.subNodes) {
				dumpNodes(*sub, stream);
			}
		}
	}
	
	static void dumpBinary(const Unity::ITypeTree& tree, std::ofstream& stream) {
		std::vector<std::shared_ptr<Unity::ITypeTreeNode>> nodes = tree.nodes();
		Unity::DynamicArray<uint8_t> stringBuffer = tree.stringBuffer();

		stream <= (uint32_t)nodes.size();
		stream <= (uint32_t)stringBuffer.size;
		for (std::shared_ptr<Unity::ITypeTreeNode> node : nodes) {
			stream <= node->version();
			stream <= node->level();
			stream <= (uint8_t)node->nodeType();
			stream <= node->typeStrOffset();
			stream <= node->nameStrOffset();
			stream <= node->byteSize();
			stream <= node->index();
			stream <= (uint32_t)node->meta();
		}

		for (int64_t i = 0, n = stringBuffer.size; i < n; i++) {
			stream <= stringBuffer.data[i];
		}
	}
}