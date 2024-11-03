#include "sdfat_file_browser.h"
#include <ArduinoSort.h>

bool Bas::SdFatFileBrowser::compareFileIndexes(uint32_t firstIndex, uint32_t secondIndex)
{
	FsFile firstFile;
	firstFile.open(firstIndex, O_RDONLY);

	FsFile secondFile;
	secondFile.open(secondIndex, O_RDONLY);

 	if (firstFile.isDirectory() && !secondFile.isDirectory())
	{
		return false;
	}
	else if (!firstFile.isDirectory() && secondFile.isDirectory())
	{
		return true;
	}
	else
	{
		char firstFileName[maxFileNameLength];
		firstFile.getName(firstFileName, maxFileNameLength);

		char secondFileName[maxFileNameLength];
		secondFile.getName(secondFileName, maxFileNameLength);

		int result = strcmp(firstFileName, secondFileName);
		return result > 0; // Return true if second is higher than first, return false when vice versa or when they have the same name.
	}
}

void Bas::SdFatFileBrowser::indexCurrentDirectory()
{
	numFilesInCurrentDirectory = 0;
	currentFileIndex = 0;

	FsFile file;

	while (file.openNext(&currentDirectory, O_RDONLY))
	{
		if (!file.isHidden())
		{
			fileIndexes[numFilesInCurrentDirectory] = file.dirIndex();
			numFilesInCurrentDirectory++;
		}
	}

	// Sort file indexes by type (directory or file) then by name.
	sortArray(fileIndexes, numFilesInCurrentDirectory, compareFileIndexes);
}

void Bas::SdFatFileBrowser::begin()
{
	if (!sdCard.begin(SdSpiConfig(sdChipSelectPin, SHARED_SPI, SD_SCK_MHZ(50))))
	{
		sdCard.initErrorHalt(&Serial);
	}

	currentDirectory.open("/"); // Open the root folder
	currentPath[0] = '/';
	currentPath[1] = 0;
	currentDirectoryDepth = 0;

	indexCurrentDirectory();
}

void Bas::SdFatFileBrowser::goToSubDirectory(size_t index)
{
	currentDirectory.open(index);
	currentDirectoryDepth++;

	// Add the current directory name to the current path
	const size_t currentPathLength = strlen(currentPath);
	char currentDirectoryName[maxFileNameLength];
	currentDirectory.getName(currentDirectoryName, maxFileNameLength);
	const size_t currentDirectoryNameLength = strlen(currentDirectoryName);
	if ((currentPathLength + currentDirectoryNameLength + 2) <= maxPathLength) // the current path, plus the path of the sub folder, plus the trailing / and the zero terminator.
	{
		strcpy(&currentPath[currentPathLength], currentDirectoryName);
		currentPath[currentPathLength + currentDirectoryNameLength] = '/';
		currentPath[currentPathLength + currentDirectoryNameLength + 1] = 0;
	}

	indexCurrentDirectory();
}

void Bas::SdFatFileBrowser::goToParentDirectory()
{
	// Serial.println("To parent");
	// currentDirectory.open("..");
	currentDirectoryDepth--;

	indexCurrentDirectory();
}

bool Bas::SdFatFileBrowser::getIsAtRoot()
{
	return (currentDirectory.isDirectory() && currentDirectoryDepth == 0);
}

char *Bas::SdFatFileBrowser::getCurrentPath()
{
	return currentPath;
}

bool Bas::SdFatFileBrowser::read(bool &isDirectory, char *fileName)
{
	if (currentFileIndex < numFilesInCurrentDirectory)
	{
		FsFile file;
		file.open(fileIndexes[currentFileIndex], O_RDONLY);

		file.getName(fileName, maxFileNameLength);
		isDirectory = file.isDirectory();

		currentFileIndex++;

		return true;
	}

	return false;
}
