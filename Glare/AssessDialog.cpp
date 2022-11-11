#include "AssessDialog.h"
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QHBoxLayout>
#include <QImage>
#include <QPixmap>
#include <QVBoxLayout>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>



AssessDialog::AssessDialog(QWidget *parent)
	: QDialog(parent)
{
	setupUI();

	image_ = new QImage;

	bool is_ok = connect(pushbutton_import_, &QPushButton::clicked,
						 this, &AssessDialog::SlotImportImage);


	resize(QSize(1200, 600));
}



AssessDialog::~AssessDialog()
{
	delete image_;
}



void AssessDialog::setupUI()
{
	setWindowTitle(tr("Image Assessment"));

	// 图片
	QVBoxLayout* left_layout = new QVBoxLayout;
	speckle_scene_ = new SpeckleScene;
	speckle_view_ = new SpeckleView;
	speckle_view_->setScene(speckle_scene_);
	pushbutton_import_ = new QPushButton(tr("Import"));
	left_layout->addWidget(speckle_view_);
	left_layout->addWidget(pushbutton_import_);

	// 评价
	widget_coverage_ = new AssessWidget_Coverage();
	widget_autocorrelation_ = new AssessWidget_Autocorrelation();
	widget_sssig_ = new AssessWidget_SSSIG();
	widget_interplation_bias_ = new AssessWidget_InterpolationBias();

	tabwidget_assess_ = new QTabWidget;
	tabwidget_assess_->insertTab(0, widget_coverage_, tr("Speckle coverage"));
	tabwidget_assess_->insertTab(1, widget_autocorrelation_, tr("Speckle size"));
	tabwidget_assess_->insertTab(2, widget_interplation_bias_, tr("Systematic error"));
	tabwidget_assess_->insertTab(3, widget_sssig_, tr("Random error"));

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addLayout(left_layout);
	layout->addWidget(tabwidget_assess_);
	setLayout(layout);

}



void AssessDialog::connectSlots()
{

}



void AssessDialog::SlotImportImage()
{
	// 导出路径
	QFileDialog dialog(this);
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setNameFilter(tr("Images (*.bmp *tif *tiff *jpg *jpeg *png)"));
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	dialog.exec();
	QStringList files = dialog.selectedFiles();
	if (files.isEmpty())
		return;
	QString file_name = files.at(0);

	// 打开文件
	cv::Mat cv_image = cv::imread(std::string((const char*)file_name.toLocal8Bit()), cv::IMREAD_GRAYSCALE);
	if (cv_image.empty())
		return;

	// 图片格式转化
	delete image_;
	image_ = new QImage(cv_image.data, cv_image.cols, cv_image.rows, QImage::Format_Grayscale8);

	// 图片显示
	QImage img(cv_image.cols, cv_image.rows, QImage::Format_Grayscale8);
	uchar* sptr, * dptr;
	int linesize = cv_image.cols;
	for (int y = 0; y < cv_image.rows; y++) {
		sptr = cv_image.ptr(y);
		dptr = img.scanLine(y);
		memcpy(dptr, sptr, linesize);
	}

	QPixmap image_pixmap = QPixmap::fromImage(img);

	speckle_scene_->clear();
	speckle_scene_->setSceneRect(0, 0, image_->width(), image_->height());
	QGraphicsPixmapItem* pixmapItem = speckle_scene_->addPixmap(image_pixmap);
	pixmapItem->show();

	widget_autocorrelation_->SlotNewImageImported(image_->bits(), image_->height(), image_->width());
	widget_coverage_->SlotNewImageImported(image_->bits(), image_->height(), image_->width());
	widget_sssig_->SlotNewImageImported(image_->bits(), image_->height(), image_->width());
	widget_interplation_bias_->SlotNewImageImported(image_->bits(), image_->height(), image_->width());
}



void AssessDialog::setAssessType(const AssessType& assess_type)
{
	if (assess_type == ASSESS_COVERAGE)
		tabwidget_assess_->setCurrentIndex(0);
	else if (assess_type == ASSESS_AUTOCORRELATION)
		tabwidget_assess_->setCurrentIndex(1);
	else if (assess_type == ASSESS_BIAS)
		tabwidget_assess_->setCurrentIndex(2);
	else if (assess_type == ASSESS_NOISE)
		tabwidget_assess_->setCurrentIndex(3);

}