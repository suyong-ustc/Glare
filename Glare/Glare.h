#pragma once

#include <QtWidgets/QMainWindow>
#include <QAction>
#include "AssessDialog.h"
#include "CentralWidget.h"
#include "Recommendation\RecommendDialog.h"
#include "TransformDialog.h"


class Glare : public QMainWindow
{
    Q_OBJECT

public:
    Glare(QWidget *parent = Q_NULLPTR);


private:
	
	void setupMenuBar();
	void ConnectSlots();

	CentralWidget* pattern_widget_;
	AssessDialog* assess_dialog_;
	RecommendDialog* recommand_dialog_;
	TransformDialog* transform_dialog_;


	QAction* action_save_image_;
	QAction* action_quit_;

	QAction* action_transform_translation_;
	QAction* action_transform_elongation_;
	QAction* action_transform_sinusoidal_;
	QAction* action_transform_gaussian_;
	QAction* action_transform_plc_band_;
	QAction* action_transform_rotation_;
	QAction* action_transform_2D_sinusoidal_;
	QAction* action_transform_2D_gaussian_;
	QAction* action_transform_cross_sinusoidal_;
	QAction* action_transform_cross_gaussian_;
	QAction* action_transform_power_;

	QAction* action_assess_coverage_;
	QAction* action_assess_autocorrelation_;
	QAction* action_assess_bias_;
	QAction* action_assess_noise_;

	QAction* action_recommand_;

	QAction* action_manual_;
	QAction* action_about_glare_;

public slots:

	void SlotTransform_Translation();
	void SlotTransform_Elongation();
	void SlotTransform_Sinusoidal();
	void SlotTransform_Gaussian();
	void SlotTransform_PLC();
	void SlotTransform_Rotation();
	void SlotTransform_2DSinusoidal();
	void SlotTransform_2DGaussian();
	void SlotTransform_CrossSinusoidal();
	void SlotTransform_CrossGaussian();
	void SlotTransform_Power();

	void SlotAssess_Coverage();
	void SlotAssess_Autocorrelation();
	void SlotAssess_Bias();
	void SlotAssess_Noise();

	void SlotOpenManual();
	void SlotAboutGlare();
};