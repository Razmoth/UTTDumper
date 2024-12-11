#pragma once

#include <info.h>
#include <engine.h>
#include <string_view>

namespace UTTD {
	class Dumper {
	public:
		Dumper(const Engine& engine) : m_engine(engine) {}

		void execute();
	private:
		void dumpStructData(Unity::TransferInstruction transfer) const;
		void dumpStruct(Unity::TransferInstruction transfer) const;
		void dumpStringData(const Unity::CommonString& commonString) const;
		void dumpClassesJson() const;
		void dumpHashesJson() const;
		void dumpInfoJson() const;
		void dumpRTTI() const;

		const Engine& m_engine;
		std::unique_ptr<Info> m_info;
	};

	static void dumpNodes(const InfoNode& node, std::ofstream& stream);
	static void dumpBinary(const Unity::ITypeTree& tree, std::ofstream& stream);
}