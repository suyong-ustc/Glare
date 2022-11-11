#pragma once

#include <QDialog>
#include <QImage>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QTabWidget>
#include "SpeckleScene.h"
#include "SpeckleView.h"
#include "AssessWidget_Autocorrelation.h"
#include "AssessWidget_Coverage.h"
#include "AssessWidget_InterpolationBias.h"
#include "AssessWidget_SSSIG.h"
#include "XianType.h"



class AssessDialog : public QDialog
{
	Q_OBJECT

public:
	AssessDialog(QWidget *parent = Q_NULLPTR);
	~AssessDialog();

private:
	SpeckleScene* speckle_scene_;
	SpeckleView* speckle_view_;

	QPushButton* pushbutton_import_;

	AssessWidget_Autocorrelation* widget_autocorrelation_;
	AssessWidget_Coverage* widget_coverage_;
	AssessWidget_SSSIG* widget_sssig_;
	AssessWidget_InterpolationBias* widget_interplation_bias_;

	QTabWidget* tabwidget_assess_;

	QImage* image_;

	void setupUI();
	void connectSlots();

public:
	void setAssessType(const AssessType& assess_type);

private slots:
	void SlotImportImage();
};
