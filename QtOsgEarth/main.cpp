#include <QApplication>
#include "MainWindow.h"
#include <osgEarth/Registry>
#include <osgText/Font>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


	QSurfaceFormat format = QSurfaceFormat::defaultFormat();
	format.setVersion(4, 6);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setSamples(8);
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setSwapInterval(0);
	QSurfaceFormat::setDefaultFormat(format);

	osg::DisplaySettings::instance()->setGLContextVersion("4.6");
	osg::DisplaySettings::instance()->setGLContextProfileMask(0x0);
	osg::DisplaySettings::instance()->setDoubleBuffer(true);

	osgEarth::initialize();
	osgEarth::Registry::instance()->setDefaultFont(osgText::readFontFile("../data/fonts/arial.ttf"));
	osgDB::Registry::instance()->getObjectCache()->addEntryToObjectCache("DefaultFont",
		osgText::readFontFile("../data/fonts/arial.ttf"));

    MainWindow window;
    window.showMaximized();

    return app.exec();
}
