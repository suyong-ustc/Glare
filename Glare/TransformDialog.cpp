#include "TransformDialog.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <armadillo>
#include "Pattern\RenderPatternPixmap.h"
#include "InverseMap.h"
using namespace arma;


TransformDialog::TransformDialog(QWidget *parent)
	: QDialog(parent)
{
	// 初始化界面
	setupUI();

	// 连接信号槽
	connectSlots();

	// 初始化显示散斑
	gaussian_pattern_ = new GaussianPattern(20, 0.5, 0.5, 100, 100);
	SlotShowReferenceImage();

	resize(QSize(1200, 900));
}



TransformDialog::~TransformDialog()
{

}



void TransformDialog::setupUI()
{
	setWindowTitle(tr("Image transformation"));

	// 使用网格布局
	QGridLayout* layout = new QGridLayout;
	setLayout(layout);

	// 参考图生成参数选择
	QGridLayout* reference_parameter_layout = new QGridLayout;

	// 直径
	QLabel* label_diameter = new QLabel(tr("Diameter"));
	reference_parameter_layout->addWidget(label_diameter, 0, 0);

	doublespinbox_diameter_ = new QDoubleSpinBox;
	doublespinbox_diameter_->setValue(4);
	doublespinbox_diameter_->setMinimum(0.5);
	doublespinbox_diameter_->setMaximum(99);
	doublespinbox_diameter_->setDecimals(1);
	doublespinbox_diameter_->setSingleStep(0.1);
	doublespinbox_diameter_->setSuffix(tr(" Pixel"));
	reference_parameter_layout->addWidget(doublespinbox_diameter_, 0, 1);

	// 占空比
	QLabel* label_density = new QLabel(tr("Density"));
	reference_parameter_layout->addWidget(label_density, 1, 0);

	doublespinbox_density_ = new QDoubleSpinBox;
	doublespinbox_density_->setValue(60);
	doublespinbox_density_->setMinimum(0);
	doublespinbox_density_->setMaximum(150);
	doublespinbox_density_->setDecimals(0);
	doublespinbox_density_->setSingleStep(5);
	doublespinbox_density_->setSuffix(" %");
	reference_parameter_layout->addWidget(doublespinbox_density_, 1, 1);

	// 随机度
	QLabel* label_variation = new QLabel(tr("Variation"));
	reference_parameter_layout->addWidget(label_variation, 2, 0);

	doublespinbox_variation_ = new QDoubleSpinBox;
	doublespinbox_variation_->setValue(40);
	doublespinbox_variation_->setMinimum(0);
	doublespinbox_variation_->setMaximum(200);
	doublespinbox_variation_->setDecimals(0);
	doublespinbox_variation_->setSingleStep(1);
	doublespinbox_variation_->setSuffix(" %");
	reference_parameter_layout->addWidget(doublespinbox_variation_, 2, 1);


	// 图像宽度
	QLabel* label_width = new QLabel(tr("Width"));
	reference_parameter_layout->addWidget(label_width, 3, 0);

	spinbox_image_width_ = new QSpinBox;
	spinbox_image_width_->setMinimum(10);
	spinbox_image_width_->setMaximum(9999);
	spinbox_image_width_->setValue(300);
	spinbox_image_width_->setSingleStep(20);
	spinbox_image_width_->setSuffix(tr(" Pixel"));
	reference_parameter_layout->addWidget(spinbox_image_width_, 3, 1);


	// 图像高度
	QLabel* label_height = new QLabel(tr("Height"));
	reference_parameter_layout->addWidget(label_height, 4, 0);

	spinbox_image_height_ = new QSpinBox;
	spinbox_image_height_->setMinimum(10);
	spinbox_image_height_->setMaximum(9999);
	spinbox_image_height_->setValue(300);
	spinbox_image_height_->setSingleStep(20);
	spinbox_image_height_->setSuffix(tr(" Pixel"));
	reference_parameter_layout->addWidget(spinbox_image_height_, 4, 1);


	// 参考图显示
	reference_view_ = new SpeckleView;
	reference_scene_ = new SpeckleScene;
	reference_view_->setScene(reference_scene_);
	reference_view_->setMinimumSize(QSize(400, 400));

	// 变形参数选择
	deformation_parameter_panel_ = new QStackedWidget;
	panel_translation_ = new DeformationParameterPanel_Translation;
	panel_elongation_ = new DeformationParameterPanel_Elongation;
	panel_sinusoidal_ = new DeformationParameterPanel_Sinusoidal;
	panel_gaussian_ = new DeformationParameterPanel_Gaussian;
	panel_plc_ = new DeformationParameterPanel_PLC;
	panel_rotation_ = new DeformationParameterPanel_Rotation;
	panel_2d_sinusoidal_ = new DeformationParameterPanel_2DSinusoidal;
	panel_2d_gaussian_ = new DeformationParameterPanel_2DGaussian;
	panel_cross_sinusoidal_ = new DeformationParameterPanel_CrossSinusoidal;
	panel_cross_gaussian_ = new DeformationParameterPanel_CrossGaussian;
	panel_power_ = new DeformationParameterPanel_Power;

	deformation_parameter_panel_->addWidget(panel_translation_);
	deformation_parameter_panel_->addWidget(panel_elongation_);
	deformation_parameter_panel_->addWidget(panel_sinusoidal_);
	deformation_parameter_panel_->addWidget(panel_gaussian_);
	deformation_parameter_panel_->addWidget(panel_plc_);
	deformation_parameter_panel_->addWidget(panel_rotation_);
	deformation_parameter_panel_->addWidget(panel_2d_sinusoidal_);
	deformation_parameter_panel_->addWidget(panel_2d_gaussian_);
	deformation_parameter_panel_->addWidget(panel_cross_sinusoidal_);
	deformation_parameter_panel_->addWidget(panel_cross_gaussian_);
	deformation_parameter_panel_->addWidget(panel_power_);
	deformation_parameter_panel_->setCurrentIndex(0);

	// 导出图片
	QHBoxLayout* save_image_layout = new QHBoxLayout;

	pushbutton_save_image_ = new QPushButton(tr("Save"));
	save_image_layout->addWidget(pushbutton_save_image_);
	save_image_layout->setStretchFactor(pushbutton_save_image_, 1);

	combobox_save_image_type_ = new QComboBox;
	combobox_save_image_type_->addItem(tr("Single Image"));
	combobox_save_image_type_->addItem(tr("Series Images"));
	save_image_layout->addWidget(combobox_save_image_type_);
	save_image_layout->setStretchFactor(combobox_save_image_type_, 1);


	QVBoxLayout* deform_parameter_layout = new QVBoxLayout;
	deform_parameter_layout->addWidget(deformation_parameter_panel_);
	deform_parameter_layout->addStretch();
	deform_parameter_layout->addLayout(save_image_layout);

	// 变形图显示
	deformed_view_ = new SpeckleView;
	deformed_scene_ = new SpeckleScene;
	deformed_view_->setScene(deformed_scene_);
	deformed_view_->setMinimumSize(QSize(400, 400));

	// 选择变形形式：平移、拉伸、正弦、高斯
	deformation_type_list_ = new QListWidget(this);
	deformation_type_list_->insertItem(0, tr("Translation"));
	deformation_type_list_->insertItem(1, tr("Stretch"));
	deformation_type_list_->insertItem(2, tr("Sinusoidal"));
	deformation_type_list_->insertItem(3, tr("Gaussian"));
	deformation_type_list_->insertItem(4, tr("PLC Band"));
	deformation_type_list_->insertItem(5, tr("Rotation"));
	deformation_type_list_->insertItem(6, tr("2D Sinusoidal"));
	deformation_type_list_->insertItem(7, tr("2D Gaussian"));
	deformation_type_list_->insertItem(8, tr("Cross Sinusoidal"));
	deformation_type_list_->insertItem(9, tr("Cross Gaussian"));
	deformation_type_list_->insertItem(10, tr("Power"));

	// 导出
	layout->addWidget(reference_view_, 0, 0);
	layout->addLayout(reference_parameter_layout, 1, 0);
	layout->addLayout(deform_parameter_layout, 1, 1);
	layout->addWidget(deformed_view_, 0, 1);
	layout->addWidget(deformation_type_list_, 0, 2, 2, 1);

	layout->setColumnStretch(0, 3);
	layout->setColumnStretch(1, 3);
	layout->setColumnStretch(2, 1);
	layout->setRowStretch(0, 3);
	layout->setRowStretch(1, 1);

	dialog_export_series_images_ = new ExportSeriesImageDialog(this);
}



void TransformDialog::connectSlots()
{
	bool is_ok = false;

	// 选择不同类型的散斑，更改散斑参数面板
	is_ok = connect(deformation_type_list_, &QListWidget::currentRowChanged,
					deformation_parameter_panel_, &QStackedWidget::setCurrentIndex);

	// 选择不同类型的散斑，重绘变形散斑
	is_ok = connect(deformation_type_list_, &QListWidget::itemSelectionChanged,
					this, &TransformDialog::SlotShowDeformedImage);

	// 参考图生成参数变化，发出重绘信号
	is_ok = connect(doublespinbox_diameter_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalReferenceParameterChanged(); });

	is_ok = connect(doublespinbox_density_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalReferenceParameterChanged(); });

	is_ok = connect(doublespinbox_variation_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { emit SignalReferenceParameterChanged(); });

	is_ok = connect(spinbox_image_width_, QOverload<int>::of(&QSpinBox::valueChanged),
		this, [=]() { emit SignalReferenceParameterChanged(); });

	is_ok = connect(spinbox_image_height_, QOverload<int>::of(&QSpinBox::valueChanged),
		this, [=]() { emit SignalReferenceParameterChanged(); });

	// 参考图生成参数变化，重绘参考图
	is_ok = connect(this, &TransformDialog::SignalReferenceParameterChanged,
					this, &TransformDialog::SlotShowReferenceImage);

	// 参考图生成参数变化，重绘变形图
	is_ok = connect(this, &TransformDialog::SignalReferenceParameterChanged,
					this, &TransformDialog::SlotShowDeformedImage);

	// 平移参数变化，重绘平移变形图
	is_ok = connect(panel_translation_, &DeformationParameterPanel_Translation::SignalParameterChanged,
					this, &TransformDialog::SlotShowDeformedImage);

	// 拉伸参数变化，重绘拉伸变形图
	is_ok = connect(panel_elongation_, &DeformationParameterPanel_Elongation::SignalParameterChanged,
					this, &TransformDialog::SlotShowDeformedImage);
	
	// 正弦参数变化，重绘正弦变形图
	is_ok = connect(panel_sinusoidal_, &DeformationParameterPanel_Sinusoidal::SignalParameterChanged,
					this, &TransformDialog::SlotShowDeformedImage);

	// 高斯参数变化，重绘高斯变形图
	is_ok = connect(panel_gaussian_, &DeformationParameterPanel_Gaussian::SignalParameterChanged,
					this, &TransformDialog::SlotShowDeformedImage);

	// 剪切带参数变化，重绘剪切带变形图
	is_ok = connect(panel_plc_, &DeformationParameterPanel_PLC::SignalParameterChanged,
					this, &TransformDialog::SlotShowDeformedImage);

	// 旋转参数变化，重绘旋转变形图
	is_ok = connect(panel_rotation_, &DeformationParameterPanel_Rotation::SignalParameterChanged,
					this, &TransformDialog::SlotShowDeformedImage);

	// 二维正弦参数变化，重绘二维正弦变形图
	is_ok = connect(panel_2d_sinusoidal_, &DeformationParameterPanel_2DSinusoidal::SignalParameterChanged,
		this, &TransformDialog::SlotShowDeformedImage);

	// 二维高斯参数变化，重绘二维高斯变形图
	is_ok = connect(panel_2d_gaussian_, &DeformationParameterPanel_2DGaussian::SignalParameterChanged,
		this, &TransformDialog::SlotShowDeformedImage);

	// 交叉正弦参数变化，重绘交叉正弦变形图
	is_ok = connect(panel_cross_sinusoidal_, &DeformationParameterPanel_CrossSinusoidal::SignalParameterChanged,
		this, &TransformDialog::SlotShowDeformedImage);

	// 交叉高斯参数变化，重绘交叉高斯变形图
	is_ok = connect(panel_cross_gaussian_, &DeformationParameterPanel_CrossGaussian::SignalParameterChanged,
		this, &TransformDialog::SlotShowDeformedImage);

	// 幂指数参数变化，重绘交叉高斯变形图
	is_ok = connect(panel_power_, &DeformationParameterPanel_Power::SignalParameterChanged,
		this, &TransformDialog::SlotShowDeformedImage);

	// 导出图像
	is_ok = connect(pushbutton_save_image_, &QPushButton::clicked,
					this, &TransformDialog::SlotSaveImage);

}



void TransformDialog::createNewSpecklePattern()
{
	// 读取参数
	const double diameter = doublespinbox_diameter_->value();
	const double coverage = 0.01 * doublespinbox_density_->value();
	const double variation = 0.01 * doublespinbox_variation_->value();
	const int width = spinbox_image_width_->value();
	const int height = spinbox_image_height_->value();

	// 生成新的高斯散斑
	delete gaussian_pattern_;
	gaussian_pattern_ = new GaussianPattern(diameter, coverage, variation, height, width);

}



void TransformDialog::setDeformationType(const TransformType& transform_type)
{
	if (transform_type == TRANSLATION_DEFORMATION)
		deformation_type_list_->setCurrentRow(0);
	else if (transform_type == ELONGATION_DEFORMATION)
		deformation_type_list_->setCurrentRow(1);
	else if (transform_type == SINUSOIDAL_DEFORMATION)
		deformation_type_list_->setCurrentRow(2);
	else if (transform_type == GAUSSIAN_DEFORMATION)
		deformation_type_list_->setCurrentRow(3);
	else if (transform_type == PLC_BAND_DEFORMATION)
		deformation_type_list_->setCurrentRow(4);
	else if (transform_type == ROTATION_DEFORMATION)
		deformation_type_list_->setCurrentRow(5);
	else if (transform_type == TWO_DIMENSIONAL_SINUSOIDAL_DEFORMATION)
		deformation_type_list_->setCurrentRow(6);
	else if (transform_type == TWO_DIMENSIONAL_GAUSSIAN_DEFORMATION)
		deformation_type_list_->setCurrentRow(7);
	else if (transform_type == CROSS_SINUSOIDAL_DEFORMATION)
		deformation_type_list_->setCurrentRow(8);
	else if (transform_type == CROSS_GAUSSIAN_DEFORMATION)
		deformation_type_list_->setCurrentRow(9);
	else if (transform_type == POWER_DEFORMATION)
		deformation_type_list_->setCurrentRow(10);

}



QPixmap TransformDialog::RenderReferencePixmap()
{
	// 读取参数
	const int width = spinbox_image_width_->value();
	const int height = spinbox_image_height_->value();

	// 渲染图片
	QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, height, width, Qt::black);

	// 返回参考图图片
	return pixmap;
}



QPixmap TransformDialog::RenderDeformedPixmap_Translation(bool using_interpolation)
{
	// 采样点在变形图中的位置
	mat deform_x, deform_y;
	RenderPatternPixmap::MeshGrid(0, 0, spinbox_image_width_->value(), spinbox_image_height_->value(), deform_x, deform_y);

	// 采样点在参考图中的位置
	mat refer_x, refer_y;
	InverseMap::InverseMap_TranslationDeformation(panel_translation_->u0(), deform_x, deform_y, refer_x, refer_y);

	// 渲染变形图
	QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, refer_x, refer_y, Qt::black, using_interpolation);

	// 返回渲染图
	return pixmap;

}



QPixmap TransformDialog::RenderDeformedPixmap_Elongation(bool using_interpolation)
{
	// 读取参数
	const double a = panel_elongation_->a();
	const double x0 = panel_elongation_->x0();

	// 图像尺寸
	const int width = spinbox_image_width_->value();
	const int height = spinbox_image_height_->value();

	// 采样点在变形图中的位置
	mat deform_x, deform_y;
	RenderPatternPixmap::MeshGrid(0, 0, width, height, deform_x, deform_y);

	// 采样点在参考图中的位置
	mat refer_x, refer_y;
	InverseMap::InverseMap_ElongtationDeformation(a, x0, deform_x, deform_y, refer_x, refer_y);

	// 渲染变形图
	QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, refer_x, refer_y, Qt::black, using_interpolation);

	// 返回渲染图
	return pixmap;
}



QPixmap TransformDialog::RenderDeformedPixmap_Sinusoidal(bool using_interpolation, bool with_iteration)
{
	// 读取参数
	const double a = panel_sinusoidal_->a();
	const double T = panel_sinusoidal_->T();
	const double b = panel_sinusoidal_->b();

	// 图像尺寸
	const int width = spinbox_image_width_->value();
	const int height = spinbox_image_height_->value();

	// 估计变形图像素点在参考图中的位置
	rowvec deform_x = regspace<rowvec>(0, width - 1);
	rowvec refer_x;
	if (with_iteration)
		refer_x = InverseMap::InverseMap_SinusoidalDeformation(a, T, b, width);
	else
		refer_x = InverseMap::EstimateInitialPosition_SinusoidalDeformation(a, T, b, width);

	mat xmat = repmat(refer_x, height, 1);

	vec refer_y = regspace<vec>(0, height - 1);
	mat ymat = repmat(refer_y, 1, width);

	// 渲染变形图
	QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, xmat, ymat, Qt::black, using_interpolation);

	// 返回渲染图
	return pixmap;
}



QPixmap TransformDialog::RenderDeformedPixmap_Gaussian(bool using_interpolation, bool with_iteration)
{
	// 读取参数
	const double a = panel_gaussian_->a();
	const double x0 = panel_gaussian_->x0();
	const double c = panel_gaussian_->c();

	// 图像尺寸
	const int width = spinbox_image_width_->value();
	const int height = spinbox_image_height_->value();


	// 估计变形图像素点在参考图中的位置
	rowvec deform_x = regspace<rowvec>(0, width - 1);
	rowvec refer_x;
	if (with_iteration)
		refer_x = InverseMap::InverseMap_GaussianDeformation(a, x0, c, width);
	else
		refer_x = InverseMap::EstimateInitialPosition_GaussianDeformation(a, x0, c, width);

	mat xmat = repmat(refer_x, height, 1);

	vec refer_y = regspace<vec>(0, height - 1);
	mat ymat = repmat(refer_y, 1, width);

	// 渲染变形图
	QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, xmat, ymat, Qt::black, using_interpolation);

	// 返回渲染图
	return pixmap;
}



QPixmap TransformDialog::RenderDeformedPixmap_PLC(bool using_interpolation, bool with_iteration)
{
	// 读取参数
	const double a = panel_plc_->a();
	const double x0 = panel_plc_->x0();
	const double c = panel_plc_->c();

	// 图像尺寸
	const int width = spinbox_image_width_->value();
	const int height = spinbox_image_height_->value();


	// 估计变形图像素点在参考图中的位置
	rowvec deform_x = regspace<rowvec>(0, width - 1);
	rowvec refer_x;
	if (with_iteration)
		refer_x = InverseMap::InverseMap_PLCDeformation(a, x0, c, width);
	else
		refer_x = InverseMap::EstimateInitialPosition_PLCDeformation(a, x0, c, width);

	mat xmat = repmat(refer_x, height, 1);

	vec refer_y = regspace<vec>(0, height - 1);
	mat ymat = repmat(refer_y, 1, width);

	// 渲染变形图
	QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, xmat, ymat, Qt::black, using_interpolation);

	// 返回渲染图
	return pixmap;
}



QPixmap TransformDialog::RenderDeformedPixmap_Rotation(bool using_interpolation)
{
	// 读取参数
	const double a = panel_rotation_->a();
	const double theta = panel_rotation_->theta();
	const double x0 = panel_rotation_->x0();
	const double y0 = panel_rotation_->y0();

	// 图像尺寸
	const int width = spinbox_image_width_->value();
	const int height = spinbox_image_height_->value();

	// 采样点在变形图中的位置
	mat deform_x, deform_y;
	RenderPatternPixmap::MeshGrid(0, 0, width, height, deform_x, deform_y);

	// 采样点在参考图中的位置
	mat refer_x, refer_y;
	InverseMap::InverseMap_RotationDeformation(a, theta, x0, y0, deform_x, deform_y, refer_x, refer_y);

	// 渲染变形图
	QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, refer_x, refer_y, Qt::black, using_interpolation);

	// 返回渲染图
	return pixmap;
}



QPixmap TransformDialog::RenderDeformedPixmap_2DSinusoidal(bool using_interpolation, bool using_iteration)
{
	// 读取参数
	const double ax = panel_2d_sinusoidal_->ax();
	const double Tx = panel_2d_sinusoidal_->Tx();
	const double bx = panel_2d_sinusoidal_->bx();

	const double ay = panel_2d_sinusoidal_->ay();
	const double Ty = panel_2d_sinusoidal_->Ty();
	const double by = panel_2d_sinusoidal_->by();

	// 图像尺寸
	const int width = spinbox_image_width_->value();
	const int height = spinbox_image_height_->value();

	// 估计变形图像素点在参考图中的位置
	mat xmat;
	mat ymat;
	if (using_iteration)
		InverseMap::InverseMap_2DSinusoidalDeformation(ax, Tx, bx, width, ay, Ty, by, height, xmat, ymat);
	else
		InverseMap::EstimateInitialPosition_2DSinusoidalDeformation(ax, Tx, bx, width, ay, Ty, by, height, xmat, ymat);

	// 渲染变形图
	QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, xmat, ymat, Qt::black, using_interpolation);

	// 返回渲染图
	return pixmap;


}



QPixmap TransformDialog::RenderDeformedPixmap_2DGaussian(bool using_interpolation, bool using_iteration)
{
	// 读取参数
	const double ax = panel_2d_gaussian_->ax();
	const double x0 = panel_2d_gaussian_->x0();
	const double cx = panel_2d_gaussian_->cx();

	const double ay = panel_2d_gaussian_->ay();
	const double y0 = panel_2d_gaussian_->y0();
	const double cy = panel_2d_gaussian_->cy();

	// 图像尺寸
	const int width = spinbox_image_width_->value();
	const int height = spinbox_image_height_->value();

	// 估计变形图像素点在参考图中的位置
	mat xmat;
	mat ymat;
	if (using_iteration)
		InverseMap::InverseMap_2DGaussianDeformation(ax, x0, cx, width, ay, y0, cy, height, xmat, ymat);
	else
		InverseMap::EstimateInitialPosition_2DGaussianDeformation(ax, x0, cx, width, ay, y0, cy, height, xmat, ymat);

	// 渲染变形图
	QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, xmat, ymat, Qt::black, using_interpolation);

	// 返回渲染图
	return pixmap;

}



QPixmap TransformDialog::RenderDeformedPixmap_CrossSinusoidal(bool using_interpolation, bool using_iteration)
{
	// 读取参数
	const double a = panel_cross_sinusoidal_->a();
	const double Tx = panel_cross_sinusoidal_->Tx();
	const double bx = panel_cross_sinusoidal_->bx();
	const double Ty = panel_cross_sinusoidal_->Ty();
	const double by = panel_cross_sinusoidal_->by();

	// 图像尺寸
	const int width = spinbox_image_width_->value();
	const int height = spinbox_image_height_->value();

	// 估计变形图像素点在参考图中的位置
	mat xmat;
	mat ymat;
	if (using_iteration)
		InverseMap::InverseMap_CrossSinusoidalDeformation(a, Tx, bx, Ty, by, width, height, xmat, ymat);
	else
		InverseMap::EstimateInitialPosition_CrossSinusoidalDeformation(a, Tx, bx, Ty, by, width, height, xmat, ymat);

	// 渲染变形图
	QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, xmat, ymat, Qt::black, using_interpolation);

	// 返回渲染图
	return pixmap;

}


QPixmap TransformDialog::RenderDeformedPixmap_CrossGaussian(bool using_interpolation, bool using_iteration)
{
	// 读取参数
	const double a = panel_cross_gaussian_->a();
	const double x0 = panel_cross_gaussian_->x0();
	const double cx = panel_cross_gaussian_->cx();
	const double y0 = panel_cross_gaussian_->y0();
	const double cy = panel_cross_gaussian_->cy();

	// 图像尺寸
	const int width = spinbox_image_width_->value();
	const int height = spinbox_image_height_->value();

	// 估计变形图像素点在参考图中的位置
	mat xmat;
	mat ymat;
	if (using_iteration)
		InverseMap::InverseMap_CrossGaussianDeformation(a, x0, cx, y0, cy, width, height, xmat, ymat);
	else
		InverseMap::EstimateInitialPosition_CrossGaussianDeformation(a, x0, cx, y0, cy, width, height, xmat, ymat);

	// 渲染变形图
	QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, xmat, ymat, Qt::black, using_interpolation);

	// 返回渲染图
	return pixmap;
}



QPixmap TransformDialog::RenderDeformedPixmap_Power(bool using_interpolation, bool using_iteration)
{
	// 读取参数
	const double a = panel_power_->a();
	const double x0 = panel_power_->x0();
	const double n = panel_power_->n();

	// 图像尺寸
	const int width = spinbox_image_width_->value();
	const int height = spinbox_image_height_->value();

	// 估计变形图像素点在参考图中的位置
	rowvec deform_x = regspace<rowvec>(0, width - 1);
	rowvec refer_x;
	if (using_iteration)
		refer_x = InverseMap::InverseMap_PowerDeformation(a, x0, n, width);
	else
		refer_x = InverseMap::EstimateInitialPosition_PowerDeformation(a, x0, n, width);

	mat xmat = repmat(refer_x, height, 1);

	vec refer_y = regspace<vec>(0, height - 1);
	mat ymat = repmat(refer_y, 1, width);

	// 渲染变形图
	QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, xmat, ymat, Qt::black, using_interpolation);

	// 返回渲染图
	return pixmap;
}



void TransformDialog::SlotShowReferenceImage()
{
	// 生成新的高斯散斑
	createNewSpecklePattern();

	// 渲染图片
	QPixmap pixmap = RenderReferencePixmap();

	// 设置视图大小
	if (abs(reference_scene_->height() - spinbox_image_height_->value()) +
		abs(reference_scene_->width() - spinbox_image_width_->value()) > 0.5)
		reference_scene_->setSceneRect(0, 0, spinbox_image_width_->value(), spinbox_image_height_->value());

	// 显示参考图片
	reference_scene_->clear();
	QGraphicsPixmapItem* pixmapItem = reference_scene_->addPixmap(pixmap);
	pixmapItem->show();

}



void TransformDialog::SlotShowDeformedImage()
{
	// 当前的变形模式
	TransformType deformation_type = static_cast<TransformType>(deformation_type_list_->currentRow());

	// 渲染图片
	QPixmap pixmap;
	if (deformation_type == TRANSLATION_DEFORMATION)
		pixmap = RenderDeformedPixmap_Translation(true);
	else if (deformation_type == ELONGATION_DEFORMATION)
		pixmap = RenderDeformedPixmap_Elongation(true);
	else if (deformation_type == SINUSOIDAL_DEFORMATION)
		pixmap = RenderDeformedPixmap_Sinusoidal(true, false);
	else if (deformation_type == GAUSSIAN_DEFORMATION)
		pixmap = RenderDeformedPixmap_Gaussian(true, false);
	else if (deformation_type == PLC_BAND_DEFORMATION)
		pixmap = RenderDeformedPixmap_PLC(true, false);
	else if (deformation_type == ROTATION_DEFORMATION)
		pixmap = RenderDeformedPixmap_Rotation(true);
	else if (deformation_type == TWO_DIMENSIONAL_SINUSOIDAL_DEFORMATION)
		pixmap = RenderDeformedPixmap_2DSinusoidal(true, false);
	else if (deformation_type == TWO_DIMENSIONAL_GAUSSIAN_DEFORMATION)
		pixmap = RenderDeformedPixmap_2DGaussian(true, false);
	else if (deformation_type == CROSS_SINUSOIDAL_DEFORMATION)
		pixmap = RenderDeformedPixmap_CrossSinusoidal(true, false);
	else if (deformation_type == CROSS_GAUSSIAN_DEFORMATION)
		pixmap = RenderDeformedPixmap_CrossGaussian(true, false);
	else if (deformation_type == POWER_DEFORMATION)
		pixmap = RenderDeformedPixmap_Power(true, false);

	// 设置视图大小
	if (abs(deformed_scene_->height() - spinbox_image_height_->value()) + 
		abs(deformed_scene_->width() - spinbox_image_width_->value()) > 0.5)
		deformed_scene_->setSceneRect(0, 0, spinbox_image_width_->value(), spinbox_image_height_->value());

	// 显示变形图片
	deformed_scene_->clear();
	QGraphicsPixmapItem* pixmapItem = deformed_scene_->addPixmap(pixmap);
	pixmapItem->show();

}



void TransformDialog::SlotSaveImage()
{
	if (combobox_save_image_type_->currentIndex() == 0)
	{
		// 导出的文件名
		QFileDialog dialog(this);
		dialog.setWindowModality(Qt::WindowModal);
		dialog.setNameFilter(tr("Images (*.bmp)"));
		dialog.setAcceptMode(QFileDialog::AcceptSave);
		dialog.exec();
		QStringList files = dialog.selectedFiles();
		if (files.isEmpty())
			return;
		QString bmp_name = files.at(0);

		QString prefix = bmp_name.split(".")[0];
		QString refer_image_name = prefix + "_0.bmp";
		QString deform_image_name = prefix + "_1.bmp";


		// 存储参考图
		QPixmap refer_image_pixmap = RenderReferencePixmap();
		refer_image_pixmap.save(refer_image_name, "BMP");

		// 存储变形图
		QPixmap deform_image_pixmap;

		TransformType deformation_type = static_cast<TransformType>(deformation_type_list_->currentRow());
		if (deformation_type == TRANSLATION_DEFORMATION)
			deform_image_pixmap = RenderDeformedPixmap_Translation(false);
		else if (deformation_type == ELONGATION_DEFORMATION)
			deform_image_pixmap = RenderDeformedPixmap_Elongation(false);
		else if (deformation_type == SINUSOIDAL_DEFORMATION)
			deform_image_pixmap = RenderDeformedPixmap_Sinusoidal(false, true);
		else if (deformation_type == GAUSSIAN_DEFORMATION)
			deform_image_pixmap = RenderDeformedPixmap_Gaussian(false, true);
		else if (deformation_type == PLC_BAND_DEFORMATION)
			deform_image_pixmap = RenderDeformedPixmap_PLC(false, true);
		else if (deformation_type == ROTATION_DEFORMATION)
			deform_image_pixmap = RenderDeformedPixmap_Rotation(false);
		else if (deformation_type == TWO_DIMENSIONAL_SINUSOIDAL_DEFORMATION)
			deform_image_pixmap = RenderDeformedPixmap_2DSinusoidal(false, true);
		else if (deformation_type == TWO_DIMENSIONAL_GAUSSIAN_DEFORMATION)
			deform_image_pixmap = RenderDeformedPixmap_2DGaussian(false, true);
		else if (deformation_type == CROSS_SINUSOIDAL_DEFORMATION)
			deform_image_pixmap = RenderDeformedPixmap_CrossSinusoidal(false, true);
		else if (deformation_type == CROSS_GAUSSIAN_DEFORMATION)
			deform_image_pixmap = RenderDeformedPixmap_CrossGaussian(false, true);
		else if (deformation_type == POWER_DEFORMATION)
			deform_image_pixmap = RenderDeformedPixmap_Power(false, true);

		deform_image_pixmap.save(deform_image_name, "BMP");
	}
	else if (combobox_save_image_type_->currentIndex() == 1)
	{
		// 图像大小
		const int width = spinbox_image_width_->value();
		const int height = spinbox_image_height_->value();
		QSize image_size(width, height);

		TransformType deformation_type = static_cast<TransformType>(deformation_type_list_->currentRow());

		dialog_export_series_images_->setCurrentTab(deformation_type);
		dialog_export_series_images_->setPattern(gaussian_pattern_);
		dialog_export_series_images_->setImageSize(image_size);
		dialog_export_series_images_->open();

	}


}