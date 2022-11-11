#pragma once

#include <QWidget>
#include <QDoubleSpinBox>

class DeformationParameterPanel_Elongation : public QWidget
{
	Q_OBJECT

public:
	DeformationParameterPanel_Elongation(QWidget *parent = Q_NULLPTR);
	~DeformationParameterPanel_Elongation();

	double a() const { return doublespinbox_a_->value(); }
	double x0() const { return doublespinbox_x0_->value(); }

private:
	QDoubleSpinBox* doublespinbox_a_;
	QDoubleSpinBox* doublespinbox_x0_;

	void setupUI();
	void connectSlots();

signals:
	void SignalParameterChanged();

};
