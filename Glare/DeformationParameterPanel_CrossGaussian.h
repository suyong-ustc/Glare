#pragma once

#include <QWidget>
#include <QDoubleSpinBox>


class DeformationParameterPanel_CrossGaussian : public QWidget
{
	Q_OBJECT

public:
	DeformationParameterPanel_CrossGaussian(QWidget* parent = Q_NULLPTR);
	~DeformationParameterPanel_CrossGaussian();

	double a() const { return doublespinbox_a_->value(); }
	double x0() const { return doublespinbox_x0_->value(); }
	double cx() const { return doublespinbox_cx_->value(); }
	double y0() const { return doublespinbox_y0_->value(); }
	double cy() const { return doublespinbox_cy_->value(); }

private:
	QDoubleSpinBox* doublespinbox_a_;
	QDoubleSpinBox* doublespinbox_x0_;
	QDoubleSpinBox* doublespinbox_cx_;
	QDoubleSpinBox* doublespinbox_y0_;
	QDoubleSpinBox* doublespinbox_cy_;

	void setupUI();
	void connectSlots();

signals:
	void SignalParameterChanged();

};

