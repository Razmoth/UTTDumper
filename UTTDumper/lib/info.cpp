#include <info.h>
#include <rtti.h>
#include <utils.h>

namespace UTTD {
    Info::Info(const Engine& engine, 
        Unity::TransferInstruction release = Unity::TransferInstruction::SerializeGameRelease, 
        Unity::TransferInstruction editor = Unity::TransferInstruction::None) {
        version = engine.version().str();
        strings = InfoString::s_makeList(engine.commonString());
        classes = InfoClass::s_makeList(engine, release, editor);
    }
    
    InfoClass::InfoClass(Unity::IRTTI& rtti) {
        name = rtti.name();
        nameSpace = rtti.nameSpace();
        fullName = rtti.fullName();
        module = rtti.module();
        typeID = rtti.typeID();
        base = rtti.base() != nullptr ? rtti.base()->name() : "";
        descendantCount = rtti.descendantCount();
        size = rtti.size();
        typeIndex = rtti.typeIndex();
        isAbstract = rtti.isAbstract();
        isSealed = rtti.isSealed();
        isEditorOnly = rtti.isEditorOnly();
        isStripped = rtti.isStripped();
    
        std::vector<std::shared_ptr<Unity::IRTTI>>& derived = rtti.derived();
        std::vector<std::string> names;
        std::transform(derived.begin(), derived.end(), std::back_inserter(names), [](std::shared_ptr<Unity::IRTTI> const& x) { return x != nullptr ? std::string(x->name()) : ""; });
        
        this->derived = names;
    }
    
    InfoNode::InfoNode(const Unity::ITypeTreeNode& node) {
        level = node.level();
        type = node.type();
        name = node.name();
        byteSize = node.byteSize();
        index = node.index();
        version = node.version();
        nodeType = static_cast<uint8_t>(node.nodeType());
        meta = static_cast<uint32_t>(node.meta());
        subNodes = std::vector<std::shared_ptr<UTTD::InfoNode>>();
    }
    
    InfoNode::InfoNode(const Unity::ITypeTreeNode& node, InfoNode* parent) : InfoNode(node) {
        this->parent = parent;
    }
    
    std::vector<InfoString> InfoString::s_makeList(const Unity::CommonString& commonString) {
        std::vector<InfoString> strings;

        for (const char* i = commonString.begin; i < commonString.end;) {
            size_t size = std::strlen(i);
            if (size == 0) break;
            InfoString string{ i - commonString.begin, std::string_view(i, size) };
            strings.emplace_back(string);
            i += size + 1;
        }

        return strings;
    }

    std::vector<std::shared_ptr<InfoClass>> InfoClass::s_makeList(const Engine& engine, Unity::TransferInstruction release, Unity::TransferInstruction editor) {
        std::vector<std::shared_ptr<InfoClass>> result;

        std::vector<std::shared_ptr<Unity::IRTTI>> types = engine.rtti().types();

        for (auto i : sorted(types, [&types](size_t a, size_t b) { return types[a]->typeID() < types[b]->typeID(); })) {
            const std::shared_ptr<Unity::IRTTI>& type = types[i];

            std::shared_ptr<InfoClass> infoClass = std::make_shared<InfoClass>(*type);
            
            std::shared_ptr<Unity::IRTTI> iter = type;
            while (iter->isAbstract()) {
                std::shared_ptr<Unity::IRTTI> base = iter->base();
                if (base == nullptr) {
                    break;
                }
                else {
                    iter.swap(base);
                }
            }

            if (iter->typeID() == 116) continue; // MonoManager

            std::shared_ptr<Unity::INativeObject> object = engine.nativeObject().produce(*iter, 0, Unity::CreationMode::Default);

            if (object.get() != nullptr) {
                std::shared_ptr<Unity::ITypeTree> releaseTree = engine.typeTree().typeTree(*object, release);
                std::shared_ptr<Unity::ITypeTree> editorTree = engine.typeTree().typeTree(*object, editor);

                infoClass->releaseRootNode = InfoNode::s_rootNode(*releaseTree);
                infoClass->editorRootNode = InfoNode::s_rootNode(*editorTree);
            }

            result.emplace_back(infoClass);
        }

        return result;
    }

    std::shared_ptr<InfoNode> InfoNode::s_rootNode(const Unity::ITypeTree& typeTree) {
        std::vector<std::shared_ptr<Unity::ITypeTreeNode>> nodes = typeTree.nodes();
        const Unity::ITypeTreeNode& rootNode = *nodes[0];

        std::shared_ptr<InfoNode> root = std::make_shared<InfoNode>(rootNode);

        InfoNode* iter = root.get();
        for (int32_t i = 1; i < nodes.size(); i++) {
            const Unity::ITypeTreeNode& treeNode = *nodes[i];

            while (treeNode.level() <= iter->level) {
                iter = iter->parent;
            }

            std::shared_ptr<InfoNode> node = std::make_shared<InfoNode>(treeNode, iter);

            iter->subNodes.emplace_back(node);
            iter = node.get();
        }

        return root;
    }
}