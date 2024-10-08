#pragma once


#include <QMainWindow>
#include <QObject>
#include <QEvent>
#include <QMouseEvent>
#include <QList> 
#include "GlobalEventListener.h"

class GlobalEventProvider : public QObject {
	Q_OBJECT

public:

	// Add a listener
	void addMouseEventListener(GlobalEventListener* listener) {
		listeners.append(listener);  // Changed from push_back to append
	}

	// Remove a listener
	void removeMouseEventListener(GlobalEventListener* listener) {
		listeners.removeAll(listener);  // Changed from erase to removeAll
	}

private:
	bool earthZoomingEvent = false;
	bool rightMousePressed = false;
protected:
	// Event filter to capture mouse events
	bool eventFilter(QObject* obj, QEvent* event) override {
		//if (event->type() != QEvent::Paint && event->type() != QEvent::UpdateRequest)
		//	qDebug() << event->type();

		switch (event->type()) {
		case QEvent::MouseMove: {
			if (rightMousePressed)
				earthZoomingEvent = true;
			break;
		}
		case QEvent::HoverMove: {
			notifyMouseMove();
			break;
		}
		case QEvent::MouseButtonPress: {
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
			if (mouseEvent->button() == Qt::RightButton) {
				rightMousePressed = true;
				earthZoomingEvent = false;
			}
			break;
		}
		case QEvent::MouseButtonRelease: {
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
			if (mouseEvent->button() == Qt::RightButton) {
				rightMousePressed = false;
			}
			else if (mouseEvent->button() == Qt::LeftButton) {
				notifyMouseLeftRelese();
			}
			break;
		}
		case QEvent::ContextMenu: {
			// Check if the event is from QMainWindow
			if (qobject_cast<QMainWindow*>(obj) && !earthZoomingEvent) {
				notifyContextMenu();
				earthZoomingEvent = false;
			}
			break;
		}
		}
		return QObject::eventFilter(obj, event);
	}

private:
	QList<GlobalEventListener*> listeners;  // Changed from vector to QList

	void notifyContextMenu() {
		for (auto listener : listeners) {
			listener->onContextMenu();
		}
	}

	void notifyMouseMove() {
		for (auto listener : listeners) {
			listener->onMouseMove();
		}
	}

	void notifyMouseLeftRelese() {
		for (auto listener : listeners) {
			listener->onMouseLeftRelese();
		}
	}
};
