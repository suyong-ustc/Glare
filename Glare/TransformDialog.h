#pragma once

#include <QDialog>
#include <QComboBox>
#include <QListWidget>
#include <QPixmap>
#include <QPushButton>
#include <QStackedWidget>
#include "SpeckleView.h"
#include "SpeckleScene.h"
#include "Pattern\GaussianPattern.h"
#include "DeformationParameterPanel_Translation.h"
#include "DeformationParameterPanel_Elongation.h"
#include "DeformationParameterPanel_Sinusoidal.h"
#include "DeformationParameterPanel_Gaussian.h"
#include "DeformationParameterPanel_PLC.h"
#include "DeformationParameterPanel_Rotation.h"
#include "DeformationParameterPanel_2DSinusoidal.h"
#include "DeformationParameterPanel_2DGaussian.h"
#include "DeformationParameterPanel_CrossSinusoidal.h"
#include "DeformationParameterPanel_CrossGaussian.h"
#include "DeformationParameterPanel_Power.h"
#include "ExportSeriesImageDialog.h"


class TransformDialog : public QDialog
{
	Q_OBJECT

public:
	TransformDialog(QWidget *parent = Q_NULLPTR);
	~TransformDialog();

private:
	
	// 参考图视图
	SpeckleView* reference_view_;
	SpeckleScene* reference_scene_;

	// 参考图散斑生成参数
	QDoubleSpinBox* doublespinbox_diameter_;
	QDoubleSpinBox* doublespinbox_density_;
	QDoubleSpinBox* doublespinbox_variation_;

	QSpinBox* spinbox_image_width_;
	QSpinBox* spinbox_image_height_;

	// 变形图视图
	SpeckleView* deformed_view_;
	SpeckleScene* deformed_scene_;

	// 变形图散斑生成参数
	QStackedWidget* deformation_parameter_panel_;
	DeformationParameterPanel_Translation* panel_translation_;
	DeformationParameterPanel_Elongation* panel_elongation_;
	DeformationParameterPanel_Sinusoidal* panel_sinusoidal_;
	DeformationParameterPanel_Gaussian* panel_gaussian_;
	DeformationParameterPanel_PLC* panel_plc_;
	DeformationParameterPanel_Rotation* panel_rotation_;
	DeformationParameterPanel_2DSinusoidal* panel_2d_sinusoidal_;
	DeformationParameterPanel_2DGaussian* panel_2d_gaussian_;
	DeformationParameterPanel_CrossSinusoidal* panel_cross_sinusoidal_;
	DeformationParameterPanel_CrossGaussian* panel_cross_gaussian_;
	DeformationParameterPanel_Power* panel_power_;

	// 变形形式选择
	QListWidget* deformation_type_list_;

	// 导出
	QPushButton* pushbutton_save_image_;
	QComboBox* combobox_save_image_type_;
	ExportSeriesImageDialog* dialog_export_series_images_;

	// 高斯散斑
	GaussianPattern* gaussian_pattern_;


private:

	void setupUI();
	void connectSlots();
	void createNewSpecklePattern();

	// 渲染散斑图
	QPixmap RenderReferencePixmap();
	QPixmap RenderDeformedPixmap_Translation(bool using_interpolation);
	QPixmap RenderDeformedPixmap_Elongation(bool using_interpolation);
	QPixmap RenderDeformedPixmap_Sinusoidal(bool using_interpolation, bool using_iteration);
	QPixmap RenderDeformedPixmap_Gaussian(bool using_interpolation, bool using_iteration);
	QPixmap RenderDeformedPixmap_PLC(bool using_interpolation, bool using_iteration);
	QPixmap RenderDeformedPixmap_Rotation(bool using_interpolation);
	QPixmap RenderDeformedPixmap_2DSinusoidal(bool using_interpolation, bool using_iteration);
	QPixmap RenderDeformedPixmap_2DGaussian(bool using_interpolation, bool using_iteration);
	QPixmap RenderDeformedPixmap_CrossSinusoidal(bool using_interpolation, bool using_iteration);
	QPixmap RenderDeformedPixmap_CrossGaussian(bool using_interpolation, bool using_iteration);
	QPixmap RenderDeformedPixmap_Power(bool using_interpolation, bool using_iteration);



signals:
	void SignalReferenceParameterChanged();

public:
	void setDeformationType(const TransformType& transform_type);

private slots:

	// 刷新界面
	void SlotShowReferenceImage();
	void SlotShowDeformedImage();

	// 存储图片
	void SlotSaveImage();

};
