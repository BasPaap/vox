#include "sdfat_file_browser.h"
#include <ArduinoSort.h>

// bool Bas::SdFatFileBrowser::compareFileSystemEntries(FileSystemEntry first, FileSystemEntry second)
// {
// 	if (first.isDirectory && !second.isDirectory)
// 	{
// 		return false;
// 	}
// 	else if (!first.isDirectory && second.isDirectory)
// 	{
// 		return true;
// 	}
// 	else
// 	{
// 		int result = strcmp(first.name, second.name);
// 		return result > 0; // Return true if second is higher than first, return false when vice versa or when they have the same name.
// 	}
// }

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
}

// void Bas::SdFatFileBrowser::readFileSystemEntries()
// {
// 	FsFile file;
// 	numFileSystemEntries = 0;

// 	while (file.openNext(&currentDirectory, O_RDONLY) && numFileSystemEntries < maxFileSystemEntries)
// 	{
// 		if (!file.isHidden())
// 		{
// 			file.getName(fileSystemEntries[numFileSystemEntries].name, maxFileNameLength);
// 			fileSystemEntries[numFileSystemEntries].isDirectory = file.isDirectory();
// 			numFileSystemEntries++;
// 		}
// 	}

// 	sortArray(fileSystemEntries, numFileSystemEntries, &SdFatFileBrowser::compareFileSystemEntries); // Sort all entries by type (folder or file) and name.

// 	Serial.print(numFileSystemEntries);
// 	Serial.println(" file system entries in current folder.");

// 	for (size_t i = 0; i < numFileSystemEntries; i++)
// 	{
// 		Serial.print(fileSystemEntries[i].name);
// 		Serial.print("\t");
// 		Serial.println(fileSystemEntries[i].isDirectory);
// 	}
// }

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
}

void Bas::SdFatFileBrowser::goToParentDirectory()
{
	// Serial.println("To parent");
	// currentDirectory.open("..");
	// currentDirectoryDepth--;
}

bool Bas::SdFatFileBrowser::getIsAtRoot()
{
	return (currentDirectory.isDirectory() && currentDirectoryDepth == 0);
}

char *Bas::SdFatFileBrowser::getCurrentPath()
{
	return currentPath;
}

bool Bas::SdFatFileBrowser::read(bool &isDirectory, char *fileName, size_t &fileNameLength)
{
	FsFile file;

	while (file.openNext(&currentDirectory, O_RDONLY))
	{
		if (!file.isHidden())
		{
			fileNameLength = file.getName(fileName, maxFileNameLength);
			isDirectory = file.isDirectory();

			return true;
		}
	}

	return false;
}
