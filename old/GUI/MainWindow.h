#pragma once

#ifdef QT_VERSION
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QInputDialog>
#include "../Model/Model.h"
#include "../Application/Application.h"
#include <memory>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void createPresentation();
    void loadPresentation();
    void savePresentation();
    void addSlide();
    void removeSlide();
    void duplicateSlide();
    void moveSlide();
    void addShape();
    void undo();
    void redo();
    void refreshSlideList();
    void onSlideSelected(int index);
    void showPresentation();

private:
    void setupUI();
    void updateSlideList();
    void showError(const QString& message);
    void showSuccess(const QString& message);

    QWidget* centralWidget_;
    QVBoxLayout* mainLayout_;
    QHBoxLayout* buttonLayout_;
    QListWidget* slideList_;
    QTextEdit* slidePreview_;
    QLabel* statusLabel_;

    QMenuBar* menuBar_;
    QMenu* fileMenu_;
    QMenu* editMenu_;
    QMenu* slideMenu_;

    QAction* newAction_;
    QAction* loadAction_;
    QAction* saveAction_;
    QAction* exitAction_;
    QAction* undoAction_;
    QAction* redoAction_;
    QAction* addSlideAction_;
    QAction* removeSlideAction_;
    QAction* duplicateSlideAction_;
    QAction* moveSlideAction_;
};

#endif // QT_VERSION

