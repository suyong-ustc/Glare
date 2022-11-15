#include "DeformationParameterPanel_CrossSinusoidal.h"
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>


DeformationParameterPanel_CrossSinusoidal::DeformationParameterPanel_CrossSinusoidal(QWidget* parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}

DeformationParameterPanel_CrossSinusoidal::~DeformationParameterPanel_CrossSinusoidal()
{

}


void DeformationParameterPanel_CrossSinusoidal::setupUI()
{
	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);

	QLabel* label_parameter = new QLabel(tr("Displacement : u = a sin(2*pi*x/Tx + bx) sin(2*pi*y/Ty + by)"));
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

	QLabel* label_Tx = new QLabel("Tx");
	parameter_layout->addWidget(label_Tx, 1, 0);

	doublespinbox_Tx_ = new QDoubleSpinBox;
	doublespinbox_Tx_->setRange(10, 500);
	doublespinbox_Tx_->setValue(100);
	parameter_layout->addWidget(doublespinbox_Tx_, 1, 1);

	QLabel* label_bx = new QLabel("bx");
	parameter_layout->addWidget(label_bx, 2, 0);

	doublespinbox_bx_ = new QDoubleSpinBox;
	doublespinbox_bx_->setRange(-50, 50);
	doublespinbox_bx_->setValue(0);
	parameter_layout->addWidget(doublespinbox_bx_, 2, 1);

	QLabel* label_Ty = new QLabel("Ty");
	parameter_layout->addWidget(label_Ty, 3, 0);

	doublespinbox_Ty_ = new QDoubleSpinBox;
	doublespinbox_Ty_->setRange(10, 500);
	doublespinbox_Ty_->setValue(100);
	parameter_layout->addWidget(doublespinbox_Ty_, 3, 1);

	QLabel* label_by = new QLabel("by");
	parameter_layout->addWidget(label_by, 4, 0);

	doublespinbox_by_ = new QDoubleSpinBox;
	doublespinbox_by_->setRange(-50, 50);
	doublespinbox_by_->setValue(0);
	parameter_layout->addWidget(doublespinbox_by_, 4, 1);

	layout->addLayout(parameter_layout);
	layout->addStretch();
}


void DeformationParameterPanel_CrossSinusoidal::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(doublespinbox_a_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_Tx_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_bx_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_Ty_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_by_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

}