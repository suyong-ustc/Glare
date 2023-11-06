#pragma once

#include <QWidget>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QPixmap>
#include <QSpinBox>
#include "SpeckleScene.h"
#include "SpeckleView.h"
#include "Pattern\EllipseSpecklePattern.h"
#include "Pattern\PolygonSpecklePattern.h"
#include "Pattern\GaussianPattern.h"


class CentralWidget : public QWidget
{
	Q_OBJECT

public:
	CentralWidget(QWidget *parent = Q_NULLPTR);
	~CentralWidget();

private:

	QComboBox* combobox_mode_;
	QComboBox* combobox_antialiasing_;
	QComboBox* combobox_background_;

	QLabel* label_render_;
	QLabel* label_eccentricity_;
	QLabel* label_edge_;
	QLabel* label_ratio_;

	QDoubleSpinBox* doublespinbox_diameter_;
	QDoubleSpinBox* doublespinbox_eccentricity_;
	QDoubleSpinBox* doublespinbox_edge_;
	QDoubleSpinBox* doublespinbox_ratio_;
	QDoubleSpinBox* doublespinbox_density_;
	QDoubleSpinBox* doublespinbox_variation_;

	QSpinBox* spinbox_image_width_;
	QSpinBox* spinbox_image_height_;

	SpeckleScene* speckle_scene_;
	SpeckleView* speckle_view_;

	EllipseSpecklePattern* ellipse_speckle_pattern_;
	PolygonSpecklePattern* polygon_speckle_pattern_;
	GaussianPattern* gaussian_pattern_;

private:

	QGroupBox* createModeGroupbox();
	QGroupBox* createPatternGroupbox();
	QGroupBox* createResolutionGroupbox();

	void setupUI();
	void connectSlots();


public slots:
	void SlotElementTypeChanged(int type_number);
	void SlotAntialiasingTypeChanged(int type_number);
	void SlotDrawSpecklePattern();
	void SlotSaveImage();
};
