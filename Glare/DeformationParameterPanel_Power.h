#pragma once

#include <QWidget>
#include <QDoubleSpinBox>

class DeformationParameterPanel_Power : public QWidget
{
	Q_OBJECT

public:
	DeformationParameterPanel_Power(QWidget *parent = Q_NULLPTR);
	~DeformationParameterPanel_Power();

	double a() const { return doublespinbox_a_->value(); }
	double x0() const { return doublespinbox_x0_->value(); }
	double n() const { return doublespinbox_n_->value(); }

private:
	QDoubleSpinBox* doublespinbox_a_;
	QDoubleSpinBox* doublespinbox_x0_;
	QDoubleSpinBox* doublespinbox_n_;

	void setupUI();
	void connectSlots();

signals:
	void SignalParameterChanged();

};
