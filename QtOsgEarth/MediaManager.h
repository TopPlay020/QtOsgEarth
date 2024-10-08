#ifndef MEDIAMANAGER_H
#define MEDIAMANAGER_H

#include <QIcon>
#include <QString>
#include <QMap>

class MediaManager {
public:
    MediaManager();
    QIcon getIcon(const QString& fileName);
    std::string getIconPath(const QIcon& icon);

private:
    QMap<QString, QIcon> iconCache;  // Cache for icons
};

#endif // MEDIAMANAGER_H
