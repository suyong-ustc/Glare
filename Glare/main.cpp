#include "Glare.h"
#include <QtWidgets/QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	// 增加多语言支持
	QLocale locale = QLocale::system();

	QTranslator* translator = new QTranslator;
	if (locale.language() == QLocale::Chinese)
	{
		translator->load("xiangen_zh.qm");
		app.installTranslator(translator);
	}

	// 启动程序
    Glare glare;
	glare.resize(1600, 1200);	// 默认大小为 1200*800
	glare.show();

    return app.exec();
}
