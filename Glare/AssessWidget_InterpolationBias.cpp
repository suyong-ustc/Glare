#include "AssessWidget_InterpolationBias.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include "AssessSpecklePattern.h"
#include "XianType.h"


AssessWidget_InterpolationBias::AssessWidget_InterpolationBias(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	setupGraph();
	connectSlots();
}



AssessWidget_InterpolationBias::~AssessWidget_InterpolationBias()
{

}



void AssessWidget_InterpolationBias::setupUI()
{
	graph_ = new QCustomPlot;

	QLabel* label_interpolation_type = new QLabel(tr("Interpolation Method : "));
	combobox_interpolation_type_ = new QComboBox;
	combobox_interpolation_type_->addItem(tr("Keys Method"));
	combobox_interpolation_type_->addItem(tr("Cubic B-spline"));
	combobox_interpolation_type_->addItem(tr("Quintic B-spline"));
	combobox_interpolation_type_->addItem(tr("Septic B-spline"));
	combobox_interpolation_type_->setCurrentIndex(1);

	QLabel* label_interpolation_bias = new QLabel(tr("Interpolation Bias : "));
	doublespinbox_interpolation_bias_ = new QDoubleSpinBox;
	doublespinbox_interpolation_bias_->setRange(-1, 1);
	doublespinbox_interpolation_bias_->setDecimals(5);
	doublespinbox_interpolation_bias_->setReadOnly(true);
	doublespinbox_interpolation_bias_->setStyleSheet("color:#ba2f7b;");

	QGridLayout* parameter_layout = new QGridLayout;
	parameter_layout->addWidget(label_interpolation_type, 0, 0);
	parameter_layout->addWidget(combobox_interpolation_type_, 0, 1);
	parameter_layout->addWidget(label_interpolation_bias, 1, 0);
	parameter_layout->addWidget(doublespinbox_interpolation_bias_, 1, 1);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(graph_);
	layout->addLayout(parameter_layout);
	setLayout(layout);
}


void AssessWidget_InterpolationBias::setupGraph()
{
	// 清空图像
	graph_->clearGraphs();
	graph_->clearPlottables();

	// 设置坐标轴
	graph_->addGraph();
	graph_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	graph_->axisRect()->setupFullAxesBox(true);
	graph_->xAxis->setLabel("fx");
	graph_->yAxis->setLabel("fy");

	// 设置 QCPColorMap:
	color_map_ = new QCPColorMap(graph_->xAxis, graph_->yAxis);
	color_map_->setGradient(QCPColorGradient::gpPolar);
	color_map_->rescaleDataRange(true);

	// make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
	margin_group_ = new QCPMarginGroup(graph_);
	graph_->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, margin_group_);

	graph_->rescaleAxes();
	graph_->replot();

}



void AssessWidget_InterpolationBias::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(combobox_interpolation_type_, QOverload<int>::of(&QComboBox::currentIndexChanged),
					this, &AssessWidget_InterpolationBias::SlotCalculateInterpolationBias);

}


void AssessWidget_InterpolationBias::SlotNewImageImported(uchar* data, int h, int w)
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

	// 计算功率谱
	power_spectra_ = AssessSpecklePattern::PowerSpectrum(image_);

	// 计算插值偏差
	SlotCalculateInterpolationBias(static_cast<InterpolationAlgorithm>(combobox_interpolation_type_->currentIndex()));

	// 绘制功率谱
	ShowPowerSpectrum();
}



void AssessWidget_InterpolationBias::ShowPowerSpectrum()
{
	// 图像大小
	int nx = power_spectra_.cols;
	int ny = power_spectra_.rows;

	double xmin = -0.5;
	double xmax = 0.5 - 1.0 / nx;
	double ymin = -0.5;
	double ymax = 0.5 - 1.0 / ny;


	// 设置 QCPColorMap:
	color_map_->data()->setSize(nx, ny);
	color_map_->data()->setRange(QCPRange(xmin, xmax), QCPRange(ymin, ymax));

	double x, y, z;
	for (int xIndex = 0; xIndex < nx; ++xIndex)
	{
		for (int yIndex = 0; yIndex < ny; ++yIndex)
		{
			color_map_->data()->cellToCoord(xIndex, yIndex, &x, &y);
			color_map_->data()->setCell(xIndex, yIndex, log(abs(power_spectra_.at<double>(yIndex,xIndex)+1)));
		}
	}

	color_map_->setGradient(QCPColorGradient::gpPolar);
	color_map_->rescaleDataRange(true);
	graph_->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, margin_group_);
	graph_->rescaleAxes();
	graph_->replot();

}



void AssessWidget_InterpolationBias::SlotCalculateInterpolationBias(int interp_id)
{
	// 判断图片是否为空
	if (image_.empty())
		return;

	// 插值偏差核
	InterpolationAlgorithm interp_type = static_cast<InterpolationAlgorithm>(combobox_interpolation_type_->currentIndex());

	cv::Mat bias_kernel, sssig_kernel;
	if (interp_type == KEYS_INTERPOLATION)
		AssessSpecklePattern::Kernel_Keys(power_spectra_.size(), bias_kernel, sssig_kernel);
	else if (interp_type == CUBIC_BSPLINE_INTERPOLATION)
		AssessSpecklePattern::Kernel_BSpline3(power_spectra_.size(), bias_kernel, sssig_kernel);
	else if (interp_type == QUINTIC_BSPLINE_INTERPOLATION)
		AssessSpecklePattern::Kernel_BSpline5(power_spectra_.size(), bias_kernel, sssig_kernel);
	else if (interp_type == SEPTIC_BSPLINE_INTERPOLATION)
		AssessSpecklePattern::Kernel_BSpline7(power_spectra_.size(), bias_kernel, sssig_kernel);

	// 计算插值偏差
	cv::Mat a = bias_kernel.mul(power_spectra_);
	cv::Mat b = sssig_kernel.mul(power_spectra_);
	double ue = 0.5 / CV_PI * AssessSpecklePattern::TrapezoidIntegral(a) / AssessSpecklePattern::TrapezoidIntegral(b);

	doublespinbox_interpolation_bias_->setValue(ue);
}
