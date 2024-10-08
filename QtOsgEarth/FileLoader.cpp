#include <IOsgFileLoadingListener.h>

class FileLoader {
	std::vector<IOsgFileLoadingListener*> listeners;

public:
	void addListener(IOsgFileLoadingListener* listener) {
		listeners.push_back(listener);
	}

	void removeListener(IOsgFileLoadingListener* listener) {
		listeners.erase(remove(listeners.begin(), listeners.end(), listener), listeners.end());
	}

	void notifyFileLoadingStart(const QString& fileName) {
		for (auto* listener : listeners) {
			listener->onFileLoadingStart(fileName);
		}
	}

	void notifyFileLoadingEnd(const QString& fileName, bool success) {
		for (auto* listener : listeners) {
			listener->onFileLoadingEnd(fileName, success);
		}
	}

};