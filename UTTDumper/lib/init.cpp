#include <init.h>
#include <engine.h>
#include <dumper.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <format>

int init(std::filesystem::path path) {
	try {
		UTTD::Engine engine{ path };
		engine.parse();
		if (engine.options().delay > 0) {
			std::cout << std::format("Waiting for {0} seconds...", engine.options().delay) << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(engine.options().delay));
		}
		if (!engine.initialize()) {
			std::cout << "Unable to initialize engine, aborting..." << std::endl;
			return 1;
		}

		UTTD::Dumper dumper{ engine };
		dumper.execute();
	}
	catch (const std::exception& e) {
		std::cout << "Error while dumping..." << std::endl;
		std::cout << e.what() << std::endl;
		std::cout << "Aborting..." << std::endl;
	}

	return 0;
}