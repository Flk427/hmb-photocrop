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
#include "ImagesList.h"

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
	void onSelectorAspectParamsChanged();
	void onCropSelectorMoved();
	void changeImage(QString fileName, bool isFirst, bool isLast);

private:
	SApplicationParams m_params;
	Ui::MainWindow *ui;
	ImageContainer m_imageContainer;
	GraphicsRibbonItem* m_cropSelector;
	QGraphicsScene m_graphicsScene;
	AspectInfo m_acpectsContainer;
	ImagesList* m_imagesList;

	QGraphicsPixmapItem* m_sceneImageItem;
	QComboBox* m_aspectSelector;
    QComboBox* m_formatSelector;
	QComboBox* m_ribbonOrientationSelector;

	QString m_saveDir;
	QLabel* m_acpectInfo;
	QLabel* m_selectionSizeInfo;
	QLabel* m_fileInfo;

	int m_imageIndex;
	QString m_language;

	void readSettings();
	void setupUiConnections();
	void setupLanguage();
	//! Adds aspect ratio selector and orientation selector to the toolbar.
	void setupToolBar();
	void setupGraphicScene();
	void setupStatusBar();
	void setEditControlsEnable(bool enabled);
	void applyParams();
	void updateFileInfo(const QString& name, int pos, int count);
	void createAspectSelector();
    void createFormatSelector();

	// QWidget interface
protected:
	virtual void closeEvent(QCloseEvent*event);
};

#endif // MAINWINDOW_H
