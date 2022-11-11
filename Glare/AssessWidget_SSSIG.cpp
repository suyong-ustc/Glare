#include "AssessWidget_SSSIG.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QSize>
#include <QVBoxLayout>
#include "AssessSpecklePattern.h"
using namespace arma;


AssessWidget_SSSIG::AssessWidget_SSSIG(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	setupGraph();
	connectSlots();
}



AssessWidget_SSSIG::~AssessWidget_SSSIG()
{

}



void AssessWidget_SSSIG::setupUI()
{
	QLabel* label_displacement_direction = new QLabel(tr("Displacement Direction : "));
	combobox_displacement_direction_ = new QComboBox;
	combobox_displacement_direction_->addItem(tr("U"));
	combobox_displacement_direction_->addItem(tr("V"));
	combobox_displacement_direction_->setCurrentIndex(0);

	QLabel* label_subset_size = new QLabel(tr("Subset Size : "));
	spinbox_subset_size_ = new QSpinBox;
	spinbox_subset_size_->setRange(1, 99);
	spinbox_subset_size_->setValue(21);
	spinbox_subset_size_->setSingleStep(2);
	spinbox_subset_size_->setSuffix(tr(" Pixel"));

	QLabel* label_noise_level = new QLabel(tr("Noise Level : "));
	doublespinbox_noise_level_ = new QDoubleSpinBox;
	doublespinbox_noise_level_->setRange(0.01, 10);
	doublespinbox_noise_level_->setValue(3);
	doublespinbox_noise_level_->setSingleStep(0.1);
	doublespinbox_noise_level_->setSuffix(tr(" Grayscale"));

	QLabel* label_shape_function_order = new QLabel(tr("Shape Function : "));
	combobox_shape_function_order_ = new QComboBox;
	combobox_shape_function_order_->addItem(tr("Zero-order Shape Function"));
	combobox_shape_function_order_->addItem(tr("First-order Shape Function"));
	combobox_shape_function_order_->addItem(tr("Second-order Shape Function"));
	combobox_shape_function_order_->setCurrentIndex(1);


	QLabel* label_mig_ = new QLabel(tr("Mean Image Gradient (MIG) : "));
	doublespinbox_mig_ = new QDoubleSpinBox;
	doublespinbox_mig_->setReadOnly(true);
	doublespinbox_mig_->setStyleSheet("color:blue;");


	QGridLayout* parameter_layout = new QGridLayout;
	parameter_layout->addWidget(label_displacement_direction, 0, 0);
	parameter_layout->addWidget(combobox_displacement_direction_, 0, 1);
	parameter_layout->addWidget(label_subset_size, 1, 0);
	parameter_layout->addWidget(spinbox_subset_size_, 1, 1);
	parameter_layout->addWidget(label_noise_level, 2, 0);
	parameter_layout->addWidget(doublespinbox_noise_level_, 2, 1);
	parameter_layout->addWidget(label_shape_function_order, 3, 0);
	parameter_layout->addWidget(combobox_shape_function_order_, 3, 1);
	parameter_layout->addWidget(label_mig_, 4, 0);
	parameter_layout->addWidget(doublespinbox_mig_, 4, 1);

	graph_ = new QCustomPlot;

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(graph_);
	layout->addLayout(parameter_layout);
	setLayout(layout);

}



void AssessWidget_SSSIG::setupGraph()
{
	// 清空图片
	graph_->clearGraphs();
	graph_->clearPlottables();

	// 设置坐标轴
	graph_->addGraph();
	graph_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	graph_->axisRect()->setupFullAxesBox(true);
	graph_->xAxis->setLabel("x");
	graph_->yAxis->setLabel("y");
	graph_->yAxis->setRangeReversed(true);	// y 轴方向与图片保持一致

	// 添加 QCPColorMap:
	color_map_ = new QCPColorMap(graph_->xAxis, graph_->yAxis);
	color_map_->setGradient(QCPColorGradient::gpPolar);

	// 添加 Colorbar
	color_scale_ = new QCPColorScale(graph_);
	graph_->plotLayout()->addElement(0, 1, color_scale_);
	color_scale_->setType(QCPAxis::atRight);
	color_scale_->axis()->setLabel(tr("Random Error"));
	color_map_->setColorScale(color_scale_);

	// 边界
	margin_group_ = new QCPMarginGroup(graph_);
	graph_->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, margin_group_);
	color_scale_->setMarginGroup(QCP::msBottom | QCP::msTop, margin_group_);

	// rescale the key (x) and value (y) axes so the whole color map is visible:
	graph_->rescaleAxes();
	graph_->replot();

}



void AssessWidget_SSSIG::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(spinbox_subset_size_, QOverload<int>::of(&QSpinBox::valueChanged),
		this, [=]() { SlotUpdate(); });

	is_ok = connect(doublespinbox_noise_level_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { SlotUpdate(); });

	is_ok = connect(combobox_shape_function_order_, QOverload<int>::of(&QComboBox::currentIndexChanged),
		this, [=]() { SlotUpdate(); });

	is_ok = connect(combobox_displacement_direction_, QOverload<int>::of(&QComboBox::currentIndexChanged),
		this, [=]() { SlotUpdate(); });

}



void AssessWidget_SSSIG::SlotNewImageImported(uchar* data, int h, int w)
{
	// 转换格式
	const mat image = AssessSpecklePattern::Uchar2ArmaMat(data, h, w);

	// 灰度梯度
	mat gx, gy;
	AssessSpecklePattern::CalculateImageGradients(image, gx, gy);

	// 平均灰度梯度平方
	const mat gx2 = gx % gx;
	const mat gy2 = gy % gy;

	// 获得 MIG
	const double mig = accu(sqrt(gx2 + gy2)) / ((h - 2)*(w - 2));
	doublespinbox_mig_->setValue(mig);

	// 计算梯度的积分图像
	grad_integral_image_x_ = AssessSpecklePattern::CalculateIntegralImage(gx2);
	grad_integral_image_y_ = AssessSpecklePattern::CalculateIntegralImage(gy2);

	// 计算灰度梯度平方和并显示
	SlotUpdate();
}



void AssessWidget_SSSIG::ShowSSSIG(const mat& random_error, const int& xmin, const int& xmax, const int& ymin, const int& ymax)
{
	// 设置 QCPColorMap:
	const int nx = xmax - xmin + 1;
	const int ny = ymax - ymin + 1;
	color_map_->data()->setSize(nx, ny);
	color_map_->data()->setRange(QCPRange(xmin, xmax), QCPRange(ymin, ymax));

	double x, y, z;
	for (int xIndex = 0; xIndex < nx; ++xIndex)
	{
		for (int yIndex = 0; yIndex < ny; ++yIndex)
		{
			color_map_->data()->cellToCoord(xIndex, yIndex, &x, &y);
			color_map_->data()->setCell(xIndex, yIndex, random_error(ymin + yIndex, xmin + xIndex));
		}
	}

	// 添加 Colorbar
	color_map_->setColorScale(color_scale_);
	color_map_->setGradient(QCPColorGradient::gpPolar);
	color_map_->rescaleDataRange(true);
	graph_->rescaleAxes();
	graph_->replot();

}



void AssessWidget_SSSIG::SlotUpdate()
{
	// 判断图片是否为空
	if (grad_integral_image_x_.empty() || grad_integral_image_y_.empty())
		return;

	// 图像尺寸
	const int h = grad_integral_image_x_.n_rows;
	const int w = grad_integral_image_x_.n_cols;

	// 子区大小
	const int half_subset_size = (spinbox_subset_size_->value() - 1) / 2;

	// 需要计算灰度梯度平方和的区域
	const int xmin = half_subset_size + 1;
	const int xmax = w - half_subset_size - 1;
	const int ymin = half_subset_size + 1;
	const int ymax = h - half_subset_size - 1;

	// 需要计算灰度梯度平方和的方向
	mat grad_integral_image;
	if (combobox_displacement_direction_->currentIndex() == 0)
		grad_integral_image = grad_integral_image_x_;
	else
		grad_integral_image = grad_integral_image_y_;

	// 计算灰度梯度平方和
	mat sssig = ones(h, w);
	for (int r = ymin; r <= ymax; ++r)
	{
		for (int c = xmin; c <= xmax; ++c)
		{
			const int subset_row_min = r - half_subset_size;
			const int subset_row_max = r + half_subset_size;
			const int subset_col_min = c - half_subset_size;
			const int subset_col_max = c + half_subset_size;

			sssig(r, c) =
				grad_integral_image(subset_row_max, subset_col_max)
				- grad_integral_image(subset_row_min - 1, subset_col_max)
				- grad_integral_image(subset_row_max, subset_col_min - 1)
				+ grad_integral_image(subset_row_min - 1, subset_col_min - 1);

		}

	}

	// 噪声标准差
	const double noise_level = doublespinbox_noise_level_->value();

	// 计算随机误差 (二阶形函数误差是零阶和一阶的 sqrt(3.5) 倍)
	mat random_error;

	if (combobox_shape_function_order_->currentIndex() != 2)
		random_error = sqrt(2.0) * noise_level / sqrt(sssig);
	else
		random_error = sqrt(7.0) * noise_level / sqrt(sssig);


	// 显示图片
	ShowSSSIG(random_error, xmin, xmax, ymin, ymax);

}