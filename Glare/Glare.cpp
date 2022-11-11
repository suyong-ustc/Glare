#include "Glare.h"
#include <QDesktopServices>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGraphicsView>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QToolBar>
#include <QUrl>
#include "XianType.h"



Glare::Glare(QWidget *parent)
    : QMainWindow(parent)
{
	// 软件名称
	setWindowTitle(tr("Glare"));

	// 核心部件
	pattern_widget_ = new CentralWidget(this);
	setCentralWidget(pattern_widget_);

	// 对话框
	assess_dialog_ = new AssessDialog(this);
	recommand_dialog_ = new RecommendDialog(this);
	transform_dialog_ = new TransformDialog(this);

	// 菜单栏
	setupMenuBar();

	ConnectSlots();
}





void Glare::setupMenuBar()
{
	// 菜单：文件
	QMenu *menu_file = menuBar()->addMenu(tr("File"));

	action_save_image_ = menu_file->addAction(tr("Save image..."));
	menu_file->addSeparator();
	action_quit_ = menu_file->addAction(tr("Quit"), this, &QWidget::close);

	// 菜单：生成变形图
	QMenu *menu_transform = menuBar()->addMenu(tr("Transform"));

	action_transform_translation_ = menu_transform->addAction(tr("Translation"));
	action_transform_elongation_ = menu_transform->addAction(tr("Stretch"));
	action_transform_sinusoidal_ = menu_transform->addAction(tr("Sinusoidal"));
	action_transform_gaussian_ = menu_transform->addAction(tr("Gaussian"));
	action_transform_plc_band_ = menu_transform->addAction(tr("PLC Band"));
	menu_transform->addSeparator();
	action_transform_rotation_ = menu_transform->addAction(tr("Rotation"));
	action_transform_2D_sinusoidal_ = menu_transform->addAction(tr("2D Sinusoidal"));
	action_transform_2D_gaussian_ = menu_transform->addAction(tr("2D Gaussian"));

	// 菜单：图像质量评价
	QMenu *menu_assess = menuBar()->addMenu(tr("Assess"));

	action_assess_coverage_ = menu_assess->addAction(tr("Speckle coverage"));
	action_assess_autocorrelation_ = menu_assess->addAction(tr("Speckle size"));
	action_assess_bias_ = menu_assess->addAction(tr("Systematic error"));
	action_assess_noise_ = menu_assess->addAction(tr("Random error"));

	// 菜单：推荐散斑图
	QMenu *menu_recommand = menuBar()->addMenu(tr("Recommand"));

	action_recommand_ = menu_recommand->addAction(tr("Recommand"));

	// 菜单：关于本软件
	QMenu *menu_about = menuBar()->addMenu(tr("About"));

	action_manual_ = menu_about->addAction(tr("Manual"));
	action_about_glare_ = menu_about->addAction(tr("About Glare"));
}



void Glare::ConnectSlots()
{
	bool is_ok = false;

	is_ok = connect(action_save_image_, &QAction::triggered, pattern_widget_, &CentralWidget::SlotSaveImage);

	is_ok = connect(action_transform_translation_, &QAction::triggered, this, &Glare::SlotTransform_Translation);
	is_ok = connect(action_transform_elongation_, &QAction::triggered, this, &Glare::SlotTransform_Elongation);
	is_ok = connect(action_transform_sinusoidal_, &QAction::triggered, this, &Glare::SlotTransform_Sinusoidal);
	is_ok = connect(action_transform_gaussian_, &QAction::triggered, this, &Glare::SlotTransform_Gaussian);
	is_ok = connect(action_transform_plc_band_, &QAction::triggered, this, &Glare::SlotTransform_PLC);
	is_ok = connect(action_transform_rotation_, &QAction::triggered, this, &Glare::SlotTransform_Rotation);

	is_ok = connect(action_assess_coverage_, &QAction::triggered, this, &Glare::SlotAssess_Coverage);
	is_ok = connect(action_assess_autocorrelation_, &QAction::triggered, this, &Glare::SlotAssess_Autocorrelation);
	is_ok = connect(action_assess_bias_, &QAction::triggered, this, &Glare::SlotAssess_Bias);
	is_ok = connect(action_assess_noise_, &QAction::triggered, this, &Glare::SlotAssess_Noise);

	is_ok = connect(action_recommand_, &QAction::triggered, recommand_dialog_, &QDialog::open);

	is_ok = connect(action_manual_, &QAction::triggered, this, &Glare::SlotOpenManual);
	is_ok = connect(action_about_glare_, &QAction::triggered, this, &Glare::SlotAboutGlare);
}



void Glare::SlotTransform_Translation()
{
	transform_dialog_->setDeformationType(TRANSLATION_DEFORMATION);
	transform_dialog_->open();
}



void Glare::SlotTransform_Elongation()
{
	transform_dialog_->setDeformationType(ELONGATION_DEFORMATION);
	transform_dialog_->open();
}



void Glare::SlotTransform_Sinusoidal()
{
	transform_dialog_->setDeformationType(SINUSOIDAL_DEFORMATION);
	transform_dialog_->open();
}



void Glare::SlotTransform_Gaussian()
{
	transform_dialog_->setDeformationType(GAUSSIAN_DEFORMATION);
	transform_dialog_->open();
}



void Glare::SlotTransform_PLC()
{
	transform_dialog_->setDeformationType(PLC_BAND_DEFORMATION);
	transform_dialog_->open();
}


void Glare::SlotTransform_Rotation()
{
	transform_dialog_->setDeformationType(ROTATION_DEFORMATION);
	transform_dialog_->open();
}


void Glare::SlotTransform_2DSinusoidal()
{
	transform_dialog_->setDeformationType(TWO_DIMENSIONAL_SINUSOIDAL_DEFORMATION);
	transform_dialog_->open();
}


void Glare::SlotTransform_2DGaussian()
{
	transform_dialog_->setDeformationType(TWO_DIMENSIONAL_GAUSSIAN_DEFORMATION);
	transform_dialog_->open();
}


void Glare::SlotAssess_Coverage()
{
	assess_dialog_->setAssessType(ASSESS_COVERAGE);
	assess_dialog_->open();
}



void Glare::SlotAssess_Autocorrelation()
{
	assess_dialog_->setAssessType(ASSESS_AUTOCORRELATION);
	assess_dialog_->open();
}



void Glare::SlotAssess_Bias()
{
	assess_dialog_->setAssessType(ASSESS_BIAS);
	assess_dialog_->open();
}



void Glare::SlotAssess_Noise()
{
	assess_dialog_->setAssessType(ASSESS_NOISE);
	assess_dialog_->open();
}



void Glare::SlotOpenManual()
{
	const QUrl url("http://staff.ustc.edu.cn/~suyong/xian.html");
	QDesktopServices::openUrl(url);
}



void Glare::SlotAboutGlare()
{
	QMessageBox::about(this, tr("About Glare"),
		tr("<p> GLARE is a free and open-source software for digital speckle pattern generation and assessment. "
			"It can generate speckle patterns, render deformed images, assess pattern quality, and present pattern recommendations. "
			"It is developed for academic research, engineering applications, and teaching in the field of digital image correlation. </p>"
			"<p> I would like to dedicate this software to my Alma mater, the University of Science and Technology of China, to thank for its education and cultivation.</p>"
			"<p>Yong Su<br/>"
			"Department of Modern Mechanics<br/>"
			"University of Science and Technology of China<br/>"
			"November, 21th, 2021</p>"));
}