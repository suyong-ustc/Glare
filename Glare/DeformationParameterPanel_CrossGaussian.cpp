#include "DeformationParameterPanel_CrossGaussian.h"
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>

DeformationParameterPanel_CrossGaussian::DeformationParameterPanel_CrossGaussian(QWidget* parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}

DeformationParameterPanel_CrossGaussian::~DeformationParameterPanel_CrossGaussian()
{

}


void DeformationParameterPanel_CrossGaussian::setupUI()
{
	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);

	QLabel* label_parameter = new QLabel(tr("Displacement : u = a * exp[-(x-x0)^2/cx^2 * exp[-(y-y0)^2/cy^2]"));
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

	QLabel* label_cx = new QLabel("cx");
	parameter_layout->addWidget(label_cx, 2, 0);

	doublespinbox_cx_ = new QDoubleSpinBox;
	doublespinbox_cx_->setValue(20);
	parameter_layout->addWidget(doublespinbox_cx_, 2, 1);

	QLabel* label_y0 = new QLabel("y0");
	parameter_layout->addWidget(label_y0, 3, 0);

	doublespinbox_y0_ = new QDoubleSpinBox;
	doublespinbox_y0_->setRange(0, 300);
	doublespinbox_y0_->setValue(150);
	parameter_layout->addWidget(doublespinbox_y0_, 3, 1);

	QLabel* label_cy = new QLabel("cy");
	parameter_layout->addWidget(label_cy, 4, 0);

	doublespinbox_cy_ = new QDoubleSpinBox;
	doublespinbox_cy_->setValue(20);
	parameter_layout->addWidget(doublespinbox_cy_, 4, 1);

	layout->addLayout(parameter_layout);
	layout->addStretch();
}


void DeformationParameterPanel_CrossGaussian::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(doublespinbox_a_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_x0_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_cx_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_y0_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_cy_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

}
