#pragma once

#include <QWidget>
#include <QDoubleSpinBox>


class DeformationParameterPanel_PLC : public QWidget
{
	Q_OBJECT

public:
	DeformationParameterPanel_PLC(QWidget *parent = Q_NULLPTR);
	~DeformationParameterPanel_PLC();

	double a() const { return doublespinbox_a_->value(); }
	double x0() const { return doublespinbox_x0_->value(); }
	double c() const { return doublespinbox_c_->value(); }

private:
	QDoubleSpinBox* doublespinbox_a_;
	QDoubleSpinBox* doublespinbox_x0_;
	QDoubleSpinBox* doublespinbox_c_;

	void setupUI();
	void connectSlots();

signals:
	void SignalParameterChanged();

};
