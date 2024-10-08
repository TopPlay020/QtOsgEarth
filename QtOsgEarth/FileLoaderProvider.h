#pragma once


#include <QList>  // Changed from vector to QList
#include <QString>
#include "FileLoaderListener.h"

class FileLoaderProvider {
    QList<FileLoaderListener*> listeners;  // Changed from std::vector to QList

public:
    void addFileLoaderListener(FileLoaderListener* listener) {
        listeners.append(listener);  // Changed from push_back to append
    }

    void removeFileLoaderListener(FileLoaderListener* listener) {
        listeners.removeAll(listener);  // Changed from erase to removeAll
    }

    void notifyFileLoadingStart(const QString& fileName) {
        for (auto* listener : listeners) {
            listener->onFileLoadingStart(fileName);
        }
    }

    void notifyFileReloadingStart() {
        for (auto* listener : listeners) {
            listener->onFileReloadingStart();
        }
    }

    void notifyFileLoadingEnd(const QString& fileName, bool success) {
        for (auto* listener : listeners) {
            listener->onFileLoadingEnd(fileName, success);
        }
    }

    void notifyFileReloadingEnd(bool success) {
        for (auto* listener : listeners) {
            listener->onFileReloadingEnd(success);
        }
    }

};

