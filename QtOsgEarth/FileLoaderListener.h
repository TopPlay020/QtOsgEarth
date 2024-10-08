#pragma once
#include <qstring.h>

class FileLoaderListener {
public:
	virtual void onFileLoadingStart(const QString& fileName) = 0;
	virtual void onFileReloadingStart() = 0;
	virtual void onFileLoadingEnd(const QString& fileName, bool success) = 0;
	virtual void onFileReloadingEnd(bool success) = 0;
	virtual ~FileLoaderListener() = default;
};