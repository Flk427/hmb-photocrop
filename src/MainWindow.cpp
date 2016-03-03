#include <QApplication>
#include <QLabel>
#include <QTranslator>
#include <QTimer>
#include <QFileDialog>
#include <QFileInfo>
#include "ImagePreview.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(const SApplicationParams& params, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_params(params),
	m_acpectInfo(nullptr),
	m_selectionSizeInfo(nullptr),
	m_fileInfo(nullptr)
{
	ui->setupUi(this);

	this->resize(800, 600);

	connect(ui->actionFit, &QAction::triggered, this, &MainWindow::fit);
	connect(ui->actionScaleP10, &QAction::triggered, this, &MainWindow::scaleUp);
	connect(ui->actionScaleM10, &QAction::triggered, this, &MainWindow::scaleDown);
	connect(ui->actionPreviousFile, &QAction::triggered, this, &MainWindow::onPrevFilePressed);
	connect(ui->actionNextFile, &QAction::triggered, this, &MainWindow::onNextFilePressed);

	connect(ui->actionCropImage, &QAction::triggered, this, &MainWindow::onCropAction);
	connect(ui->actionSaveFile, &QAction::triggered, this, &MainWindow::onSavePressed);
	connect(ui->actionDestDir, &QAction::triggered, this, &MainWindow::onDestDirPressed);
	connect(ui->actionOpenFiles, &QAction::triggered, this, &MainWindow::onOpenFilesPressed);
	connect(ui->actionRestoreImage, &QAction::triggered, &m_imageContainer, &ImageContainer::restoreImage);
	connect(ui->actionRotL, &QAction::triggered, &m_imageContainer, &ImageContainer::rotateLeft);
	connect(ui->actionRotR, &QAction::triggered, &m_imageContainer, &ImageContainer::rotateRight);
	connect(ui->actionMirrorV, &QAction::triggered, &m_imageContainer, &ImageContainer::mirrorVertical);
	connect(ui->actionMirrorH, &QAction::triggered, &m_imageContainer, &ImageContainer::mirrorHorisontal);

	connect(&m_imageContainer, &ImageContainer::imageChanged, this, &MainWindow::onImageChanged);
	connect(&m_imageContainer, &ImageContainer::imageLoaded, this, &MainWindow::onImageLoaded);
	connect(&m_imageContainer, &ImageContainer::imageLoadError, this, &MainWindow::onImageLoadError);

	connect(ui->graphicsView, SIGNAL(cropSelectorMoved()), this, SLOT(onCropSelectorMoved()));

	QTimer* setupUITimer = new QTimer(this);
	setupUITimer->setSingleShot(true);
	connect(setupUITimer, &QTimer::timeout, this, &MainWindow::onCreate);
	setupUITimer->start(1);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::fit()
{
	ui->graphicsView->fitInView(m_graphicsScene.itemsBoundingRect(), Qt::KeepAspectRatio);
	shrinkGraphicScene();
}

void MainWindow::scaleUp()
{
	ui->graphicsView->scale(1.1, 1.1);
}

void MainWindow::scaleDown()
{
	ui->graphicsView->scale(0.9, 0.9);
}

void MainWindow::setFileIndex(int fileIndex)
{
	m_fileIndex = fileIndex;

	if (m_fileIndex < 0)
	{
		updateFileInfo("", -1, 0);
	}
	else
	{
		updateFileInfo(m_files.at(m_fileIndex), m_fileIndex, m_files.size());
	}

	ui->actionNextFile->setEnabled(false);
	ui->actionPreviousFile->setEnabled(false);

	if (m_fileIndex > 0)
	{
		ui->actionPreviousFile->setEnabled(true);
	}

	if (m_fileIndex >= 0 && m_fileIndex < m_files.size() - 1)
	{
		ui->actionNextFile->setEnabled(true);
	}
}

void MainWindow::updateFileInfo(const QString& name, int pos, int count)
{
	if (m_fileInfo)
	{
		QFileInfo fileInfo(name);

		m_fileInfo->setText(tr("File:")
							+ QString(" %1 (%2) \"%3\"").arg(pos+1).arg(count).arg(fileInfo.fileName()));
	}
}

void MainWindow::fillAspects()
{
	m_acpectsContainer.append(1, 1);
	m_acpectsContainer.append(4, 3);
	m_acpectsContainer.append(5, 4);
	m_acpectsContainer.append(16, 9);
	m_acpectsContainer.append(16, 10);

	m_aspectSelector = new QComboBox();
	m_aspectSelector->addItem(tr("none"), -1);

	for (int i = 0; i < m_acpectsContainer.size(); ++i)
	{
		AspectInfo::SASPECT aspect = m_acpectsContainer.getAspect(i);
		m_aspectSelector->addItem(QString("%1:%2").arg(aspect.w).arg(aspect.h), i);
	}
}

void MainWindow::setupLanguage()
{
	QTranslator translator;

	if (m_params.language == "ru")
	{
		translator.load(":/i18n/hmb-photocrop_ru.qm");
	}

	QApplication::installTranslator(&translator);
	ui->retranslateUi(this);
}

void MainWindow::setupToolBar()
{
	QLabel* aspectSelectorLabel = new QLabel(tr("Selector:"));
	aspectSelectorLabel->setContentsMargins(5, 0, 5, 0);
	ui->toolBar->addWidget(aspectSelectorLabel);
	ui->toolBar->addWidget(m_aspectSelector);

	m_ribbonOrientationSelector = new QComboBox();
	m_ribbonOrientationSelector->addItem(tr("Portrait"));
	m_ribbonOrientationSelector->addItem(tr("Landscape"));
	m_ribbonOrientationSelector->setEnabled(false);
	ui->toolBar->addWidget(m_ribbonOrientationSelector);
}

void MainWindow::setupGraphicScene()
{
	QPixmap tempPixmap = QPixmap::fromImage(m_imageContainer.m_image);
	m_sceneImageItem = m_graphicsScene.addPixmap(tempPixmap);

	m_cropSelector = new GraphicsRibbonItem();
	m_graphicsScene.addItem(m_cropSelector);
	m_cropSelector->setPos(50, 50);
	m_cropSelector->setSize(300, 400);

	ui->graphicsView->setScene(&m_graphicsScene);
	fit();
}

void MainWindow::setupStatusBar()
{
	// TODO:
	m_acpectInfo = new QLabel(tr("Aspect ratio:") + "-");
	m_acpectInfo->setContentsMargins(5, 0, 5, 0);
	ui->statusBar->addWidget(m_acpectInfo);

	m_selectionSizeInfo = new QLabel(tr("Selection size:") + QString(" %1x%2").arg(m_cropSelector->width()).arg(m_cropSelector->height()));
	m_selectionSizeInfo->setContentsMargins(5, 0, 5, 0);
	ui->statusBar->addWidget(m_selectionSizeInfo);

	m_fileInfo = new QLabel();
	updateFileInfo("", -1, 0);
	m_fileInfo->setContentsMargins(5, 0, 5, 0);
	ui->statusBar->addWidget(m_fileInfo);
}

void MainWindow::setEditControlsEnable(bool enabled)
{
	ui->actionCropImage->setEnabled(enabled);

	if (!enabled)
	{
		ui->actionRestoreImage->setEnabled(enabled);
	}

	ui->actionSaveFile->setEnabled(enabled);
	ui->actionFadeMask->setEnabled(enabled);
	ui->actionMirrorH->setEnabled(enabled);
	ui->actionMirrorV->setEnabled(enabled);
	ui->actionRotL->setEnabled(enabled);
	ui->actionRotR->setEnabled(enabled);
}

bool MainWindow::loadFile(int index)
{
	if (index < m_files.size())
	{
		return m_imageContainer.loadImage(m_files.at(index));
	}

	return false;
}

void MainWindow::shrinkGraphicScene()
{
	//http://www.qtcentre.org/threads/54043-how-to-make-the-QGraphicsScene-resize-automaticly
	//Calculates and returns the bounding rect of all items on the scene.
	//This function works by iterating over all items, and because if this, it can be slow for large scenes.
	QRectF rect = m_graphicsScene.itemsBoundingRect();
	m_graphicsScene.setSceneRect(rect);
}

void MainWindow::onImageChanged()
{
	m_sceneImageItem->setPixmap(QPixmap::fromImage(m_imageContainer.m_image));
	fit();

	// ui->actionSaveFile->setEnabled(m_imageContainer.isImageChanged());
	ui->actionRestoreImage->setEnabled(m_imageContainer.isImageChanged());
}

void MainWindow::onCreate()
{
	// TODO: брать из настроек
	m_saveDir = QApplication::applicationDirPath();
	m_filesDir = QApplication::applicationDirPath();

	setFileIndex(-1);

	setEditControlsEnable(false);

	setupLanguage();
	fillAspects();
	setupToolBar();
	setupGraphicScene();

	if (!m_params.sourceImage.isEmpty())
	{
		m_imageContainer.loadImage(m_params.sourceImage);
	}

	setupStatusBar();

	connect(m_aspectSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectorAspectParamsChanged(int)));
	connect(m_ribbonOrientationSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectorAspectParamsChanged(int)));
}

void MainWindow::onCropAction()
{
	m_imageContainer.cropImage(
				QRect(m_cropSelector->x(),
					  m_cropSelector->y(),
					  m_cropSelector->width(),
					  m_cropSelector->height()));

	m_cropSelector->setPos(0, 0);
	fit();
}

void MainWindow::onSavePressed()
{
	QString fileName = m_saveDir + QDir::separator() + QFileInfo(m_files.at(m_fileIndex)).fileName();
	QFile file(fileName);

	if (file.exists())
	{
		fileName = QFileDialog::getSaveFileName(
							   this,
							   tr("Save file"),
							   fileName,
							   "Portable network graphic (*.png);;JPEG (*.jpg)",
							   0, 0);

		if (fileName.isEmpty())
		{
			return;
		}
	}

	m_imageContainer.saveImage(fileName);
	onNextFilePressed();
}

void MainWindow::onDestDirPressed()
{
	QString dirName = QFileDialog::getExistingDirectory(
						  this,
						  tr("Choose destination directory..."),
						  m_saveDir,
						  QFileDialog::ShowDirsOnly);

	if (!dirName.isEmpty())
	{
		m_saveDir = dirName;
	}
}

void MainWindow::onImageLoaded()
{
	setEditControlsEnable(true);
}

void MainWindow::onImageLoadError()
{
	setEditControlsEnable(false);
}

void MainWindow::onOpenFilesPressed()
{
	m_files = QFileDialog::getOpenFileNames(
				  this,
				  tr("Select files..."),
				  m_filesDir,
				  "Portable network graphic (*.png);;JPEG (*.jpg)", 0, 0);

	if (m_files.size() != 0)
	{
		m_filesDir = QFileInfo(m_files.at(0)).absolutePath();

		if (loadFile(0))
		{
			setFileIndex(0);
		}
		else
		{
			// Не удалось загрузить файл - сброс списка.
			m_files.clear();
			setFileIndex(-1);
		}
	}
	else
	{
		// Используем старый список.
	}
}

void MainWindow::onPrevFilePressed()
{
	if (m_fileIndex > 0)
	{
		setFileIndex(m_fileIndex - 1);
		loadFile(m_fileIndex);
	}
}

void MainWindow::onNextFilePressed()
{
	if (m_fileIndex < m_files.size() - 1)
	{
		setFileIndex(m_fileIndex + 1);
		loadFile(m_fileIndex);
	}
}

void MainWindow::onSelectorAspectParamsChanged(int index)
{
	int aspectIndex = m_aspectSelector->currentData().toInt();

	if (aspectIndex >= 0)
	{
		if (!m_ribbonOrientationSelector->isEnabled())
		{
			m_ribbonOrientationSelector->setEnabled(true);
		}

		AspectInfo::SASPECT aspectInfo = m_acpectsContainer.getAspect(aspectIndex);

		bool isPortrait = m_ribbonOrientationSelector->currentIndex() == 0;
		m_cropSelector->setAspect(aspectInfo.w, aspectInfo.h, isPortrait);
		m_cropSelector->setFixedAspect(true);
	}
	else
	{
		m_ribbonOrientationSelector->setEnabled(false);
		m_cropSelector->setFixedAspect(false);
	}

	m_graphicsScene.update();
}

void MainWindow::onCropSelectorMoved()
{
	// Обновить превью.

	ui->labelPreview->updatePreview(m_imageContainer.m_image,
									QRect(m_cropSelector->x(),
										  m_cropSelector->y(),
										  m_cropSelector->width(),
										  m_cropSelector->height()));

	m_selectionSizeInfo->setText(tr("Selection size:")
								 + QString(" %1x%2")
								 .arg((int) m_cropSelector->width())
								 .arg((int) m_cropSelector->height()));

	m_acpectInfo->setText(tr("Aspect ratio:") + QString(" %1").arg(m_cropSelector->width() / m_cropSelector->height()));
}
