#pragma once

#include <QWidget>
#include <QDoubleSpinBox>


class DeformationParameterPanel_2DGaussian : public QWidget
{
	Q_OBJECT

public:
	DeformationParameterPanel_2DGaussian(QWidget* parent = Q_NULLPTR);
	~DeformationParameterPanel_2DGaussian();

	double ax() const { return doublespinbox_ax_->value(); }
	double x0() const { return doublespinbox_x0_->value(); }
	double cx() const { return doublespinbox_cx_->value(); }

	double ay() const { return doublespinbox_ay_->value(); }
	double y0() const { return doublespinbox_y0_->value(); }
	double cy() const { return doublespinbox_cy_->value(); }

private:
	QDoubleSpinBox* doublespinbox_ax_;
	QDoubleSpinBox* doublespinbox_x0_;
	QDoubleSpinBox* doublespinbox_cx_;

	QDoubleSpinBox* doublespinbox_ay_;
	QDoubleSpinBox* doublespinbox_y0_;
	QDoubleSpinBox* doublespinbox_cy_;

	void setupUI();
	void connectSlots();

signals:
	void SignalParameterChanged();
};

