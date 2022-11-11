#include "DeformationParameterPanel_2DGaussian.h"
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>


DeformationParameterPanel_2DGaussian::DeformationParameterPanel_2DGaussian(QWidget* parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}


DeformationParameterPanel_2DGaussian::~DeformationParameterPanel_2DGaussian()
{

}


void DeformationParameterPanel_2DGaussian::setupUI()
{
	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);

	QLabel* label_parameter = new QLabel(tr("Displacement : u = ax * exp[-(x-x0)^2/cx^2]; v = ay * exp[-(y-y0)^2/cy^2];"));
	layout->addWidget(label_parameter);
	layout->addSpacing(10);

	// 变形参数
	QGridLayout* parameter_layout = new QGridLayout;

	QLabel* label_ax = new QLabel("ax");
	parameter_layout->addWidget(label_ax, 0, 0);

	doublespinbox_ax_ = new QDoubleSpinBox;
	doublespinbox_ax_->setValue(5);
	parameter_layout->addWidget(doublespinbox_ax_, 0, 1);

	QLabel* label_x0 = new QLabel("x0");
	parameter_layout->addWidget(label_x0, 1, 0);

	doublespinbox_x0_ = new QDoubleSpinBox;
	doublespinbox_x0_->setRange(0, 300);
	doublespinbox_x0_->setValue(150);
	parameter_layout->addWidget(doublespinbox_x0_, 1, 1);

	QLabel* label_cx = new QLabel("cx");
	parameter_layout->addWidget(label_cx, 2, 0);

	doublespinbox_cx_ = new QDoubleSpinBox;
	doublespinbox_cx_->setValue(20);
	parameter_layout->addWidget(doublespinbox_cx_, 2, 1);

	QLabel* label_ay = new QLabel("ay");
	parameter_layout->addWidget(label_ay, 3, 0);

	doublespinbox_ay_ = new QDoubleSpinBox;
	doublespinbox_ay_->setValue(5);
	parameter_layout->addWidget(doublespinbox_ay_, 3, 1);

	QLabel* label_y0 = new QLabel("y0");
	parameter_layout->addWidget(label_y0, 4, 0);

	doublespinbox_y0_ = new QDoubleSpinBox;
	doublespinbox_y0_->setRange(0, 300);
	doublespinbox_y0_->setValue(150);
	parameter_layout->addWidget(doublespinbox_y0_, 4, 1);

	QLabel* label_cy = new QLabel("cy");
	parameter_layout->addWidget(label_cy, 5, 0);

	doublespinbox_cy_ = new QDoubleSpinBox;
	doublespinbox_cy_->setValue(20);
	parameter_layout->addWidget(doublespinbox_cy_, 5, 1);


	layout->addLayout(parameter_layout);
	layout->addStretch();
}


void DeformationParameterPanel_2DGaussian::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(doublespinbox_ax_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_x0_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_cx_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_ay_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_y0_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_cy_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

}