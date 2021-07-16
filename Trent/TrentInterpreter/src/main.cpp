#include <fstream>
#include <sstream>
#include <filesystem>
#include "TrentInterpreter.h"
using namespace trent;

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		if (!std::filesystem::exists(argv[1]))
		{
			printf("Error: Invalid source file provided\n");
			return -1;
		}

		std::ifstream input_stream(argv[1]);
		std::stringstream sstream;
		sstream << input_stream.rdbuf();

		auto interpreter = std::make_unique<TrentInterpreter>();
		interpreter->InitializeEnvironment();
		interpreter->Interpret(sstream.str());
	}
	else
	{
		printf("Error: No source file provided...\n");
		return 1;
	}

	return 0;
}
