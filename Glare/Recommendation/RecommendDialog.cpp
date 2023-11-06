#include "RecommendDialog.h"
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMargins>
#include <QPageSize>
#include <QPdfWriter>
#include <QVBoxLayout>
#include <armadillo>
#include "..\EllipseSpeckleItem.h"

using namespace arma;


RecommendDialog::RecommendDialog(QWidget *parent)
	: QDialog(parent)
{
	setupUI();
	connectSlots();

	resize(QSize(800, 600));


	speckle_pattern_ = Q_NULLPTR;
	PaintSpecklePattern();
}



RecommendDialog::~RecommendDialog()
{

}



void RecommendDialog::setupUI()
{
	setWindowTitle(tr("Speckle Pattern Recommendation"));

	QHBoxLayout* layout = new QHBoxLayout;

	// 参数面板
	QVBoxLayout* parameter_panel_layout = new QVBoxLayout;

	parameter_panel_layout->addWidget(setupFOVGroupBox());
	parameter_panel_layout->addWidget(setupDistanceGroupBox());
	parameter_panel_layout->addWidget(setupImageGroupBox());
	parameter_panel_layout->addStretch();
	pushbutton_export_ = new QPushButton(tr("Export"));
	parameter_panel_layout->addWidget(pushbutton_export_);

	layout->addLayout(parameter_panel_layout);

	// 散斑显示区域
	speckle_view_ = new SpeckleView(this);
	speckle_scene_ = new SpeckleScene;
	speckle_view_->setScene(speckle_scene_);
	speckle_view_->setMinimumSize(QSize(400, 400));
	layout->addWidget(speckle_view_);

	setLayout(layout);

}



QGroupBox* RecommendDialog::setupFOVGroupBox()
{
	QGroupBox* groupbox_fov = new QGroupBox(tr("Field of View"));

	// 设置布局
	QGridLayout* layout = new QGridLayout;
	groupbox_fov->setLayout(layout);

	// 视场大小
	QLabel* label_fov_width = new QLabel(tr("FOV Width:"));
	layout->addWidget(label_fov_width, 0, 0);

	doublespinbox_fov_width_ = new QDoubleSpinBox();
	doublespinbox_fov_width_->setRange(1e-10, 10000);
	doublespinbox_fov_width_->setValue(200);
	doublespinbox_fov_width_->setSuffix(tr(" mm"));
	layout->addWidget(doublespinbox_fov_width_, 0, 1);

	QLabel* label_fov_height = new QLabel(tr("FOV Height:"));
	layout->addWidget(label_fov_height, 1, 0);

	doublespinbox_fov_height_ = new QDoubleSpinBox();
	doublespinbox_fov_height_->setRange(1e-10, 10000);
	doublespinbox_fov_height_->setValue(200);
	doublespinbox_fov_height_->setSuffix(tr(" mm"));
	layout->addWidget(doublespinbox_fov_height_, 1, 1);

	return groupbox_fov;

}



QGroupBox* RecommendDialog::setupDistanceGroupBox()
{
	QGroupBox* groupbox_distance = new QGroupBox(tr("Distance"));

	// 设置布局
	QGridLayout* layout = new QGridLayout;
	groupbox_distance->setLayout(layout);

	// 物距
	QLabel* label_object_distance = new QLabel(tr("Object Distance:"));
	layout->addWidget(label_object_distance, 0, 0);

	doublespinbox_object_distance_ = new QDoubleSpinBox();
	doublespinbox_object_distance_->setRange(1e-10, 10000);
	doublespinbox_object_distance_->setValue(1000);
	doublespinbox_object_distance_->setSuffix(tr(" mm"));
	layout->addWidget(doublespinbox_object_distance_, 0, 1);

	// 像距
	QLabel* label_image_distance = new QLabel(tr("Image Distance:"));
	layout->addWidget(label_image_distance, 1, 0);

	doublespinbox_image_distance_ = new QDoubleSpinBox();
	doublespinbox_image_distance_->setRange(1e-10, 500);
	doublespinbox_image_distance_->setValue(20);
	doublespinbox_image_distance_->setSuffix(tr(" mm"));
	layout->addWidget(doublespinbox_image_distance_, 1, 1);

	return groupbox_distance;

}



QGroupBox* RecommendDialog::setupImageGroupBox()
{
	QGroupBox* groupbox_image = new QGroupBox(tr("Image"));

	// 设置布局
	QGridLayout* layout = new QGridLayout;
	groupbox_image->setLayout(layout);

	// 像元尺寸
	QLabel* label_pixel_size = new QLabel(tr("Pixel Size:"));
	layout->addWidget(label_pixel_size, 0, 0);

	doublespinbox_pixel_size_ = new QDoubleSpinBox();
	doublespinbox_pixel_size_->setRange(0.5, 30);
	doublespinbox_pixel_size_->setValue(5);
	doublespinbox_pixel_size_->setSuffix(tr(" um"));
	layout->addWidget(doublespinbox_pixel_size_, 0, 1);

	// 图像大小
	QLabel* label_image_width = new QLabel(tr("Image Width:"));
	layout->addWidget(label_image_width, 1, 0);

	doublespinbox_image_width_ = new QDoubleSpinBox();
	doublespinbox_image_width_->setRange(0.5, 10000);
	doublespinbox_image_width_->setValue(800);
	doublespinbox_image_width_->setSingleStep(1);
	doublespinbox_image_width_->setDecimals(0);
	doublespinbox_image_width_->setSuffix(tr(" Pixel"));
	doublespinbox_image_width_->setStyleSheet("color:blue");
	layout->addWidget(doublespinbox_image_width_, 1, 1);

	// 像元尺寸
	QLabel* label_image_height = new QLabel(tr("Image Height:"));
	layout->addWidget(label_image_height, 2, 0);

	doublespinbox_image_height_ = new QDoubleSpinBox();
	doublespinbox_image_height_->setRange(0.5, 10000);
	doublespinbox_image_height_->setValue(800);
	doublespinbox_image_height_->setSingleStep(1);
	doublespinbox_image_height_->setDecimals(0);
	doublespinbox_image_height_->setSuffix(tr(" Pixel"));
	doublespinbox_image_height_->setStyleSheet("color:blue");
	layout->addWidget(doublespinbox_image_height_, 2, 1);

	return groupbox_image;

}



void RecommendDialog::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(doublespinbox_fov_width_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
					this, &RecommendDialog::SlotFovWidthChanged);
	is_ok = connect(doublespinbox_fov_height_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
					this, &RecommendDialog::SlotFovHeightChanged);

	is_ok = connect(doublespinbox_object_distance_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
					this, &RecommendDialog::SlotObjectDistanceChanged);
	is_ok = connect(doublespinbox_image_distance_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
					this, &RecommendDialog::SlotImageDistanceChanged);

	is_ok = connect(doublespinbox_pixel_size_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
					this, &RecommendDialog::SlotPixelSizeChanged);
	is_ok = connect(doublespinbox_image_width_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
					this, &RecommendDialog::SlotImageWidthChanged);
	is_ok = connect(doublespinbox_image_height_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
					this, &RecommendDialog::SlotImageHeightChanged);

	is_ok = connect(pushbutton_export_, &QPushButton::clicked,
					this, &RecommendDialog::SlotExport);

}


void RecommendDialog::SlotFovWidthChanged(double fov_width)
{
	// 放大率
	const double object_distance = doublespinbox_object_distance_->value();
	const double image_distance = doublespinbox_image_distance_->value();
	const double ratio = image_distance / object_distance;

	// 图像宽度
	const double pixel_size = doublespinbox_pixel_size_->value();
	const double image_width = 1000 * ratio * fov_width / pixel_size;

	// 设置图像宽度
	doublespinbox_image_width_->blockSignals(true);
	doublespinbox_image_width_->setValue(floor(image_width));
	doublespinbox_image_width_->blockSignals(false);

	PaintSpecklePattern();

}



void RecommendDialog::SlotFovHeightChanged(double fov_height)
{
	// 放大率
	const double object_distance = doublespinbox_object_distance_->value();
	const double image_distance = doublespinbox_image_distance_->value();
	const double ratio = image_distance / object_distance;

	// 图像宽度
	const double pixel_size = doublespinbox_pixel_size_->value();
	const double image_height = 1000 * ratio * fov_height / pixel_size;

	// 设置图像宽度
	doublespinbox_image_height_->blockSignals(true);
	doublespinbox_image_height_->setValue(floor(image_height));
	doublespinbox_image_height_->blockSignals(false);

	PaintSpecklePattern();

}



void RecommendDialog::SlotObjectDistanceChanged(double object_distance)
{
	// 放大率
	const double image_distance = doublespinbox_image_distance_->value();
	const double ratio = image_distance / object_distance;

	// 视场大小
	const double fov_width = doublespinbox_fov_width_->value();
	const double fov_height = doublespinbox_fov_height_->value();

	// 图像尺寸
	const double pixel_size = doublespinbox_pixel_size_->value();
	const double image_width = 1000 * ratio * fov_width / pixel_size;
	const double image_height = 1000 * ratio * fov_height / pixel_size;

	// 设置图像尺寸
	doublespinbox_image_width_->blockSignals(true);
	doublespinbox_image_width_->setValue(floor(image_width));
	doublespinbox_image_width_->blockSignals(false);

	doublespinbox_image_height_->blockSignals(true);
	doublespinbox_image_height_->setValue(floor(image_height));
	doublespinbox_image_height_->blockSignals(false);

	PaintSpecklePattern();


}



void RecommendDialog::SlotImageDistanceChanged(double image_distance)
{
	// 放大率
	const double object_distance = doublespinbox_object_distance_->value();
	const double ratio = image_distance / object_distance;

	// 视场大小
	const double fov_width = doublespinbox_fov_width_->value();
	const double fov_height = doublespinbox_fov_height_->value();

	// 图像尺寸
	const double pixel_size = doublespinbox_pixel_size_->value();
	const double image_width = 1000 * ratio * fov_width / pixel_size;
	const double image_height = 1000 * ratio * fov_height / pixel_size;

	// 设置图像尺寸
	doublespinbox_image_width_->blockSignals(true);
	doublespinbox_image_width_->setValue(floor(image_width));
	doublespinbox_image_width_->blockSignals(false);

	doublespinbox_image_height_->blockSignals(true);
	doublespinbox_image_height_->setValue(floor(image_height));
	doublespinbox_image_height_->blockSignals(false);

	PaintSpecklePattern();


}



void RecommendDialog::SlotPixelSizeChanged(double pixel_size)
{
	// 放大率
	const double image_distance = doublespinbox_image_distance_->value();
	const double object_distance = doublespinbox_object_distance_->value();
	const double ratio = image_distance / object_distance;

	// 视场大小
	const double fov_width = doublespinbox_fov_width_->value();
	const double fov_height = doublespinbox_fov_height_->value();

	// 图像尺寸
	const double image_width = 1000 * ratio * fov_width / pixel_size;
	const double image_height = 1000 * ratio * fov_height / pixel_size;

	// 设置图像尺寸
	doublespinbox_image_width_->blockSignals(true);
	doublespinbox_image_width_->setValue(floor(image_width));
	doublespinbox_image_width_->blockSignals(false);

	doublespinbox_image_height_->blockSignals(true);
	doublespinbox_image_height_->setValue(floor(image_height));
	doublespinbox_image_height_->blockSignals(false);

	PaintSpecklePattern();

}



void RecommendDialog::SlotImageWidthChanged(double image_width)
{
	// 放大率
	const double fov_width = doublespinbox_fov_width_->value();
	const double pixel_size = 0.001 * doublespinbox_pixel_size_->value();
	const double ratio = (image_width * pixel_size) / fov_width;

	// 物距
	const double image_distance = doublespinbox_image_distance_->value();
	const double object_distance = image_distance / ratio;

	// 设置物距
	doublespinbox_object_distance_->blockSignals(true);
	doublespinbox_object_distance_->setValue(object_distance);
	doublespinbox_object_distance_->blockSignals(false);

	// 图像高度
	const double fov_height = doublespinbox_fov_height_->value();
	const double image_height = floor(fov_height * ratio / pixel_size);

	doublespinbox_image_height_->blockSignals(true);
	doublespinbox_image_height_->setValue(image_height);
	doublespinbox_image_height_->blockSignals(false);

	PaintSpecklePattern();

}



void RecommendDialog::SlotImageHeightChanged(double image_height)
{
	// 放大率
	const double fov_height = doublespinbox_fov_height_->value();
	const double pixel_size = 0.001 * doublespinbox_pixel_size_->value();
	const double ratio = (image_height * pixel_size) / fov_height;

	// 物距
	const double image_distance = doublespinbox_image_distance_->value();
	const double object_distance = image_distance / ratio;

	// 设置物距
	doublespinbox_object_distance_->blockSignals(true);
	doublespinbox_object_distance_->setValue(object_distance);
	doublespinbox_object_distance_->blockSignals(false);

	// 图像宽度
	const double fov_width = doublespinbox_fov_width_->value();
	const double image_width = floor(fov_width * ratio / pixel_size);

	doublespinbox_image_width_->blockSignals(true);
	doublespinbox_image_width_->setValue(image_width);
	doublespinbox_image_width_->blockSignals(false);

	PaintSpecklePattern();

}



void RecommendDialog::PaintSpecklePattern()
{
	// 直径
	const double diameter_image = 4;

	// 随机度
	const double variation = 0.4;

	// 占空比
	const double density = 0.6;

	// 图像尺寸
	const double width = doublespinbox_fov_width_->value();
	const double height = doublespinbox_fov_height_->value();
	speckle_scene_->setSceneRect(0, 0, width, height);

	// 比例
	const double object_distance = doublespinbox_object_distance_->value();
	const double image_distance = doublespinbox_image_distance_->value();
	const double magnification = image_distance / object_distance;
	const double pixel_size = 0.001 * doublespinbox_pixel_size_->value();
	const double diameter = diameter_image * pixel_size / magnification;

	// 生成散斑
	if (speckle_pattern_ != Q_NULLPTR)
		delete speckle_pattern_;

	speckle_pattern_ = new EllipseSpecklePattern(diameter, density, variation, 0, 0, height, width);

	// 散斑位置
	mat x(speckle_pattern_->x());
	mat y(speckle_pattern_->y());

	// 前景和背景色
	QColor backgroundColor = Qt::white;
	QColor foregroundColor = Qt::black;

		// 绘制
	speckle_scene_->clear();
	speckle_scene_->setBackgroundBrush(QBrush(backgroundColor));

	double radius = 0.5 * diameter;

	const int rows = x.n_rows;
	const int cols = x.n_cols;
	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < cols; ++c)
		{
			EllipseSpeckleItem* ellipse = new EllipseSpeckleItem(radius, radius, 0, foregroundColor);

			speckle_scene_->addItem(ellipse);

			const double center_x = x(r, c);
			const double cneter_y = y(r, c);
			ellipse->setPos(center_x, cneter_y);
			ellipse->show();

		}
	}


}


void RecommendDialog::SlotExport()
{
	// 导出路径
	QFileDialog dialog(this);
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setNameFilter(tr("Images (*.pdf)"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.exec();
	QStringList files = dialog.selectedFiles();
	if (files.isEmpty())
		return;
	QString file_name = files.at(0);

	// 打开文件
	QFile pdf_file(file_name);
	pdf_file.open(QIODevice::WriteOnly);

	QPdfWriter* pdf_writer = new QPdfWriter(&pdf_file);
	const int fov_width = static_cast<int>(doublespinbox_fov_width_->value());
	const int fov_height = static_cast<int>(doublespinbox_fov_height_->value());
	QPageSize page_size(QSize(fov_width, fov_height), QPageSize::Millimeter);
	pdf_writer->setPageSize(page_size);
	const double dpi = 300;
	const double transfer_ratio = dpi / 25.4;
	pdf_writer->setResolution(dpi);
	pdf_writer->setPageMargins(QMarginsF(0, 0, 0, 0));
	pdf_writer->setCreator("Glare by Suyong");
	pdf_writer->setTitle("Optimized speckle pattern");


	// QT 绘制工具
	QPainter* pdf_painter = new QPainter(pdf_writer);
	pdf_painter->setPen(Qt::NoPen);
	pdf_painter->setBrush(QBrush(Qt::black));

	// 散斑位置
	const mat x = transfer_ratio * speckle_pattern_->x();
	const mat y = transfer_ratio * speckle_pattern_->y();
	const double radius = transfer_ratio * speckle_pattern_->r();

	const int rows = x.n_rows;
	const int cols = x.n_cols;
	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < cols; ++c)
		{
			pdf_painter->drawEllipse(QPointF(x(r, c), y(r, c)), radius, radius);
		}

	}

	delete pdf_painter;
	delete pdf_writer;
	pdf_file.close();

}