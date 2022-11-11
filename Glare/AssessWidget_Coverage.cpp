#include "AssessWidget_Coverage.h"
#include <vector>
#include <QGraphicsPixmapItem>
#include <QGridLayout>
#include <QLabel>
#include <QPalette>
#include <QVBoxLayout>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "AssessSpecklePattern.h"


AssessWidget_Coverage::AssessWidget_Coverage(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}



AssessWidget_Coverage::~AssessWidget_Coverage()
{

}


void AssessWidget_Coverage::setupUI()
{
	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);

	// 显示图像
	scene_ = new QGraphicsScene;
	view_ = new QGraphicsView;
	view_->setScene(scene_);
	layout->addWidget(view_);

	// 设置参数
	QLabel* label_threshold = new QLabel(tr("Threshold"));
	doublespinbox_threshold_ = new QDoubleSpinBox;
	doublespinbox_threshold_->setRange(0, 255);
	doublespinbox_threshold_->setValue(126);

	QLabel* label_coverage = new QLabel(tr("Coverage"));
	label_coverage->setToolTip(tr("the ratio of black pixels in the binarized image"));
	doublespinbox_coverage_ = new QDoubleSpinBox;
	doublespinbox_coverage_->setRange(0, 100);
	doublespinbox_coverage_->setValue(50);
	doublespinbox_coverage_->setSuffix(" %");
	doublespinbox_coverage_->setReadOnly(true);
	doublespinbox_coverage_->setStyleSheet("color:#ba2f7b;");
	doublespinbox_coverage_->setToolTip(tr("the recommended coverage is 50%"));

	QGridLayout* parameter_layout = new QGridLayout;
	parameter_layout->addWidget(label_threshold, 0, 0);
	parameter_layout->addWidget(doublespinbox_threshold_, 0, 1);
	parameter_layout->addWidget(label_coverage, 1, 0);
	parameter_layout->addWidget(doublespinbox_coverage_, 1, 1);

	layout->addLayout(parameter_layout);

}



void AssessWidget_Coverage::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(doublespinbox_threshold_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, &AssessWidget_Coverage::SlotRenderBinaryImage);

}



void AssessWidget_Coverage::SlotNewImageImported(uchar* data, const int& h, const int& w)
{
	// 转换数据格式
	image_ = cv::Mat(h, w, CV_8UC1);
	for (int r = 0; r < h; ++r)
	{
		uchar* data_row = data + r * w;
		for (int c = 0; c < w; ++c)
		{
			image_.at<uchar>(r, c) = data_row[c];
		}
	}

	// 确定阈值
	double threshold = AssessSpecklePattern::OtsuThreshold(image_);
	doublespinbox_threshold_->setValue(threshold);

}



void AssessWidget_Coverage::SlotRenderBinaryImage(double threshold)
{
	// 判断图片是否为空
	if (image_.empty())
		return;

	// 二值化
	cv::Mat binary_image;
	cv::threshold(image_, binary_image, threshold, 255, 0);

	// 计算占空比
	int num_white_pixels = cv::countNonZero(binary_image);
	const double coverage = 1.0 - double(num_white_pixels) / (binary_image.rows * binary_image.cols);
	doublespinbox_coverage_->setValue(100 * coverage);

	// 显示图像
	const uchar* p_binary_image = (const uchar*) binary_image.data;
	QImage binary_qt_image(p_binary_image, binary_image.cols, binary_image.rows, binary_image.step, QImage::Format_Grayscale8);
	QPixmap binary_image_pixmap = QPixmap::fromImage(binary_qt_image);

	scene_->clear();
	scene_->setSceneRect(0, 0, image_.cols, image_.rows);
	QGraphicsPixmapItem* pixmapItem = scene_->addPixmap(binary_image_pixmap);
	pixmapItem->show();

}
