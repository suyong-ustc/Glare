#include "CentralWidget.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include "RenderPatternPixmap.h"
#include "XianType.h"


CentralWidget::CentralWidget(QWidget *parent)
	: QWidget(parent)
{
	// 初始化界面
	setupUI();

	// 初始化散斑
	ellipse_speckle_pattern_ = new EllipseSpecklePattern(20, 0.5, 0.5, 0, 0, 100, 100);
	polygon_speckle_pattern_ = new PolygonSpecklePattern(20, 0.5, 0.5, 3, 0, 100, 100);
	gaussian_pattern_ = new GaussianPattern(20, 0.5, 0.5, 100, 100);

	// 连接信号和槽
	connectSlots();

	// 初始化
	SlotElementTypeChanged(combobox_mode_->currentIndex());
	speckle_view_->setRenderHint(QPainter::Antialiasing, true);
}



CentralWidget::~CentralWidget()
{
}



QGroupBox* CentralWidget::createModeGroupbox()
{
	QGroupBox* groupbox = new QGroupBox(tr("Mode"));
	QGridLayout* layout = new QGridLayout;
	groupbox->setLayout(layout);

	// 模式：椭圆、多边形、高斯
	QLabel* label_mode = new QLabel(tr("Element"));
	layout->addWidget(label_mode, 0, 0);

	combobox_mode_ = new QComboBox;
	combobox_mode_->addItem(tr("Ellipse"));
	combobox_mode_->addItem(tr("Polygon"));
	combobox_mode_->addItem(tr("Gaussian"));
	layout->addWidget(combobox_mode_, 0, 1);

	// 抗锯齿
	label_render_ = new QLabel(tr("Render"));
	layout->addWidget(label_render_, 1, 0);

	combobox_antialiasing_ = new QComboBox;
	combobox_antialiasing_->addItem(tr("Antialising"));
	combobox_antialiasing_->addItem(tr("No-Antialising"));
	layout->addWidget(combobox_antialiasing_, 1, 1);

	// 背景：白色、黑色
	QLabel* label_background = new QLabel(tr("Background"));
	layout->addWidget(label_background, 2, 0);

	combobox_background_ = new QComboBox;
	combobox_background_->addItem(tr("White"));
	combobox_background_->addItem(tr("Black"));
	layout->addWidget(combobox_background_, 2, 1);

	return groupbox;
}



QGroupBox* CentralWidget::createPatternGroupbox()
{
	QGroupBox* groupbox = new QGroupBox(tr("Pattern"));
	QGridLayout* layout = new QGridLayout;
	groupbox->setLayout(layout);

	// 直径
	QLabel* label_diameter = new QLabel(tr("Diameter"));
	layout->addWidget(label_diameter, 0, 0);

	doublespinbox_diameter_ = new QDoubleSpinBox;
	doublespinbox_diameter_->setMinimum(0.5);
	doublespinbox_diameter_->setMaximum(99);
	doublespinbox_diameter_->setDecimals(1);
	doublespinbox_diameter_->setSingleStep(0.1);
	doublespinbox_diameter_->setValue(20);
	doublespinbox_diameter_->setSuffix(tr(" Pixel"));
	layout->addWidget(doublespinbox_diameter_, 0, 1);

	// 椭圆偏心率
	label_eccentricity_ = new QLabel(tr("Eccentricity"));
	layout->addWidget(label_eccentricity_, 1, 0);

	doublespinbox_eccentricity_ = new QDoubleSpinBox;
	doublespinbox_eccentricity_->setMinimum(0);
	doublespinbox_eccentricity_->setMaximum(0.99);
	doublespinbox_eccentricity_->setDecimals(2);
	doublespinbox_eccentricity_->setSingleStep(0.05);
	doublespinbox_eccentricity_->setValue(0);
	layout->addWidget(doublespinbox_eccentricity_, 1, 1);

	// 多边形边数
	label_edge_ = new QLabel(tr("Edge"));
	layout->addWidget(label_edge_, 2, 0);

	doublespinbox_edge_ = new QDoubleSpinBox;
	doublespinbox_edge_->setMinimum(0);
	doublespinbox_edge_->setMaximum(20);
	doublespinbox_edge_->setDecimals(0);
	doublespinbox_edge_->setSingleStep(1);
	doublespinbox_edge_->setValue(4);
	layout->addWidget(doublespinbox_edge_, 2, 1);

	// 空心率
	label_ratio_ = new QLabel(tr("Ratio"));
	layout->addWidget(label_ratio_, 3, 0);

	doublespinbox_ratio_ = new QDoubleSpinBox;
	doublespinbox_ratio_->setMinimum(0);
	doublespinbox_ratio_->setMaximum(0.99);
	doublespinbox_ratio_->setDecimals(2);
	doublespinbox_ratio_->setSingleStep(0.05);
	doublespinbox_ratio_->setValue(0);
	layout->addWidget(doublespinbox_ratio_, 3, 1);

	// 占空比
	QLabel* label_density = new QLabel(tr("Density"));
	layout->addWidget(label_density, 4, 0);

	doublespinbox_density_ = new QDoubleSpinBox;
	doublespinbox_density_->setMinimum(0);
	doublespinbox_density_->setMaximum(150);
	doublespinbox_density_->setDecimals(0);
	doublespinbox_density_->setSingleStep(5);
	doublespinbox_density_->setValue(50);
	doublespinbox_density_->setSuffix(" %");
	layout->addWidget(doublespinbox_density_, 4, 1);

	// 随机度
	QLabel* label_variation = new QLabel(tr("Variation"));
	layout->addWidget(label_variation, 5, 0);

	doublespinbox_variation_ = new QDoubleSpinBox;
	doublespinbox_variation_->setMinimum(0);
	doublespinbox_variation_->setMaximum(200);
	doublespinbox_variation_->setDecimals(0);
	doublespinbox_variation_->setSingleStep(1);
	doublespinbox_variation_->setValue(50);
	doublespinbox_variation_->setSuffix(" %");
	layout->addWidget(doublespinbox_variation_, 5, 1);

	return groupbox;
}



QGroupBox* CentralWidget::createResolutionGroupbox()
{
	QGroupBox* groupbox = new QGroupBox(tr("Resolution"));
	QGridLayout *layout = new QGridLayout;
	groupbox->setLayout(layout);

	// 图像宽度
	QLabel* label_width = new QLabel(tr("Width"));
	layout->addWidget(label_width, 0, 0);

	spinbox_image_width_ = new QSpinBox;
	spinbox_image_width_->setMinimum(10);
	spinbox_image_width_->setMaximum(9999);
	spinbox_image_width_->setValue(500);
	spinbox_image_width_->setSingleStep(20);
	spinbox_image_width_->setSuffix(tr(" Pixel"));
	layout->addWidget(spinbox_image_width_, 0, 1);

	// 图像高度
	QLabel* label_height = new QLabel(tr("Height"));
	layout->addWidget(label_height, 1, 0);

	spinbox_image_height_ = new QSpinBox;
	spinbox_image_height_->setMinimum(10);
	spinbox_image_height_->setMaximum(9999);
	spinbox_image_height_->setValue(500);
	spinbox_image_height_->setSingleStep(20);
	spinbox_image_height_->setSuffix(tr(" Pixel"));
	layout->addWidget(spinbox_image_height_, 1, 1);

	return groupbox;
}



void CentralWidget::setupUI()
{
	// 初始化散斑显示区域
	speckle_view_ = new SpeckleView(this);
	speckle_scene_ = new SpeckleScene;
	speckle_view_->setScene(speckle_scene_);

	// 初始化参数面板
	QVBoxLayout* parameter_panel_layout = new QVBoxLayout;

	parameter_panel_layout->addWidget(createModeGroupbox());
	parameter_panel_layout->addSpacing(24);
	parameter_panel_layout->addWidget(createPatternGroupbox());
	parameter_panel_layout->addWidget(createResolutionGroupbox());
	parameter_panel_layout->addStretch(1);
	parameter_panel_layout->setMargin(0);
	parameter_panel_layout->setSpacing(10);

	// 初始化总体布局
	QHBoxLayout* layout = new QHBoxLayout;
	layout->addLayout(parameter_panel_layout);
	layout->addWidget(speckle_view_);
	setLayout(layout);
}



void CentralWidget::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(combobox_mode_, QOverload<int>::of(&QComboBox::currentIndexChanged),
		this, &CentralWidget::SlotElementTypeChanged);
	is_ok = connect(combobox_antialiasing_, QOverload<int>::of(&QComboBox::currentIndexChanged),
		this, &CentralWidget::SlotAntialiasingTypeChanged);
	is_ok = connect(combobox_background_, QOverload<int>::of(&QComboBox::currentIndexChanged),
		this, [=]() { this->SlotDrawSpecklePattern(); });

	is_ok = connect(doublespinbox_diameter_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { this->SlotDrawSpecklePattern(); });
	is_ok = connect(doublespinbox_eccentricity_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { this->SlotDrawSpecklePattern(); });
	is_ok = connect(doublespinbox_edge_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { this->SlotDrawSpecklePattern(); });
	is_ok = connect(doublespinbox_ratio_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { this->SlotDrawSpecklePattern(); });
	is_ok = connect(doublespinbox_density_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { this->SlotDrawSpecklePattern(); });
	is_ok = connect(doublespinbox_variation_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		this, [=]() { this->SlotDrawSpecklePattern(); });

	is_ok = connect(spinbox_image_width_, QOverload<int>::of(&QSpinBox::valueChanged),
		this, [=]() { this->SlotDrawSpecklePattern(); });
	is_ok = connect(spinbox_image_height_, QOverload<int>::of(&QSpinBox::valueChanged),
		this, [=]() { this->SlotDrawSpecklePattern(); });

}



void CentralWidget::SlotElementTypeChanged(int type_number)
{
	PatternType type = static_cast<PatternType>(type_number);

	if (type == ELLIPSE_PATTERN)
	{
		label_render_->setVisible(true);
		combobox_antialiasing_->setVisible(true);
		label_eccentricity_->setVisible(true);
		doublespinbox_eccentricity_->setVisible(true);
		label_edge_->setVisible(false);
		doublespinbox_edge_->setVisible(false);
		label_ratio_->setVisible(true);
		doublespinbox_ratio_->setVisible(true);
	}
	else if (type == POLYGON_PATTERN)
	{
		label_render_->setVisible(true);
		combobox_antialiasing_->setVisible(true);
		label_eccentricity_->setVisible(false);
		doublespinbox_eccentricity_->setVisible(false);
		label_edge_->setVisible(true);
		doublespinbox_edge_->setVisible(true);
		label_ratio_->setVisible(true);
		doublespinbox_ratio_->setVisible(true);
	}
	else if (type == GAUSSIAN_PATTERN)
	{
		label_render_->setVisible(false);
		combobox_antialiasing_->setVisible(false);
		label_eccentricity_->setVisible(false);
		doublespinbox_eccentricity_->setVisible(false);
		label_edge_->setVisible(false);
		doublespinbox_edge_->setVisible(false);
		label_ratio_->setVisible(false);
		doublespinbox_ratio_->setVisible(false);
	}

	SlotDrawSpecklePattern();
}



void CentralWidget::SlotAntialiasingTypeChanged(int type_number)
{
	AntialisingType type = static_cast<AntialisingType>(type_number);

	if (type == ANTIALISING)
		speckle_view_->setRenderHint(QPainter::Antialiasing, true);
	else 
		speckle_view_->setRenderHint(QPainter::Antialiasing, false);


	SlotDrawSpecklePattern();
}



void CentralWidget::SlotDrawSpecklePattern()
{
	// 设置视图大小
	if (abs(speckle_scene_->height() - spinbox_image_height_->value()) + abs(speckle_scene_->width() - spinbox_image_width_->value()) > 0.5)
		speckle_scene_->setSceneRect(0, 0, spinbox_image_width_->value(), spinbox_image_height_->value());

	// 读取参数
	PatternType element_type = static_cast<PatternType>(combobox_mode_->currentIndex());				// 散斑类型
	BackgroundColor background = static_cast<BackgroundColor>(combobox_background_->currentIndex());

	// 重新绘制散斑图
	if (element_type == ELLIPSE_PATTERN)
	{
		delete ellipse_speckle_pattern_;

		ellipse_speckle_pattern_ = new EllipseSpecklePattern(
			doublespinbox_diameter_->value(),
			0.01 * doublespinbox_density_->value(),
			0.01 * doublespinbox_variation_->value(),
			doublespinbox_eccentricity_->value(),
			doublespinbox_ratio_->value(),
			spinbox_image_height_->value(),
			spinbox_image_width_->value());

		speckle_scene_->PaintEllipseSpecklePattern(ellipse_speckle_pattern_, background);

	}
	else if (element_type == POLYGON_PATTERN)
	{
		delete polygon_speckle_pattern_;

		polygon_speckle_pattern_ = new PolygonSpecklePattern(
			doublespinbox_diameter_->value(),
			0.01 * doublespinbox_density_->value(),
			0.01 * doublespinbox_variation_->value(),
			static_cast<int>(doublespinbox_edge_->value()),
			doublespinbox_ratio_->value(),
			spinbox_image_height_->value(),
			spinbox_image_width_->value());

		speckle_scene_->PaintPolygonSpecklePattern(polygon_speckle_pattern_, background);

	}
	else if (element_type == GAUSSIAN_PATTERN)
	{
		delete gaussian_pattern_;

		gaussian_pattern_ = new GaussianPattern(
			doublespinbox_diameter_->value(),
			0.01 * doublespinbox_density_->value(),
			0.01 * doublespinbox_variation_->value(),
			spinbox_image_height_->value(),
			spinbox_image_width_->value());

		speckle_scene_->PaintGaussianSpecklePattern(gaussian_pattern_, spinbox_image_height_->value(), spinbox_image_width_->value(), background);

	}


}



void CentralWidget::SlotSaveImage()
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

	// 存储
	const int width = spinbox_image_width_->value();
	const int height = spinbox_image_height_->value();

	QPainter::RenderHint render_hint;
	if (static_cast<AntialisingType>(combobox_antialiasing_->currentIndex()) == ANTIALISING)
		render_hint = QPainter::Antialiasing;

	Qt::GlobalColor background_color;
	if (static_cast<BackgroundColor>(combobox_background_->currentIndex()) == BLACK_BACKGROUND)
		background_color = Qt::black;
	else
		background_color = Qt::white;

	// 绘制图片
	QPixmap pixmap;

	PatternType mode = static_cast<PatternType>(combobox_mode_->currentIndex());
	if (mode == ELLIPSE_PATTERN)
	{
		pixmap = RenderPatternPixmap::RenderEllipsePatternPixmap(ellipse_speckle_pattern_, height, width, render_hint, background_color);
	}
	else if (mode == POLYGON_PATTERN)
	{
		pixmap = RenderPatternPixmap::RenderPolygonPatternPixmap(polygon_speckle_pattern_, height, width, render_hint, background_color);
	}
	else
	{
		pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, height, width, background_color);
	}

	pixmap.save(bmp_name, "BMP");

}