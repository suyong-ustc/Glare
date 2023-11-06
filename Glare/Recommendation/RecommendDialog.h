#pragma once

#include <QDialog>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QPushButton>
#include "..\SpeckleScene.h"
#include "..\SpeckleView.h"
#include "..\Pattern\EllipseSpecklePattern.h"

class RecommendDialog : public QDialog
{
	Q_OBJECT

public:
	RecommendDialog(QWidget *parent = Q_NULLPTR);
	~RecommendDialog();


private:

	QDoubleSpinBox* doublespinbox_fov_width_;
	QDoubleSpinBox* doublespinbox_fov_height_;
	QDoubleSpinBox* doublespinbox_object_distance_;
	QDoubleSpinBox* doublespinbox_image_distance_;
	QDoubleSpinBox* doublespinbox_pixel_size_;
	QDoubleSpinBox* doublespinbox_image_width_;
	QDoubleSpinBox* doublespinbox_image_height_;
	QPushButton* pushbutton_export_;

	SpeckleScene* speckle_scene_;
	SpeckleView* speckle_view_;

	EllipseSpecklePattern* speckle_pattern_;

	void setupUI();
	QGroupBox* setupFOVGroupBox();
	QGroupBox* setupDistanceGroupBox();
	QGroupBox* setupImageGroupBox();

	void connectSlots();

	void PaintSpecklePattern();

private slots:
	void SlotFovWidthChanged(double);
	void SlotFovHeightChanged(double);
	void SlotObjectDistanceChanged(double);
	void SlotImageDistanceChanged(double);
	void SlotPixelSizeChanged(double);
	void SlotImageWidthChanged(double);
	void SlotImageHeightChanged(double);
	void SlotExport();
};
