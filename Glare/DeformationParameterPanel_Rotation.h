#pragma once

#include <QWidget>
#include <QDoubleSpinBox>


class DeformationParameterPanel_Rotation : public QWidget
{
	Q_OBJECT

public:
	DeformationParameterPanel_Rotation(QWidget *parent = Q_NULLPTR);
	~DeformationParameterPanel_Rotation();

	double a() const { return doublespinbox_a_->value(); }
	double theta() const { return doublespinbox_theta_->value(); }
	double x0() const { return doublespinbox_x0_->value(); }
	double y0() const { return doublespinbox_y0_->value(); }

private:

	QDoubleSpinBox* doublespinbox_a_;
	QDoubleSpinBox* doublespinbox_theta_;
	QDoubleSpinBox* doublespinbox_x0_;
	QDoubleSpinBox* doublespinbox_y0_;

	void setupUI();
	void connectSlots();

signals:
	void SignalParameterChanged();

};
