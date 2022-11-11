#pragma once

#include <QWidget>
#include <QDoubleSpinBox>

class DeformationParameterPanel_Sinusoidal : public QWidget
{
	Q_OBJECT

public:
	DeformationParameterPanel_Sinusoidal(QWidget *parent = Q_NULLPTR);
	~DeformationParameterPanel_Sinusoidal();

	double a() const { return doublespinbox_a_->value(); }
	double T() const { return doublespinbox_T_->value(); }
	double b() const { return doublespinbox_b_->value(); }


private:
	QDoubleSpinBox* doublespinbox_a_;
	QDoubleSpinBox* doublespinbox_T_;
	QDoubleSpinBox* doublespinbox_b_;

	void setupUI();
	void connectSlots();

signals:
	void SignalParameterChanged();
};
