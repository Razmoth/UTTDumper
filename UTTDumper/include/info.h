#pragma once

#include <md4.h>
#include <engine.h>
#include <transfer.h>
#include <nlohmann/json.hpp>
#include <string_view>
#include <vector>

namespace UTTD {
	struct InfoString {
		size_t index;
		std::string_view string;

		static std::vector<InfoString> s_makeList(const Unity::CommonString& commonString);
	};

	struct InfoNode {
		std::string type;
		std::string name;
		uint8_t level;
		int32_t byteSize;
		int32_t index;
		int16_t version;
		uint8_t nodeType;
		uint32_t meta;
		std::vector<std::shared_ptr<InfoNode>> subNodes;
		struct InfoNode* parent;

		InfoNode(const Unity::ITypeTreeNode& node);
		InfoNode(const Unity::ITypeTreeNode& node, InfoNode* parent);

		void hash(std::shared_ptr<MD4> md4);

		static std::shared_ptr<InfoNode> s_rootNode(const Unity::ITypeTree& typeTree);
	};

	struct InfoClass {
		std::string name;
		std::string nameSpace;
		std::string fullName;
		std::string module;
		int32_t typeID;
		std::string base;
		std::vector<std::string> derived;
		uint32_t descendantCount;
		int32_t size;
		uint32_t typeIndex;
		bool isAbstract;
		bool isSealed;
		bool isEditorOnly;
		bool isStripped;
		std::shared_ptr<InfoNode> editorRootNode;
		std::shared_ptr<InfoNode> releaseRootNode;

		InfoClass(Unity::IRTTI& rtti);

		static std::vector<std::shared_ptr<InfoClass>> s_makeList(const Engine& engine, Unity::TransferInstruction release, Unity::TransferInstruction editor);
	};

	struct Info {
		std::string version;
		std::vector<InfoString> strings;
		std::vector<std::shared_ptr<UTTD::InfoClass>> classes;

		Info(const Engine& engine, Unity::TransferInstruction release, Unity::TransferInstruction editor);
	};
}