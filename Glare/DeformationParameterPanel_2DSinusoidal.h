#pragma once

#include <QWidget>
#include <QDoubleSpinBox>

class DeformationParameterPanel_2DSinusoidal : public QWidget
{
	Q_OBJECT

public:
	DeformationParameterPanel_2DSinusoidal(QWidget* parent = Q_NULLPTR);
	~DeformationParameterPanel_2DSinusoidal();

	double ax() const { return doublespinbox_ax_->value(); }
	double Tx() const { return doublespinbox_Tx_->value(); }
	double bx() const { return doublespinbox_bx_->value(); }

	double ay() const { return doublespinbox_ay_->value(); }
	double Ty() const { return doublespinbox_Ty_->value(); }
	double by() const { return doublespinbox_by_->value(); }

private:
	QDoubleSpinBox* doublespinbox_ax_;
	QDoubleSpinBox* doublespinbox_Tx_;
	QDoubleSpinBox* doublespinbox_bx_;

	QDoubleSpinBox* doublespinbox_ay_;
	QDoubleSpinBox* doublespinbox_Ty_;
	QDoubleSpinBox* doublespinbox_by_;

	void setupUI();
	void connectSlots();

signals:
	void SignalParameterChanged();


};

