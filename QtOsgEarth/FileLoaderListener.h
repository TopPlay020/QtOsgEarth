#pragma once
#include <qstring.h>

class FileLoaderListener {
public:
	virtual void onFileLoadingStart(const QString& fileName) {};
	virtual void onFileReloadingStart(){};
	virtual void onFileLoadingEnd(const QString& fileName, bool success){};
	virtual void onFileReloadingEnd(bool success){};
	virtual ~FileLoaderListener() = default;
};