#include "DeformationParameterPanel_Power.h"
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>


DeformationParameterPanel_Power::DeformationParameterPanel_Power(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}


DeformationParameterPanel_Power::~DeformationParameterPanel_Power()
{
}


void DeformationParameterPanel_Power::setupUI()
{
	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);

	QLabel* label_parameter = new QLabel(tr("Displacement : u = a (x - x0)^n"));
	layout->addWidget(label_parameter);
	layout->addSpacing(10);

	// 变形参数
	QGridLayout* parameter_layout = new QGridLayout;

	QLabel* label_a = new QLabel("a");
	parameter_layout->addWidget(label_a, 0, 0);

	doublespinbox_a_ = new QDoubleSpinBox;
	doublespinbox_a_->setRange(-1, 1);
	doublespinbox_a_->setDecimals(6);
	doublespinbox_a_->setSingleStep(0.0001);
	doublespinbox_a_->setValue(0.001);
	parameter_layout->addWidget(doublespinbox_a_, 0, 1);

	QLabel* label_x0 = new QLabel("x0");
	parameter_layout->addWidget(label_x0, 1, 0);

	doublespinbox_x0_ = new QDoubleSpinBox;
	doublespinbox_x0_->setRange(-500,500);
	doublespinbox_x0_->setDecimals(2);
	doublespinbox_x0_->setSingleStep(1);
	doublespinbox_x0_->setValue(150);
	parameter_layout->addWidget(doublespinbox_x0_, 1, 1);

	QLabel* label_n = new QLabel("n");
	parameter_layout->addWidget(label_n, 2, 0);

	doublespinbox_n_ = new QDoubleSpinBox;
	doublespinbox_n_->setRange(-3, 3);
	doublespinbox_n_->setDecimals(2);
	doublespinbox_n_->setSingleStep(1);
	doublespinbox_n_->setValue(2);
	parameter_layout->addWidget(doublespinbox_n_, 2, 1);

	layout->addLayout(parameter_layout);
	layout->addStretch();
}


void DeformationParameterPanel_Power::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(doublespinbox_a_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_x0_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_n_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });
}