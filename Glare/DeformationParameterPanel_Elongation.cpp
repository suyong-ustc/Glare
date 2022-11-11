#include "DeformationParameterPanel_Elongation.h"
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>


DeformationParameterPanel_Elongation::DeformationParameterPanel_Elongation(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}


DeformationParameterPanel_Elongation::~DeformationParameterPanel_Elongation()
{
}


void DeformationParameterPanel_Elongation::setupUI()
{
	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);

	QLabel* label_parameter = new QLabel(tr("Displacement : u = a (x - x0)"));
	layout->addWidget(label_parameter);
	layout->addSpacing(10);

	// 变形参数
	QGridLayout* parameter_layout = new QGridLayout;

	QLabel* label_a = new QLabel("a");
	parameter_layout->addWidget(label_a, 0, 0);

	doublespinbox_a_ = new QDoubleSpinBox;
	doublespinbox_a_->setRange(-1, 3);
	doublespinbox_a_->setDecimals(6);
	doublespinbox_a_->setSingleStep(0.05);
	doublespinbox_a_->setValue(-0.5);
	parameter_layout->addWidget(doublespinbox_a_, 0, 1);

	QLabel* label_x0 = new QLabel("x0");
	parameter_layout->addWidget(label_x0, 1, 0);

	doublespinbox_x0_ = new QDoubleSpinBox;
	doublespinbox_x0_->setRange(-500,500);
	doublespinbox_x0_->setDecimals(2);
	doublespinbox_x0_->setSingleStep(1);
	doublespinbox_x0_->setValue(150);
	parameter_layout->addWidget(doublespinbox_x0_, 1, 1);

	layout->addLayout(parameter_layout);
	layout->addStretch();
}


void DeformationParameterPanel_Elongation::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(doublespinbox_a_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_x0_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

}