#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QComboBox>
#include <QLabel>
#include <QStringList>
#include "AspectInfo.h"
#include "ImageContainer.h"
#include "GraphicsRibbonItem.h"
#include "ApplicationParams.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(const SApplicationParams& params, QWidget *parent = 0);
	~MainWindow();

public slots:
	void fit();
	void scaleUp();
	void scaleDown();
	void shrinkGraphicScene();
	void onImageChanged();
	void onCreate();
	void onCropAction();
	void onSavePressed();
	void onDestDirPressed();
	void onImageLoaded();
	void onImageLoadError();
	void onOpenFilesPressed();
	void onPrevFilePressed();
	void onNextFilePressed();
	void onSelectorAspectParamsChanged(int index);
	void onCropSelectorMoved();

private:
	SApplicationParams m_params;
	Ui::MainWindow *ui;
	ImageContainer m_imageContainer;
	GraphicsRibbonItem* m_cropSelector;
	QGraphicsScene m_graphicsScene;
	AspectInfo m_acpectsContainer;

	QGraphicsPixmapItem* m_sceneImageItem;
	QComboBox* m_aspectSelector;
	QComboBox* m_ribbonOrientationSelector;

	QString m_saveDir;
	QLabel* m_acpectInfo;
	QLabel* m_selectionSizeInfo;
	QLabel* m_fileInfo;

	QString m_filesDir;
	int m_fileIndex;
	QStringList m_files;

	void setupLanguage();
	void setupToolBar();
	void setupGraphicScene();
	void setupStatusBar();
	void setEditControlsEnable(bool enabled);

	bool loadFile(int index);
	void setFileIndex(int fileIndex);
	void updateFileInfo(const QString& name, int pos, int count);
	void fillAspects();
};

#endif // MAINWINDOW_H
