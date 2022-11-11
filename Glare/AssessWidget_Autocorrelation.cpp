#include "AssessWidget_Autocorrelation.h"
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QVBoxLayout>
#include "AssessSpecklePattern.h"
using namespace arma;


AssessWidget_Autocorrelation::AssessWidget_Autocorrelation(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}



AssessWidget_Autocorrelation::~AssessWidget_Autocorrelation()
{


}



void AssessWidget_Autocorrelation::setupUI()
{
	graph_ = new QCustomPlot;

	QLabel* label_subset_size = new QLabel(tr("Subset Size :"));
	spinbox_subset_size_ = new QSpinBox();
	spinbox_subset_size_->setValue(49);
	spinbox_subset_size_->setSingleStep(2);
	spinbox_subset_size_->setRange(3, 99);
	spinbox_subset_size_->setSuffix(tr(" Pixel"));

	QLabel* label_maximal_translation = new QLabel(tr("Maximal Translation :"));
	spinbox_maximal_translation_ = new QSpinBox();
	spinbox_maximal_translation_->setValue(25);
	spinbox_maximal_translation_->setRange(9, 99);
	spinbox_maximal_translation_->setSuffix(tr(" Pixel"));

	QLabel* label_speckle_size = new QLabel(tr("Speckle Size :"));
	doublespinbox_speckle_size_ = new QDoubleSpinBox();
	doublespinbox_speckle_size_->setValue(0);
	doublespinbox_speckle_size_->setSuffix(tr(" Pixel"));
	doublespinbox_speckle_size_->setReadOnly(true);
	doublespinbox_speckle_size_->setStyleSheet("color:#2376b7;");
	doublespinbox_speckle_size_->setToolTip(tr("the recommended diameter is 3-5 pixels"));

	QGridLayout* parameter_layout = new QGridLayout;
	parameter_layout->addWidget(label_subset_size, 0, 0);
	parameter_layout->addWidget(spinbox_subset_size_, 0, 1);
	parameter_layout->addWidget(label_maximal_translation, 1, 0);
	parameter_layout->addWidget(spinbox_maximal_translation_, 1, 1);
	parameter_layout->addWidget(label_speckle_size, 2, 0);
	parameter_layout->addWidget(doublespinbox_speckle_size_, 2, 1);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(graph_);
	layout->addLayout(parameter_layout);
	setLayout(layout);

}



void AssessWidget_Autocorrelation::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(spinbox_subset_size_, QOverload<int>::of(&QSpinBox::valueChanged),
		this, [=]() { SlotRenderAutoCorrelation(); });

	is_ok = connect(spinbox_maximal_translation_, QOverload<int>::of(&QSpinBox::valueChanged),
		this, [=]() { SlotRenderAutoCorrelation(); });


}



void AssessWidget_Autocorrelation::SlotNewImageImported(uchar* data, const int& h, const int& w)
{
	// 导入新的图像
	image_ = zeros<mat>(h, w);

	for (int r = 0; r < h; ++r)
	{
		uchar* data_row = data + r * w;
		for (int c = 0; c < w; ++c)
		{
			double a = data_row[c];
			image_(r, c) = data_row[c];
		}
	}

	// 根据图像大小设置参数
	int minimum = qMin(h, w);
	if (minimum < 100)
	{
		spinbox_subset_size_->setValue(minimum / 2);
		spinbox_maximal_translation_->setValue(minimum / 4);
	}
	else
	{
		spinbox_subset_size_->setValue(49);
		spinbox_maximal_translation_->setValue(25);
	}


	// 计算自相关曲线
	SlotRenderAutoCorrelation();

}



void AssessWidget_Autocorrelation::ShowAutoCorrelationCurves()
{
	// 显示自相关曲线
	const int maximal_translation = spinbox_maximal_translation_->value();
	const int n = 2 * maximal_translation + 1;

	// 清空图像
	graph_->clearGraphs();
	graph_->clearPlottables();

	// 添加图像
	graph_->addGraph();
	graph_->graph(0)->setPen(QPen(Qt::blue));
	graph_->graph(0)->setName("x");
	graph_->addGraph();
	graph_->graph(1)->setPen(QPen(Qt::red));
	graph_->graph(1)->setName("y");

	// 图例
	graph_->legend->setVisible(true);

	// 数据
	QVector<double> x(n), y0(n), y1(n);
	for (int i = 0; i < n; ++i)
	{
		x[i] = i - maximal_translation;
		y0[i] = zncc_x_(i);
		y1[i] = zncc_y_(i);
	}

	graph_->xAxis2->setVisible(true);
	graph_->xAxis2->setTickLabels(false);
	graph_->yAxis2->setVisible(true);
	graph_->yAxis2->setTickLabels(false);

	// make left and bottom axes always transfer their ranges to right and top axes:
	connect(graph_->xAxis, SIGNAL(rangeChanged(QCPRange)), graph_->xAxis2, SLOT(setRange(QCPRange)));
	connect(graph_->yAxis, SIGNAL(rangeChanged(QCPRange)), graph_->yAxis2, SLOT(setRange(QCPRange)));

	// pass data points to graphs:
	graph_->graph(0)->setData(x, y0);
	graph_->graph(1)->setData(x, y1);

	// let the ranges scale themselves so graph 0 fits perfectly in the visible area:
	graph_->graph(0)->rescaleAxes();

	// same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
	graph_->graph(1)->rescaleAxes(true);

	// Note: we could have also just called customPlot->rescaleAxes(); instead
	// Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
	graph_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

	graph_->replot();

}



void AssessWidget_Autocorrelation::SlotRenderAutoCorrelation()
{
	// 判断图片是否为空
	if (image_.empty())
		return;

	// 计算自相关曲线
	AssessSpecklePattern::CalculateAutoCorrelation(image_, spinbox_subset_size_->value(), spinbox_maximal_translation_->value(), 
		zncc_x_, zncc_y_);

	// 计算散斑颗粒尺寸
	const double diameter = AssessSpecklePattern::CalculateSpeckleSize(zncc_x_, zncc_y_);
	doublespinbox_speckle_size_->setValue(diameter);

	// 显示自相关曲线
	ShowAutoCorrelationCurves();
}

