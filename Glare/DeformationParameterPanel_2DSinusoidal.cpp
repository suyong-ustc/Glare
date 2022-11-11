#include "DeformationParameterPanel_2DSinusoidal.h"
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>


DeformationParameterPanel_2DSinusoidal::DeformationParameterPanel_2DSinusoidal(QWidget* parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}


DeformationParameterPanel_2DSinusoidal::~DeformationParameterPanel_2DSinusoidal()
{

}


void DeformationParameterPanel_2DSinusoidal::setupUI()
{
	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);

	QLabel* label_parameter = new QLabel(tr("Displacement : u = ax sin(2*pi*x/Tx + bx); v = ay sin(2*pi*y/Ty + by)"));
	layout->addWidget(label_parameter);
	layout->addSpacing(10);

	// 变形参数
	QGridLayout* parameter_layout = new QGridLayout;

	QLabel* label_ax = new QLabel("ax");
	parameter_layout->addWidget(label_ax, 0, 0);

	doublespinbox_ax_ = new QDoubleSpinBox;
	doublespinbox_ax_->setRange(-10, 10);
	doublespinbox_ax_->setValue(5);
	parameter_layout->addWidget(doublespinbox_ax_, 0, 1);

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

	QLabel* label_ay = new QLabel("ay");
	parameter_layout->addWidget(label_ay, 3, 0);

	doublespinbox_ay_ = new QDoubleSpinBox;
	doublespinbox_ay_->setRange(-10, 10);
	doublespinbox_ay_->setValue(5);
	parameter_layout->addWidget(doublespinbox_ay_, 3, 1);

	QLabel* label_Ty = new QLabel("Ty");
	parameter_layout->addWidget(label_Ty, 4, 0);

	doublespinbox_Ty_ = new QDoubleSpinBox;
	doublespinbox_Ty_->setRange(10, 500);
	doublespinbox_Ty_->setValue(100);
	parameter_layout->addWidget(doublespinbox_Ty_, 4, 1);

	QLabel* label_by = new QLabel("by");
	parameter_layout->addWidget(label_by, 5, 0);

	doublespinbox_by_ = new QDoubleSpinBox;
	doublespinbox_by_->setRange(-50, 50);
	doublespinbox_by_->setValue(0);
	parameter_layout->addWidget(doublespinbox_by_, 5, 1);

	layout->addLayout(parameter_layout);
	layout->addStretch();
}


void DeformationParameterPanel_2DSinusoidal::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(doublespinbox_ax_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_Tx_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_bx_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_ay_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_Ty_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

	is_ok = connect(doublespinbox_by_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalParameterChanged(); });

}