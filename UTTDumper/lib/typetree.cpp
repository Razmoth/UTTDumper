#include <typetree.h>
#include <iostream>

namespace UTTD::Unity {
	class TypeTreeNode_Unity2019_1 : public ITypeTreeNode {
	public:
		struct Node {
			int16_t version;
			uint8_t level;
			NodeType type;
			uint32_t typeStrOffset;
			uint32_t nameStrOffset;
			int32_t byteSize;
			int32_t index;
			TransferMeta meta;
			uint64_t refTypeHash;
		};

		TypeTreeNode_Unity2019_1(Node node, ITypeTree* owner) :
			m_owner(owner),
			m_node(node) {

		}

		int16_t version() const { return m_node.version; }
		uint8_t level() const { return m_node.level; }
		NodeType nodeType() const { return m_node.type; }
		uint32_t typeStrOffset() const { return m_node.typeStrOffset; }
		uint32_t nameStrOffset() const { return m_node.nameStrOffset; }
		std::string_view type() const { return m_owner->string(m_node.typeStrOffset); }
		std::string_view name() const { return m_owner->string(m_node.nameStrOffset); }
		int32_t byteSize() const { return m_node.byteSize; }
		int32_t index() const { return m_node.index; }
		TransferMeta meta() const { return m_node.meta; }
		void* ptr() { return static_cast<void*>(&m_node); }

	private:
		Node m_node;
		ITypeTree* m_owner;
	};

	class TypeTreeNode_Unity5_0 : public ITypeTreeNode {
	public:
		struct Node {
			int16_t version;
			uint8_t level;
			NodeType type;
			uint32_t typeStrOffset;
			uint32_t nameStrOffset;
			int32_t byteSize;
			int32_t index;
			TransferMeta meta;
		};

		TypeTreeNode_Unity5_0(Node node, ITypeTree* owner) :
			m_owner(owner),
			m_node(node) {

		}

		int16_t version() const { return m_node.version; }
		uint8_t level() const { return m_node.level; }
		NodeType nodeType() const { return m_node.type; }
		uint32_t typeStrOffset() const { return m_node.typeStrOffset; }
		uint32_t nameStrOffset() const { return m_node.nameStrOffset; }
		std::string_view type() const { return m_owner->string(m_node.typeStrOffset); }
		std::string_view name() const { return m_owner->string(m_node.nameStrOffset); }
		int32_t byteSize() const { return m_node.byteSize; }
		int32_t index() const { return m_node.index; }
		TransferMeta meta() const { return m_node.meta; }
		void* ptr() { return static_cast<void*>(&m_node); }

	private:
		Node m_node;
		ITypeTree* m_owner;
	};

	class TypeTree_Unity2022_2 : public ITypeTree {
		struct TypeTreeShareableData {
			DynamicArray<TypeTreeNode_Unity2019_1::Node> nodes;
			DynamicArray<uint8_t> levels;
			DynamicArray<int32_t> nextIndex;
			DynamicArray<uint8_t> stringBuffer;
			DynamicArray<uint32_t> offsets;
			TransferInstruction flags;
			int32_t refCount;
			MemLabelId memLabel;
		};

		struct TypeTree {
			TypeTreeShareableData* data;
			void* referencedTypes;
			bool poolOwned;
		};

		typedef void TypeConstructor(TypeTree_Unity2022_2::TypeTree* typeTree, MemLabelId label);
	public:
		TypeTree_Unity2022_2(void* ptr, const CommonString& commonString) : ITypeTree(commonString) {
			m_tree = TypeTree{};
		}

		const std::vector<std::shared_ptr<ITypeTreeNode>>& nodes() const { return m_nodes; }
		DynamicArray<uint8_t> stringBuffer() const { return m_tree.data->stringBuffer; }
		DynamicArray<uint32_t> offsets() const { return m_tree.data->offsets; }
		void* ptr() { return static_cast<void*>(&m_tree); }

		void populate() {
			m_nodes = std::vector<std::shared_ptr<ITypeTreeNode>>(m_tree.data->nodes.size);

			for (int32_t i = 0; i < m_tree.data->nodes.size; i++) {
				m_nodes[i] = std::make_shared<TypeTreeNode_Unity2019_1>(m_tree.data->nodes.data[i], static_cast<ITypeTree*>(this));
			};
		}

	private:
		TypeTree m_tree;
		std::vector<std::shared_ptr<ITypeTreeNode>> m_nodes;
	};

	class TypeTree_Unity2019_3 : public ITypeTree {
		struct TypeTreeShareableData {
			DynamicArray<TypeTreeNode_Unity2019_1::Node> nodes;
			DynamicArray<uint8_t> stringBuffer;
			DynamicArray<uint32_t> offsets;
			TransferInstruction flags;
			int32_t refCount;
			MemLabelId memLabel;
		};

		struct TypeTree {
			TypeTreeShareableData* data;
			void* referencedTypes;
			bool poolOwned;
		};

		typedef void TypeConstructor(TypeTree_Unity2019_3::TypeTree* typeTree, MemLabelId label);
	public:
		TypeTree_Unity2019_3(void* ptr, const CommonString& commonString) : ITypeTree(commonString) {
			TypeConstructor* constructor = (TypeConstructor*)ptr;
			TypeTree typeTree;
			constructor(&typeTree, MemLabel);
			m_tree = typeTree;
		}

		const std::vector<std::shared_ptr<ITypeTreeNode>>& nodes() const { return m_nodes; }
		DynamicArray<uint8_t> stringBuffer() const { return m_tree.data->stringBuffer; }
		DynamicArray<uint32_t> offsets() const { return m_tree.data->offsets; }
		void* ptr() { return static_cast<void*>(&m_tree); }

		void populate() {
			m_nodes = std::vector<std::shared_ptr<ITypeTreeNode>>(m_tree.data->nodes.size);

			for (int32_t i = 0; i < m_tree.data->nodes.size; i++) {
				m_nodes[i] = std::make_shared<TypeTreeNode_Unity2019_1>(m_tree.data->nodes.data[i], static_cast<ITypeTree*>(this));
			};
		}

	private:
		TypeTree m_tree;
		std::vector<std::shared_ptr<ITypeTreeNode>> m_nodes;
	};

	class TypeTree_Unity2019_1 : public ITypeTree {
		struct TypeTreeShareableData {
			DynamicArray<TypeTreeNode_Unity2019_1::Node> nodes;
			DynamicArray<uint8_t> stringBuffer;
			DynamicArray<uint32_t> offsets;
			TransferInstruction flags;
			int32_t refCount;
			MemLabelId memLabel;
		};

		struct TypeTree {
			TypeTreeShareableData* data;
			TypeTreeShareableData privateData;
		};

		typedef void TypeConstructor(TypeTree_Unity2019_1::TypeTree* typeTree, MemLabelId label, uint8_t flag);
	public:
		TypeTree_Unity2019_1(void* ptr, const CommonString& commonString) : ITypeTree(commonString) {
			TypeConstructor* constructor = (TypeConstructor*)ptr;
			TypeTree typeTree;
			constructor(&typeTree, MemLabel, 0);
			m_tree = typeTree;
		}

		const std::vector<std::shared_ptr<ITypeTreeNode>>& nodes() const { return m_nodes; }
		DynamicArray<uint8_t> stringBuffer() const { return m_tree.data->stringBuffer; }
		DynamicArray<uint32_t> offsets() const { return m_tree.data->offsets; }
		void* ptr() { return static_cast<void*>(&m_tree); }

		void populate() {
			m_nodes = std::vector<std::shared_ptr<ITypeTreeNode>>(m_tree.data->nodes.size);

			for (int32_t i = 0; i < m_tree.data->nodes.size; i++) {
				m_nodes[i] = std::make_shared<TypeTreeNode_Unity2019_1>(m_tree.data->nodes.data[i], static_cast<ITypeTree*>(this));
			};
		}

	private:
		TypeTree m_tree;
		std::vector<std::shared_ptr<ITypeTreeNode>> m_nodes;
	};

	class TypeTree_Unity5_3 : public ITypeTree {
		struct TypeTree {
			DynamicArray<TypeTreeNode_Unity5_0::Node> nodes;
			DynamicArray<uint8_t> stringBuffer;
			DynamicArray<uint32_t> offsets;
		};

		typedef void TypeConstructor(TypeTree_Unity5_3::TypeTree* typeTree, MemLabelId label);
	public:
		TypeTree_Unity5_3(void* ptr, const CommonString& commonString) : ITypeTree(commonString) {
			TypeConstructor* constructor = (TypeConstructor*)ptr;
			TypeTree typeTree;
			constructor(&typeTree, MemLabel);
			m_tree = typeTree;
		}

		const std::vector<std::shared_ptr<ITypeTreeNode>>& nodes() const { return m_nodes; }
		DynamicArray<uint8_t> stringBuffer() const { return m_tree.stringBuffer; }
		DynamicArray<uint32_t> offsets() const { return m_tree.offsets; }
		void* ptr() { return static_cast<void*>(&m_tree); }

		void populate() {
			m_nodes = std::vector<std::shared_ptr<ITypeTreeNode>>(m_tree.nodes.size);

			for (int32_t i = 0; i < m_tree.nodes.size; i++) {
				m_nodes[i] = std::make_shared<TypeTreeNode_Unity5_0>(m_tree.nodes.data[i], static_cast<ITypeTree*>(this));
			};
		}

	private:
		TypeTree m_tree;
		std::vector<std::shared_ptr<ITypeTreeNode>> m_nodes;
	};

	class TypeTree_Unity5_0 : public ITypeTree {
		struct TypeTree {
			DynamicArray<TypeTreeNode_Unity5_0::Node> nodes;
			DynamicArray<uint8_t> stringBuffer;
			DynamicArray<uint32_t> offsets;
		};

		typedef void TypeConstructor(TypeTree_Unity5_0::TypeTree* typeTree);
	public:
		TypeTree_Unity5_0(void* ptr, const CommonString& commonString) : ITypeTree(commonString) {
			TypeConstructor* constructor = (TypeConstructor*)ptr;
			TypeTree typeTree;
			constructor(&typeTree);
			m_tree = typeTree;
		}

		const std::vector<std::shared_ptr<ITypeTreeNode>>& nodes() const { return m_nodes; }
		DynamicArray<uint8_t> stringBuffer() const { return m_tree.stringBuffer; }
		DynamicArray<uint32_t> offsets() const { return m_tree.offsets; }
		void* ptr() { return static_cast<void*>(&m_tree); }

		void populate() {
			m_nodes = std::vector<std::shared_ptr<ITypeTreeNode>>(m_tree.nodes.size);

			for (int32_t i = 0; i < m_tree.nodes.size; i++) {
				m_nodes[i] = std::make_shared<TypeTreeNode_Unity5_0>(m_tree.nodes.data[i], static_cast<ITypeTree*>(this));
			};
		}

	private:
		TypeTree m_tree;
		std::vector<std::shared_ptr<ITypeTreeNode>> m_nodes;
	};

	std::shared_ptr<ITypeTree> s_typeTree(void* ptr, const CommonString& commonString, const Version& version) {
		if (version < s_version<5, 0>) {
			throw std::runtime_error("version not supported !!"); //TODO: add support?
		}
		else if (version < s_version<5, 3>) {
			return std::make_shared<TypeTree_Unity5_0>(ptr, commonString);
		}
		else if (version < s_version<2019, 1>) {
			return std::make_shared<TypeTree_Unity5_3>(ptr, commonString);
		}
		else if (version < s_version<2019, 3>) {
			return std::make_shared<TypeTree_Unity2019_1>(ptr, commonString);
		}
		else if (version < s_version<2022, 2>) {
			return std::make_shared<TypeTree_Unity2019_3>(ptr, commonString);
		}
		else if (version < s_version<2023, 1, 0, 'a', 2>) {
			return std::make_shared<TypeTree_Unity2022_2>(ptr, commonString);
		}
		else {
			throw std::invalid_argument("unknown version !!");
		}
	}

	std::shared_ptr<ITypeTree> TypeTree::typeTree(INativeObject& object, TransferInstruction flags) const {
		std::shared_ptr<ITypeTree> typeTree = s_typeTree(m_ctorPtr, m_commonString, m_version);

		if (m_version >= s_version<2019>) {
			GetTypeTree* getTypeTree = (GetTypeTree*)m_ptr;
			getTypeTree(object.ptr(), flags, typeTree->ptr());
		}
		else {
			GenerateTypeTree* generateTypeTree = (GenerateTypeTree*)m_ptr;
			generateTypeTree(object.ptr(), typeTree->ptr(), flags);
		}

		typeTree->populate();

		return typeTree;
	}
}
