#include "MediaManager.h"
#include <QFileInfo>
#include <QDir>

MediaManager::MediaManager() {
}

QIcon MediaManager::getIcon(const QString& alias) {
	// Construct the resource path using the alias
	QString iconPath = QString(":/Icons/%1").arg(alias);

	// Check if the icon is already cached
	if (iconCache.contains(iconPath)) {
		return iconCache[iconPath];
	}

	// Load the icon from the resource path
	QIcon icon(iconPath);

	// Check if the icon is valid
	if (icon.isNull()) {
		qWarning() << "Failed to load icon from:" << iconPath;
		return QIcon();  // Return a null icon if loading failed
	}

	// Save the icon in the cache
	iconCache[iconPath] = icon;
	return icon;
}

std::string MediaManager::getIconPath(const QIcon& icon) {
	QString tempFolderPath = "/temp";
	QString tempFilePath = tempFolderPath + "/" + icon.name() + ".png";

	// Ensure the /temp directory exists, create it if it doesn't
	QDir dir(tempFolderPath);

	if (!dir.exists()) {
		dir.mkpath(tempFolderPath); // Create the directory
	}

	QPixmap pixmap = icon.pixmap(32, 32); // Convert QIcon to QPixmap

	// Save the QPixmap to the temporary file
	if (pixmap.save(tempFilePath)) {
		// If successful, return the file path as a std::string
		return tempFilePath.toStdString();
	}
	else {
		// Handle error (optional)
		return "";
	}
}