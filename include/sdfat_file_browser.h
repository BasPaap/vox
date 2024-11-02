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
		FsFile currentDirectory;
		uint8_t currentDirectoryDepth;

	public:
		void begin();
		void goToSubDirectory(size_t index) override;
		void goToParentDirectory() override;
		bool getIsAtRoot() override;
		char *getCurrentPath() override;
		bool read(bool &isDirectory, char *fileName) override;
	};
}

#endif