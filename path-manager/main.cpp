#include <iostream>
#include <fstream>
#include "path-manager.h"

PathManager manager;
std::ofstream output;

unsigned int indent = 0;

void Indent(void)
{
	for (unsigned int n = 0; n < indent; ++n) output << "	";
}

void ScorpStart(void)
{
	Indent();
	output << "{" << std::endl;
	indent++;
}

void ScorpEnd(void)
{
	indent--;
	Indent();
	output << "}" << std::endl;
}

void ProcessFolderRecursive(void)
{
	auto folders = manager.Folders();
	for (auto & folder : folders)
	{
		Indent();
		output << "namespace " << folder.stem() << std::endl;
		
		ScorpStart();

		manager.SetPath(folder);

		auto files = manager.Files();
		
		for (auto & file : files)
		{
			Indent();
			output << "constexpr const char * " << file.stem() << " = \"" << file.string() << "\";" << std::endl;
		}

		ProcessFolderRecursive();

		ScorpEnd();
	}
}

int main(void)
{
	output.open("resource.h");

	manager.SetPath("Resource");

	output << "namespace Resource" << std::endl;
	
	ScorpStart();

	ProcessFolderRecursive();

	ScorpEnd();

	output.close();

	rewind(stdin);
	getchar();

	return 0;
}