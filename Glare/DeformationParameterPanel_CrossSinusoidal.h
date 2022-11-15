#pragma once

#include <QWidget>
#include <QDoubleSpinBox>


class DeformationParameterPanel_CrossSinusoidal : public QWidget
{
	Q_OBJECT

public:
	DeformationParameterPanel_CrossSinusoidal(QWidget* parent = Q_NULLPTR);
	~DeformationParameterPanel_CrossSinusoidal();

	double a() const { return doublespinbox_a_->value(); }
	double Tx() const { return doublespinbox_Tx_->value(); }
	double bx() const { return doublespinbox_bx_->value(); }
	double Ty() const { return doublespinbox_Ty_->value(); }
	double by() const { return doublespinbox_by_->value(); }


private:
	QDoubleSpinBox* doublespinbox_a_;
	QDoubleSpinBox* doublespinbox_Tx_;
	QDoubleSpinBox* doublespinbox_bx_;
	QDoubleSpinBox* doublespinbox_Ty_;
	QDoubleSpinBox* doublespinbox_by_;

	void setupUI();
	void connectSlots();

signals:
	void SignalParameterChanged();
};

