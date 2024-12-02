#pragma once

#include <vector>
#include <numeric>
#include <ostream>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <info.h>

NLOHMANN_JSON_NAMESPACE_BEGIN

template <typename T>
struct adl_serializer<std::shared_ptr<T>> {
	template <typename BasicJsonType> static void to_json(BasicJsonType& json_value, const std::shared_ptr<T>& ptr) {
		if (ptr.get()) {
			json_value = *ptr;
		}
		else {
			json_value = nullptr;
		}
	}

	template <typename BasicJsonType> static void from_json(const BasicJsonType& json_value, std::shared_ptr<T>& ptr) {
		T inner_val = json_value.template get<T>();
		ptr = std::make_unique<T>(std::move(inner_val));
	}
};

NLOHMANN_JSON_NAMESPACE_END

namespace UTTD {
	template <typename T>
	static std::vector<size_t> sorted(const std::vector<T>& vec) { return sorted(vec, [&vec](size_t a, size_t b) { return vec[a] < vec[b]; }); }

	template <typename T, typename Fn>
	static std::vector<size_t> sorted(const std::vector<T>& vec, Fn predicate) {
		std::vector<size_t> idx(vec.size());
		std::iota(idx.begin(), idx.end(), 0);

		std::stable_sort(idx.begin(), idx.end(), predicate);

		return idx;
	}

	template<typename T>
	bool contains(const std::vector<T>& vec, T val) { return find(vec.begin(), vec.end(), val) != vec.end(); }

	template <typename T>
	static std::ostream& operator<=(std::ostream& os, T const& value) { return os.write(reinterpret_cast<const char*>(&value), sizeof(T)); }

	static inline void to_json(nlohmann::ordered_json& json, const InfoString& type) {
		json["Index"] = type.index;
		json["String"] = type.string;
	}

	static inline void to_json(nlohmann::ordered_json& json, const InfoNode& type) {
		json["TypeName"] = type.type;
		json["Name"] = type.name;
		json["Level"] = type.level;
		json["ByteSize"] = type.byteSize;
		json["Index"] = type.index;
		json["Version"] = type.version;
		json["TypeFlags"] = type.nodeType;
		json["MetaFlag"] = type.meta;
		json["SubNodes"] = type.subNodes;
	}

	static inline void to_json(nlohmann::ordered_json& json, const InfoClass& type) {
		json["Name"] = type.name;
		json["Namespace"] = type.nameSpace;
		json["FullName"] = type.fullName;
		json["Module"] = type.module;
		json["TypeID"] = type.typeID;
		json["Base"] = type.base;
		json["Derived"] = type.derived;
		json["DescendantCount"] = type.descendantCount;
		json["Size"] = type.size;
		json["TypeIndex"] = type.typeIndex;
		json["IsAbstract"] = type.isAbstract;
		json["IsSealed"] = type.isSealed;
		json["IsEditorOnly"] = type.isEditorOnly;
		json["IsStripped"] = type.isStripped;
		json["EditorRootNode"] = type.editorRootNode;
		json["ReleaseRootNode"] = type.releaseRootNode;
	}

	static inline void to_json(nlohmann::ordered_json& json, const Info& type) {
		json["Version"] = type.version;
		json["Strings"] = type.strings;
		json["Classes"] = type.classes;
	}
}