#include "Glare.h"
#include <QtWidgets/QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	// ���Ӷ�����֧��
	QLocale locale = QLocale::system();

	QTranslator* translator = new QTranslator;
	if (locale.language() == QLocale::Chinese)
	{
		translator->load("xiangen_zh.qm");
		app.installTranslator(translator);
	}

	// ��������
    Glare glare;
	glare.resize(1600, 1200);	// Ĭ�ϴ�СΪ 1200*800
	glare.show();

    return app.exec();
}
