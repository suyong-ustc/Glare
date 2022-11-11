#pragma once

#include <QWidget>
#include <QDoubleSpinBox>

class DeformationParameterPanel_Translation : public QWidget
{
	Q_OBJECT

public:
	DeformationParameterPanel_Translation(QWidget *parent = Q_NULLPTR);
	~DeformationParameterPanel_Translation();

	double u0() const { return doublespinbox_u0_->value(); }

private:
	QDoubleSpinBox* doublespinbox_u0_;

	void setupUI();
	void connectSlots();

signals:
	void SignalParameterChanged();

};
