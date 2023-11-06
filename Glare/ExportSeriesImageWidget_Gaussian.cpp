#include "ExportSeriesImageWidget_Gaussian.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <armadillo>
#include "InverseMap.h"
#include "Pattern\RenderPatternPixmap.h"
using namespace arma;



ExportSeriesImageWidget_Gaussian::ExportSeriesImageWidget_Gaussian(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}



ExportSeriesImageWidget_Gaussian::~ExportSeriesImageWidget_Gaussian()
{

}



void ExportSeriesImageWidget_Gaussian::setupUI()
{
	QLabel* label_parameter = new QLabel(tr("Displacement : u = a * exp[-(x-x0)^2/c^2]"));

	QGridLayout* parameter_layout = new QGridLayout;

	QLabel* label_start = new QLabel(tr("Start"));
	QLabel* label_end = new QLabel(tr("End"));
	QLabel* label_total_number = new QLabel(tr("Total number"));
	parameter_layout->addWidget(label_start, 0, 1);
	parameter_layout->addWidget(label_end, 0, 2);
	parameter_layout->addWidget(label_total_number, 0, 3);

	QLabel* label_a = new QLabel("a");
	doublespinbox_a_start_ = new QDoubleSpinBox;
	doublespinbox_a_start_->setRange(-10, 10);
	doublespinbox_a_start_->setValue(0);
	doublespinbox_a_end_ = new QDoubleSpinBox;
	doublespinbox_a_end_->setRange(-10, 10);
	doublespinbox_a_end_->setValue(5);
	spinbox_a_total_number_ = new QSpinBox;
	spinbox_a_total_number_->setRange(2, 100);
	spinbox_a_total_number_->setValue(11);
	parameter_layout->addWidget(label_a, 1, 0);
	parameter_layout->addWidget(doublespinbox_a_start_, 1, 1);
	parameter_layout->addWidget(doublespinbox_a_end_, 1, 2);
	parameter_layout->addWidget(spinbox_a_total_number_, 1, 3);

	QLabel* label_x0 = new QLabel("x0");
	doublespinbox_x0_start_ = new QDoubleSpinBox;
	doublespinbox_x0_start_->setRange(10, 500);
	doublespinbox_x0_start_->setValue(150);
	doublespinbox_x0_end_ = new QDoubleSpinBox;
	doublespinbox_x0_end_->setRange(10, 500);
	doublespinbox_x0_end_->setValue(150);
	spinbox_x0_total_number_ = new QSpinBox;
	spinbox_x0_total_number_->setRange(2, 100);
	spinbox_x0_total_number_->setValue(11);
	parameter_layout->addWidget(label_x0, 2, 0);
	parameter_layout->addWidget(doublespinbox_x0_start_, 2, 1);
	parameter_layout->addWidget(doublespinbox_x0_end_, 2, 2);
	parameter_layout->addWidget(spinbox_x0_total_number_, 2, 3);

	QLabel* label_c = new QLabel("c");
	doublespinbox_c_start_ = new QDoubleSpinBox;
	doublespinbox_c_start_->setRange(1, 100);
	doublespinbox_c_start_->setValue(20);
	doublespinbox_c_end_ = new QDoubleSpinBox;
	doublespinbox_c_end_->setRange(1, 100);
	doublespinbox_c_end_->setValue(20);
	spinbox_c_total_number_ = new QSpinBox;
	spinbox_c_total_number_->setRange(2, 100);
	spinbox_c_total_number_->setValue(11);
	parameter_layout->addWidget(label_c, 3, 0);
	parameter_layout->addWidget(doublespinbox_c_start_, 3, 1);
	parameter_layout->addWidget(doublespinbox_c_end_, 3, 2);
	parameter_layout->addWidget(spinbox_c_total_number_, 3, 3);

	QHBoxLayout* save_series_images_layout = new QHBoxLayout;
	pushbutton_save_series_images_ = new QPushButton(tr("Save series images"));
	progressbar_save_series_images_ = new QProgressBar();
	progressbar_save_series_images_->setMaximum(0);
	progressbar_save_series_images_->setMaximum(11);
	progressbar_save_series_images_->setValue(0);
	progressbar_save_series_images_->setAlignment(Qt::AlignHCenter);
	progressbar_save_series_images_->setFormat(tr("Progress: %v/%m"));
	save_series_images_layout->addWidget(pushbutton_save_series_images_);
	save_series_images_layout->addWidget(progressbar_save_series_images_);
	save_series_images_layout->setStretchFactor(pushbutton_save_series_images_, 1);
	save_series_images_layout->setStretchFactor(progressbar_save_series_images_, 2);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(label_parameter);
	layout->addSpacing(10);
	layout->addLayout(parameter_layout);
	layout->addStretch();
	layout->addLayout(save_series_images_layout);

	setLayout(layout);

}



void ExportSeriesImageWidget_Gaussian::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(spinbox_a_total_number_, QOverload<int>::of(&QSpinBox::valueChanged),
					this, &ExportSeriesImageWidget_Gaussian::SlotATotalValueChanged);

	is_ok = connect(spinbox_x0_total_number_, QOverload<int>::of(&QSpinBox::valueChanged),
					this, &ExportSeriesImageWidget_Gaussian::SlotX0TotalValueChanged);

	is_ok = connect(spinbox_c_total_number_, QOverload<int>::of(&QSpinBox::valueChanged),
					this, &ExportSeriesImageWidget_Gaussian::SlotCTotalValueChanged);

	is_ok = connect(pushbutton_save_series_images_, &QPushButton::clicked,
					this, &ExportSeriesImageWidget_Gaussian::SlotSaveSeriesImages);
}



void ExportSeriesImageWidget_Gaussian::SlotSaveSeriesImages()
{
	// 导出的文件名
	QFileDialog dialog(this);
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setNameFilter(tr("Images (*.bmp)"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.exec();
	QStringList files = dialog.selectedFiles();
	if (files.isEmpty())
		return;
	QString bmp_name = files.at(0);
	QString prefix = bmp_name.split(".")[0];

	// 图像尺寸
	const int width = image_size_.width();
	const int height = image_size_.height();

	// 读取变形序列参数
	const double a_start = doublespinbox_a_start_->value();
	const double a_end = doublespinbox_a_end_->value();
	const int image_total_number = spinbox_a_total_number_->value();
	const vec a_vec = linspace(a_start, a_end, image_total_number);

	const double x0_start = doublespinbox_x0_start_->value();
	const double x0_end = doublespinbox_x0_end_->value();
	const vec x0_vec = linspace(x0_start, x0_end, image_total_number);

	const double c_start = doublespinbox_c_start_->value();
	const double c_end = doublespinbox_c_end_->value();
	const vec c_vec = linspace(c_start, c_end, image_total_number);

	// 设置进度条
	progressbar_save_series_images_->setMaximum(image_total_number);
	progressbar_save_series_images_->setValue(0);

	// 采样点在变形图中的位置
	rowvec deform_x = regspace<rowvec>(0, width - 1);
	vec refer_y = regspace<vec>(0, height - 1);

	// 存储图片
	for (int i = 0; i < image_total_number; ++i)
	{
		// 变形参数
		const double a = a_vec(i);
		const double x0 = x0_vec(i);
		const double c = c_vec(i);

		// 采样点在参考图中的位置
		const rowvec refer_x = InverseMap::InverseMap_GaussianDeformation(a, x0, c, width);
		const mat xmat = repmat(refer_x, height, 1);
		const mat ymat = repmat(refer_y, 1, width);

		// 生成图像
		QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, xmat, ymat, Qt::black, false);

		// 存储图像
		QString image_name = QString("%1_%2.bmp").arg(prefix).arg(i, 3, 10, QLatin1Char('0'));
		pixmap.save(image_name, "BMP");

		// 设置进度条
		progressbar_save_series_images_->setValue(i + 1);

	}


}



void ExportSeriesImageWidget_Gaussian::SlotATotalValueChanged(int total_number)
{
	spinbox_c_total_number_->setValue(total_number);
	spinbox_x0_total_number_->setValue(total_number);
}



void ExportSeriesImageWidget_Gaussian::SlotX0TotalValueChanged(int total_number)
{
	spinbox_a_total_number_->setValue(total_number);
	spinbox_c_total_number_->setValue(total_number);
}



void ExportSeriesImageWidget_Gaussian::SlotCTotalValueChanged(int total_number)
{
	spinbox_a_total_number_->setValue(total_number);
	spinbox_x0_total_number_->setValue(total_number);
}
