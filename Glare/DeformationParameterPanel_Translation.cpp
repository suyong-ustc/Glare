#include "DeformationParameterPanel_Translation.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>


DeformationParameterPanel_Translation::DeformationParameterPanel_Translation(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}



DeformationParameterPanel_Translation::~DeformationParameterPanel_Translation()
{

}



void DeformationParameterPanel_Translation::setupUI()
{
	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);

	// 变形参数
	QLabel* label_parameter = new QLabel(tr("Displacement : u = u0"));
	layout->addWidget(label_parameter);
	layout->addSpacing(10);

	// 变形参数
	QGridLayout* parameter_layout = new QGridLayout;
	QLabel* label_u0 = new QLabel("u0");
	parameter_layout->addWidget(label_u0, 0, 0);
	doublespinbox_u0_ = new QDoubleSpinBox;
	doublespinbox_u0_->setValue(50);
	doublespinbox_u0_->setRange(-150, 150);
	doublespinbox_u0_->setSingleStep(0.5);
	doublespinbox_u0_->setDecimals(2);
	doublespinbox_u0_->setSuffix(tr(" Pixel"));
	parameter_layout->addWidget(doublespinbox_u0_, 0, 1);

	layout->addLayout(parameter_layout);
	layout->addStretch();

}


void DeformationParameterPanel_Translation::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(doublespinbox_u0_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

}