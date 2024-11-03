#ifndef _FILE_BROWSER_H
#define _FILE_BROWSER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace Bas
{
	class FileBrowser
	{
	protected:
		static const size_t maxPathLength = 257;
		static const size_t maxFileNameLength = 256;
		char currentPath[maxPathLength] = { 0 };

	public:
		virtual void goToSubDirectory(size_t index) = 0;
		virtual void goToParentDirectory() = 0;
		virtual bool getIsAtRoot() = 0;
		virtual char *getCurrentPath() = 0;
		virtual bool isDirectory(size_t index) = 0;
		virtual bool read(bool &isDirectory, char *fileName) = 0;
	};
}

#endif