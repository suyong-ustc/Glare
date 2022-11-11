#include "DeformationParameterPanel_Gaussian.h"
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>

DeformationParameterPanel_Gaussian::DeformationParameterPanel_Gaussian(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}

DeformationParameterPanel_Gaussian::~DeformationParameterPanel_Gaussian()
{

}


void DeformationParameterPanel_Gaussian::setupUI()
{
	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);

	QLabel* label_parameter = new QLabel(tr("Displacement : u = a * exp[-(x-x0)^2/c^2]"));
	layout->addWidget(label_parameter);
	layout->addSpacing(10);

	// 变形参数
	QGridLayout* parameter_layout = new QGridLayout;

	QLabel* label_a = new QLabel("a");
	parameter_layout->addWidget(label_a, 0, 0);

	doublespinbox_a_ = new QDoubleSpinBox;
	doublespinbox_a_->setValue(5);
	parameter_layout->addWidget(doublespinbox_a_, 0, 1);

	QLabel* label_x0 = new QLabel("x0");
	parameter_layout->addWidget(label_x0, 1, 0);

	doublespinbox_x0_ = new QDoubleSpinBox;
	doublespinbox_x0_->setRange(0, 300);
	doublespinbox_x0_->setValue(150);
	parameter_layout->addWidget(doublespinbox_x0_, 1, 1);

	QLabel* label_c = new QLabel("c");
	parameter_layout->addWidget(label_c, 2, 0);

	doublespinbox_c_ = new QDoubleSpinBox;
	doublespinbox_c_->setValue(20);
	parameter_layout->addWidget(doublespinbox_c_, 2, 1);

	layout->addLayout(parameter_layout);
	layout->addStretch();
}


void DeformationParameterPanel_Gaussian::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(doublespinbox_a_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_x0_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_c_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

}
