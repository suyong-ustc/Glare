#include "DeformationParameterPanel_Rotation.h"
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>


DeformationParameterPanel_Rotation::DeformationParameterPanel_Rotation(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();

}



DeformationParameterPanel_Rotation::~DeformationParameterPanel_Rotation()
{

}



void DeformationParameterPanel_Rotation::setupUI()
{
	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);

	QLabel* label_parameter = new QLabel(tr("Location : (x' - x0, y' - y0) =  <br/>"
		                                    " a * (cos(theta), -sin(theta); sin(theta), cos(theta)) (x - x0, y - y0)"));
	layout->addWidget(label_parameter);
	layout->addSpacing(10);

	// 变形参数
	QGridLayout* parameter_layout = new QGridLayout;

	QLabel* label_a = new QLabel("a");
	parameter_layout->addWidget(label_a, 0, 0);

	doublespinbox_a_ = new QDoubleSpinBox;
	doublespinbox_a_->setRange(0.01, 20);
	doublespinbox_a_->setValue(1);
	doublespinbox_a_->setSingleStep(0.1);
	parameter_layout->addWidget(doublespinbox_a_, 0, 1);

	QLabel* label_theta = new QLabel("theta");
	parameter_layout->addWidget(label_theta, 1, 0);

	doublespinbox_theta_ = new QDoubleSpinBox;
	doublespinbox_theta_->setRange(0, 360);
	doublespinbox_theta_->setValue(45);
	doublespinbox_theta_->setSuffix(tr(" Degree"));
	parameter_layout->addWidget(doublespinbox_theta_, 1, 1);

	QLabel* label_x0 = new QLabel("x0");
	parameter_layout->addWidget(label_x0, 2, 0);

	doublespinbox_x0_ = new QDoubleSpinBox;
	doublespinbox_x0_->setRange(0, 300);
	doublespinbox_x0_->setValue(150);
	parameter_layout->addWidget(doublespinbox_x0_, 2, 1);

	QLabel* label_y0 = new QLabel("y0");
	parameter_layout->addWidget(label_y0, 3, 0);

	doublespinbox_y0_ = new QDoubleSpinBox;
	doublespinbox_y0_->setRange(0, 300);
	doublespinbox_y0_->setValue(150);
	parameter_layout->addWidget(doublespinbox_y0_, 3, 1);

	layout->addLayout(parameter_layout);
	layout->addStretch();
}



void DeformationParameterPanel_Rotation::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(doublespinbox_theta_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_x0_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_y0_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_a_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

}
