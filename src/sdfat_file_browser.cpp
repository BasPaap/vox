#include "filebrowser/sdfat_file_browser.h"
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
	FsFile currentDirectory;
	currentDirectory.open(currentPath);

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

	currentPath[0] = '/';
	currentPath[1] = 0;
	currentDirectoryDepth = 0;

	indexCurrentDirectory();
}

void Bas::SdFatFileBrowser::goToSubDirectory(size_t index)
{
	FsFile currentDirectory;
	currentDirectory.open(currentPath);

	if (currentDirectoryDepth < maxDirectoryDepth)
	{
		parentDirectoryIndexes[currentDirectoryDepth] = currentDirectory.dirIndex();

		currentDirectory.open(fileIndexes[index]);

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

		sdCard.chdir(currentPath);	// Change the actual working directory

		indexCurrentDirectory();
	}
}

void Bas::SdFatFileBrowser::goToParentDirectory()
{
	if (currentDirectoryDepth > 0)
	{
		FsFile currentDirectory;
		currentDirectory.open(currentPath);

		// Cut the current directory name off the current path
		char currentDirectoryName[maxFileNameLength];
		currentDirectory.getName(currentDirectoryName, maxFileNameLength);
		const size_t currentPathLength = strlen(currentPath);
		currentPath[currentPathLength - (strlen(currentDirectoryName) + 1)] = '\0';	// Remove current directory name length plus trailing slash

		currentDirectoryDepth--;

		sdCard.chdir(currentPath); // Change the actual working directory

		indexCurrentDirectory();
	}
}

bool Bas::SdFatFileBrowser::getIsAtRoot()
{
	return currentDirectoryDepth == 0;
}

char *Bas::SdFatFileBrowser::getCurrentPath()
{
	return currentPath;
}

bool Bas::SdFatFileBrowser::getIsDirectory(size_t index)
{
    FsFile file;

	if (file.open(fileIndexes[index]))
	{
		return file.isDirectory();
	}

	return false;
}

void Bas::SdFatFileBrowser::getFilePath(size_t index, char *filePath, size_t filePathLength)
{
	FsFile file;

	if (file.open(fileIndexes[index]))
	{
		char fileName[maxFileNameLength];
		file.getName(fileName, maxFileNameLength);

		size_t fullFilePathLength = strlen(currentPath) + strlen(fileName);
		if (fullFilePathLength + 1 <= filePathLength)
		{
			strcpy(filePath, currentPath);
			strcpy(filePath + strlen(currentPath), fileName);
			filePath[fullFilePathLength] = '\0';
		}
	}
}

bool Bas::SdFatFileBrowser::read(bool &isDirectory, char *fileName)
{
	if (currentFileIndex < numFilesInCurrentDirectory)
	{
		FsFile currentDirectory;
		currentDirectory.open(currentPath);

		FsFile file;
		file.open(fileIndexes[currentFileIndex], O_RDONLY);

		file.getName(fileName, maxFileNameLength);
		isDirectory = file.isDirectory();

		currentFileIndex++;

		return true;
	}

	return false;
}
