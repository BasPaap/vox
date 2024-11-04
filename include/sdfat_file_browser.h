#ifndef _SDFAT_FILE_BROWSER_H
#define _SDFAT_FILE_BROWSER_H

#include <SPI.h>
#include <SdFat.h>
#include "file_browser.h"

namespace Bas
{
	class SdFatFileBrowser : public FileBrowser
	{
	private:
		const uint8_t sdChipSelectPin = 9;

		SdFs sdCard;
		uint8_t currentDirectoryDepth;
		static const size_t maxFilesPerDirectory = 255;
		uint32_t fileIndexes[maxFilesPerDirectory] = {0};
		size_t numFilesInCurrentDirectory = 0;
		size_t currentFileIndex = 0;
		static const size_t maxDirectoryDepth = 64;
		uint32_t parentDirectoryIndexes[maxDirectoryDepth] = {0};
		static bool compareFileIndexes(uint32_t firstIndex, uint32_t secondIndex);
		void indexCurrentDirectory();

	public:
		void begin();
		void goToSubDirectory(size_t index) override;
		void goToParentDirectory() override;
		bool getIsAtRoot() override;
		char *getCurrentPath() override;
		bool getIsDirectory(size_t index) override;
		bool read(bool &isDirectory, char *fileName) override;
	};
}

#endif