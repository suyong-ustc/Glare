#include "DeformationParameterPanel_Sinusoidal.h"
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>


DeformationParameterPanel_Sinusoidal::DeformationParameterPanel_Sinusoidal(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}

DeformationParameterPanel_Sinusoidal::~DeformationParameterPanel_Sinusoidal()
{

}


void DeformationParameterPanel_Sinusoidal::setupUI()
{
	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);

	QLabel* label_parameter = new QLabel(tr("Displacement : u = a sin(2*pi*x/T + b)"));
	layout->addWidget(label_parameter);
	layout->addSpacing(10);

	// 变形参数
	QGridLayout* parameter_layout = new QGridLayout;

	QLabel* label_a = new QLabel("a");
	parameter_layout->addWidget(label_a, 0, 0);

	doublespinbox_a_ = new QDoubleSpinBox;
	doublespinbox_a_->setRange(-10, 10);
	doublespinbox_a_->setValue(5);
	parameter_layout->addWidget(doublespinbox_a_, 0, 1);

	QLabel* label_T = new QLabel("T");
	parameter_layout->addWidget(label_T, 1, 0);

	doublespinbox_T_ = new QDoubleSpinBox;
	doublespinbox_T_->setRange(0, 500);
	doublespinbox_T_->setValue(100);
	parameter_layout->addWidget(doublespinbox_T_, 1, 1);

	QLabel* label_b = new QLabel("b");
	parameter_layout->addWidget(label_b, 2, 0);

	doublespinbox_b_ = new QDoubleSpinBox;
	doublespinbox_b_->setRange(-50, 50);
	doublespinbox_b_->setValue(0);
	parameter_layout->addWidget(doublespinbox_b_, 2, 1);

	layout->addLayout(parameter_layout);
	layout->addStretch();
}


void DeformationParameterPanel_Sinusoidal::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(doublespinbox_a_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_T_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_b_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

}