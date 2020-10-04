/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad;
    QAction *actionClose;
    QAction *actionOpen_Folder;
    QAction *actionSettings;
    QAction *actionNewCurrent;
    QAction *actionNewNew;
    QAction *actionSaveClose;
    QAction *actionMove;
    QAction *actionSetting;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QFrame *idFrame;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *labelBookID;
    QLabel *label_3;
    QLabel *labelBatchID;
    QLabel *label_24;
    QDoubleSpinBox *spinCost;
    QComboBox *comboMode;
    QScrollArea *scrollAreaDescription;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_5;
    QFrame *discriptionFrame;
    QGridLayout *gridLayout;
    QFrame *frame_10;
    QHBoxLayout *horizontalLayout_11;
    QPushButton *pushButtonGenStoreDiscription;
    QPushButton *buttonCostBreakdown;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_13;
    QDoubleSpinBox *spinSpineDim;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_4;
    QPlainTextEdit *editExtra;
    QFrame *frame_9;
    QGridLayout *gridLayout_6;
    QLabel *label_10;
    QDoubleSpinBox *spinExtra;
    QLabel *label_12;
    QLineEdit *editCoverMaterial;
    QLabel *label_21;
    QLineEdit *editEndPageColor;
    QLabel *label_9;
    QLineEdit *editThreadColor;
    QLineEdit *editPageMaterial;
    QLabel *label_19;
    QFrame *frame_4;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_22;
    QComboBox *comboBookType;
    QLineEdit *editSection;
    QFrame *frame_5;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_16;
    QDoubleSpinBox *spinWeight;
    QLabel *label_4;
    QFrame *frame_2;
    QGridLayout *gridLayout_3;
    QDoubleSpinBox *spinPageDimY;
    QLabel *label_8;
    QLabel *label_17;
    QLabel *label_5;
    QLabel *label_15;
    QLabel *label_7;
    QDoubleSpinBox *spinPageDimX;
    QLabel *label_6;
    QDoubleSpinBox *spinCoverDimY;
    QDoubleSpinBox *spinCoverDimX;
    QFrame *frame_11;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QComboBox *comboStatus;
    QFrame *frame_7;
    QGridLayout *gridLayout_2;
    QSpinBox *spinPagesPerSig;
    QLabel *label_14;
    QLabel *label_20;
    QLabel *label_11;
    QSpinBox *spinSignitures;
    QSpinBox *spinPages;
    QLabel *label_23;
    QLineEdit *editBox;
    QMenuBar *menubar;
    QMenu *menuOptions;
    QMenu *menuNew_Entry;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->resize(870, 840);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/main.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8("#centralwidget {\n"
"	color: rgb(239, 41, 41);\n"
"}"));
        MainWindow->setDocumentMode(false);
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName(QString::fromUtf8("actionLoad"));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionOpen_Folder = new QAction(MainWindow);
        actionOpen_Folder->setObjectName(QString::fromUtf8("actionOpen_Folder"));
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        actionNewCurrent = new QAction(MainWindow);
        actionNewCurrent->setObjectName(QString::fromUtf8("actionNewCurrent"));
        actionNewNew = new QAction(MainWindow);
        actionNewNew->setObjectName(QString::fromUtf8("actionNewNew"));
        actionSaveClose = new QAction(MainWindow);
        actionSaveClose->setObjectName(QString::fromUtf8("actionSaveClose"));
        actionMove = new QAction(MainWindow);
        actionMove->setObjectName(QString::fromUtf8("actionMove"));
        actionSetting = new QAction(MainWindow);
        actionSetting->setObjectName(QString::fromUtf8("actionSetting"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setStyleSheet(QString::fromUtf8("* {\n"
"	font: 12pt \"Arial\"\n"
"}"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        idFrame = new QFrame(centralwidget);
        idFrame->setObjectName(QString::fromUtf8("idFrame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(idFrame->sizePolicy().hasHeightForWidth());
        idFrame->setSizePolicy(sizePolicy);
        idFrame->setFrameShape(QFrame::StyledPanel);
        idFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(idFrame);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(idFrame);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        label->setFont(font);

        horizontalLayout_2->addWidget(label);

        labelBookID = new QLabel(idFrame);
        labelBookID->setObjectName(QString::fromUtf8("labelBookID"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(labelBookID->sizePolicy().hasHeightForWidth());
        labelBookID->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(labelBookID);

        label_3 = new QLabel(idFrame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setFont(font);
        label_3->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_2->addWidget(label_3);

        labelBatchID = new QLabel(idFrame);
        labelBatchID->setObjectName(QString::fromUtf8("labelBatchID"));
        sizePolicy2.setHeightForWidth(labelBatchID->sizePolicy().hasHeightForWidth());
        labelBatchID->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(labelBatchID);

        label_24 = new QLabel(idFrame);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        horizontalLayout_2->addWidget(label_24);

        spinCost = new QDoubleSpinBox(idFrame);
        spinCost->setObjectName(QString::fromUtf8("spinCost"));
        spinCost->setReadOnly(true);
        spinCost->setButtonSymbols(QAbstractSpinBox::NoButtons);

        horizontalLayout_2->addWidget(spinCost);

        comboMode = new QComboBox(idFrame);
        comboMode->addItem(QString());
        comboMode->addItem(QString());
        comboMode->setObjectName(QString::fromUtf8("comboMode"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(comboMode->sizePolicy().hasHeightForWidth());
        comboMode->setSizePolicy(sizePolicy3);
        comboMode->setFont(font);
        comboMode->setStyleSheet(QString::fromUtf8("#comboMode {\n"
"	background-color: rgb(186, 189, 182);\n"
"}"));

        horizontalLayout_2->addWidget(comboMode);


        verticalLayout->addWidget(idFrame);

        scrollAreaDescription = new QScrollArea(centralwidget);
        scrollAreaDescription->setObjectName(QString::fromUtf8("scrollAreaDescription"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(scrollAreaDescription->sizePolicy().hasHeightForWidth());
        scrollAreaDescription->setSizePolicy(sizePolicy4);
        scrollAreaDescription->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 828, 888));
        gridLayout_5 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        discriptionFrame = new QFrame(scrollAreaWidgetContents);
        discriptionFrame->setObjectName(QString::fromUtf8("discriptionFrame"));
        discriptionFrame->setFrameShape(QFrame::StyledPanel);
        discriptionFrame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(discriptionFrame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frame_10 = new QFrame(discriptionFrame);
        frame_10->setObjectName(QString::fromUtf8("frame_10"));
        QSizePolicy sizePolicy5(QSizePolicy::Ignored, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(frame_10->sizePolicy().hasHeightForWidth());
        frame_10->setSizePolicy(sizePolicy5);
        frame_10->setFrameShape(QFrame::StyledPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        horizontalLayout_11 = new QHBoxLayout(frame_10);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        pushButtonGenStoreDiscription = new QPushButton(frame_10);
        pushButtonGenStoreDiscription->setObjectName(QString::fromUtf8("pushButtonGenStoreDiscription"));

        horizontalLayout_11->addWidget(pushButtonGenStoreDiscription);

        buttonCostBreakdown = new QPushButton(frame_10);
        buttonCostBreakdown->setObjectName(QString::fromUtf8("buttonCostBreakdown"));

        horizontalLayout_11->addWidget(buttonCostBreakdown);


        gridLayout->addWidget(frame_10, 17, 0, 1, 2);

        frame_6 = new QFrame(discriptionFrame);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        sizePolicy5.setHeightForWidth(frame_6->sizePolicy().hasHeightForWidth());
        frame_6->setSizePolicy(sizePolicy5);
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout_6 = new QHBoxLayout(frame_6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_13 = new QLabel(frame_6);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        sizePolicy2.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy2);
        label_13->setFont(font);

        horizontalLayout_6->addWidget(label_13);

        spinSpineDim = new QDoubleSpinBox(frame_6);
        spinSpineDim->setObjectName(QString::fromUtf8("spinSpineDim"));
        QSizePolicy sizePolicy6(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(spinSpineDim->sizePolicy().hasHeightForWidth());
        spinSpineDim->setSizePolicy(sizePolicy6);

        horizontalLayout_6->addWidget(spinSpineDim);


        gridLayout->addWidget(frame_6, 12, 1, 1, 1);

        groupBox_9 = new QGroupBox(discriptionFrame);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        QSizePolicy sizePolicy7(QSizePolicy::Ignored, QSizePolicy::Maximum);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(groupBox_9->sizePolicy().hasHeightForWidth());
        groupBox_9->setSizePolicy(sizePolicy7);
        gridLayout_4 = new QGridLayout(groupBox_9);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        editExtra = new QPlainTextEdit(groupBox_9);
        editExtra->setObjectName(QString::fromUtf8("editExtra"));
        editExtra->setFont(font);

        gridLayout_4->addWidget(editExtra, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox_9, 19, 0, 1, 2);

        frame_9 = new QFrame(discriptionFrame);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        QSizePolicy sizePolicy8(QSizePolicy::Ignored, QSizePolicy::MinimumExpanding);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(frame_9->sizePolicy().hasHeightForWidth());
        frame_9->setSizePolicy(sizePolicy8);
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        gridLayout_6 = new QGridLayout(frame_9);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label_10 = new QLabel(frame_9);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        sizePolicy2.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy2);

        gridLayout_6->addWidget(label_10, 0, 0, 1, 1);

        spinExtra = new QDoubleSpinBox(frame_9);
        spinExtra->setObjectName(QString::fromUtf8("spinExtra"));
        sizePolicy6.setHeightForWidth(spinExtra->sizePolicy().hasHeightForWidth());
        spinExtra->setSizePolicy(sizePolicy6);
        spinExtra->setSingleStep(0.250000000000000);

        gridLayout_6->addWidget(spinExtra, 0, 1, 1, 1);


        gridLayout->addWidget(frame_9, 14, 1, 1, 1);

        label_12 = new QLabel(discriptionFrame);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        sizePolicy5.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy5);
        label_12->setFont(font);

        gridLayout->addWidget(label_12, 2, 0, 1, 1);

        editCoverMaterial = new QLineEdit(discriptionFrame);
        editCoverMaterial->setObjectName(QString::fromUtf8("editCoverMaterial"));
        sizePolicy6.setHeightForWidth(editCoverMaterial->sizePolicy().hasHeightForWidth());
        editCoverMaterial->setSizePolicy(sizePolicy6);

        gridLayout->addWidget(editCoverMaterial, 1, 1, 1, 1);

        label_21 = new QLabel(discriptionFrame);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        sizePolicy5.setHeightForWidth(label_21->sizePolicy().hasHeightForWidth());
        label_21->setSizePolicy(sizePolicy5);
        label_21->setFont(font);

        gridLayout->addWidget(label_21, 4, 0, 1, 1);

        editEndPageColor = new QLineEdit(discriptionFrame);
        editEndPageColor->setObjectName(QString::fromUtf8("editEndPageColor"));
        sizePolicy6.setHeightForWidth(editEndPageColor->sizePolicy().hasHeightForWidth());
        editEndPageColor->setSizePolicy(sizePolicy6);

        gridLayout->addWidget(editEndPageColor, 4, 1, 1, 1);

        label_9 = new QLabel(discriptionFrame);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy5.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy5);
        label_9->setFont(font);

        gridLayout->addWidget(label_9, 0, 0, 1, 1);

        editThreadColor = new QLineEdit(discriptionFrame);
        editThreadColor->setObjectName(QString::fromUtf8("editThreadColor"));
        sizePolicy6.setHeightForWidth(editThreadColor->sizePolicy().hasHeightForWidth());
        editThreadColor->setSizePolicy(sizePolicy6);

        gridLayout->addWidget(editThreadColor, 2, 1, 1, 1);

        editPageMaterial = new QLineEdit(discriptionFrame);
        editPageMaterial->setObjectName(QString::fromUtf8("editPageMaterial"));
        sizePolicy6.setHeightForWidth(editPageMaterial->sizePolicy().hasHeightForWidth());
        editPageMaterial->setSizePolicy(sizePolicy6);

        gridLayout->addWidget(editPageMaterial, 0, 1, 1, 1);

        label_19 = new QLabel(discriptionFrame);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        sizePolicy5.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy5);
        label_19->setFont(font);

        gridLayout->addWidget(label_19, 9, 0, 1, 1);

        frame_4 = new QFrame(discriptionFrame);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        sizePolicy2.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy2);
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        horizontalLayout_10 = new QHBoxLayout(frame_4);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_22 = new QLabel(frame_4);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setFont(font);

        horizontalLayout_10->addWidget(label_22);

        comboBookType = new QComboBox(frame_4);
        comboBookType->addItem(QString());
        comboBookType->addItem(QString());
        comboBookType->addItem(QString());
        comboBookType->addItem(QString());
        comboBookType->addItem(QString());
        comboBookType->addItem(QString());
        comboBookType->setObjectName(QString::fromUtf8("comboBookType"));

        horizontalLayout_10->addWidget(comboBookType);


        gridLayout->addWidget(frame_4, 16, 1, 1, 1);

        editSection = new QLineEdit(discriptionFrame);
        editSection->setObjectName(QString::fromUtf8("editSection"));
        sizePolicy6.setHeightForWidth(editSection->sizePolicy().hasHeightForWidth());
        editSection->setSizePolicy(sizePolicy6);

        gridLayout->addWidget(editSection, 9, 1, 1, 1);

        frame_5 = new QFrame(discriptionFrame);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        sizePolicy5.setHeightForWidth(frame_5->sizePolicy().hasHeightForWidth());
        frame_5->setSizePolicy(sizePolicy5);
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        horizontalLayout_7 = new QHBoxLayout(frame_5);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_16 = new QLabel(frame_5);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        sizePolicy2.setHeightForWidth(label_16->sizePolicy().hasHeightForWidth());
        label_16->setSizePolicy(sizePolicy2);
        label_16->setFont(font);

        horizontalLayout_7->addWidget(label_16);

        spinWeight = new QDoubleSpinBox(frame_5);
        spinWeight->setObjectName(QString::fromUtf8("spinWeight"));
        sizePolicy6.setHeightForWidth(spinWeight->sizePolicy().hasHeightForWidth());
        spinWeight->setSizePolicy(sizePolicy6);

        horizontalLayout_7->addWidget(spinWeight);


        gridLayout->addWidget(frame_5, 13, 1, 1, 1);

        label_4 = new QLabel(discriptionFrame);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy5.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy5);
        label_4->setFont(font);

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        frame_2 = new QFrame(discriptionFrame);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        sizePolicy2.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy2);
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        spinPageDimY = new QDoubleSpinBox(frame_2);
        spinPageDimY->setObjectName(QString::fromUtf8("spinPageDimY"));

        gridLayout_3->addWidget(spinPageDimY, 1, 4, 1, 1);

        label_8 = new QLabel(frame_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy5.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy5);
        label_8->setFont(font);

        gridLayout_3->addWidget(label_8, 1, 0, 1, 1);

        label_17 = new QLabel(frame_2);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_3->addWidget(label_17, 0, 4, 1, 1);

        label_5 = new QLabel(frame_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy5.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy5);
        label_5->setFont(font);
        label_5->setStyleSheet(QString::fromUtf8("#label_5 {\n"
"	\n"
"}"));

        gridLayout_3->addWidget(label_5, 2, 0, 1, 1);

        label_15 = new QLabel(frame_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_3->addWidget(label_15, 0, 2, 1, 1);

        label_7 = new QLabel(frame_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QSizePolicy sizePolicy9(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy9);

        gridLayout_3->addWidget(label_7, 1, 3, 1, 1);

        spinPageDimX = new QDoubleSpinBox(frame_2);
        spinPageDimX->setObjectName(QString::fromUtf8("spinPageDimX"));

        gridLayout_3->addWidget(spinPageDimX, 1, 2, 1, 1);

        label_6 = new QLabel(frame_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_3->addWidget(label_6, 2, 3, 1, 1);

        spinCoverDimY = new QDoubleSpinBox(frame_2);
        spinCoverDimY->setObjectName(QString::fromUtf8("spinCoverDimY"));

        gridLayout_3->addWidget(spinCoverDimY, 2, 4, 1, 1);

        spinCoverDimX = new QDoubleSpinBox(frame_2);
        spinCoverDimX->setObjectName(QString::fromUtf8("spinCoverDimX"));
        QSizePolicy sizePolicy10(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy10.setHorizontalStretch(0);
        sizePolicy10.setVerticalStretch(0);
        sizePolicy10.setHeightForWidth(spinCoverDimX->sizePolicy().hasHeightForWidth());
        spinCoverDimX->setSizePolicy(sizePolicy10);

        gridLayout_3->addWidget(spinCoverDimX, 2, 2, 1, 1);


        gridLayout->addWidget(frame_2, 11, 0, 1, 2);

        frame_11 = new QFrame(discriptionFrame);
        frame_11->setObjectName(QString::fromUtf8("frame_11"));
        frame_11->setFrameShape(QFrame::StyledPanel);
        frame_11->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(frame_11);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        comboStatus = new QComboBox(frame_11);
        comboStatus->addItem(QString());
        comboStatus->addItem(QString());
        comboStatus->addItem(QString());
        comboStatus->addItem(QString());
        comboStatus->addItem(QString());
        comboStatus->setObjectName(QString::fromUtf8("comboStatus"));

        horizontalLayout->addWidget(comboStatus);


        gridLayout->addWidget(frame_11, 16, 0, 1, 1);

        frame_7 = new QFrame(discriptionFrame);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        sizePolicy2.setHeightForWidth(frame_7->sizePolicy().hasHeightForWidth());
        frame_7->setSizePolicy(sizePolicy2);
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_7);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        spinPagesPerSig = new QSpinBox(frame_7);
        spinPagesPerSig->setObjectName(QString::fromUtf8("spinPagesPerSig"));
        sizePolicy3.setHeightForWidth(spinPagesPerSig->sizePolicy().hasHeightForWidth());
        spinPagesPerSig->setSizePolicy(sizePolicy3);

        gridLayout_2->addWidget(spinPagesPerSig, 2, 1, 1, 1);

        label_14 = new QLabel(frame_7);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        sizePolicy10.setHeightForWidth(label_14->sizePolicy().hasHeightForWidth());
        label_14->setSizePolicy(sizePolicy10);
        label_14->setFont(font);

        gridLayout_2->addWidget(label_14, 2, 0, 1, 1);

        label_20 = new QLabel(frame_7);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setFont(font);

        gridLayout_2->addWidget(label_20, 0, 0, 1, 1);

        label_11 = new QLabel(frame_7);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy10.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy10);
        label_11->setFont(font);

        gridLayout_2->addWidget(label_11, 3, 0, 1, 1);

        spinSignitures = new QSpinBox(frame_7);
        spinSignitures->setObjectName(QString::fromUtf8("spinSignitures"));
        sizePolicy3.setHeightForWidth(spinSignitures->sizePolicy().hasHeightForWidth());
        spinSignitures->setSizePolicy(sizePolicy3);

        gridLayout_2->addWidget(spinSignitures, 0, 1, 1, 1);

        spinPages = new QSpinBox(frame_7);
        spinPages->setObjectName(QString::fromUtf8("spinPages"));
        sizePolicy3.setHeightForWidth(spinPages->sizePolicy().hasHeightForWidth());
        spinPages->setSizePolicy(sizePolicy3);
        spinPages->setReadOnly(true);
        spinPages->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinPages->setMaximum(1000);

        gridLayout_2->addWidget(spinPages, 3, 1, 1, 1);


        gridLayout->addWidget(frame_7, 12, 0, 3, 1);

        label_23 = new QLabel(discriptionFrame);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout->addWidget(label_23, 8, 0, 1, 1);

        editBox = new QLineEdit(discriptionFrame);
        editBox->setObjectName(QString::fromUtf8("editBox"));

        gridLayout->addWidget(editBox, 8, 1, 1, 1);


        gridLayout_5->addWidget(discriptionFrame, 0, 0, 1, 1);

        scrollAreaDescription->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollAreaDescription);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 870, 29));
        menuOptions = new QMenu(menubar);
        menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
        menuNew_Entry = new QMenu(menuOptions);
        menuNew_Entry->setObjectName(QString::fromUtf8("menuNew_Entry"));
        MainWindow->setMenuBar(menubar);
        QWidget::setTabOrder(editPageMaterial, editCoverMaterial);
        QWidget::setTabOrder(editCoverMaterial, editThreadColor);
        QWidget::setTabOrder(editThreadColor, editEndPageColor);
        QWidget::setTabOrder(editEndPageColor, editBox);
        QWidget::setTabOrder(editBox, editSection);
        QWidget::setTabOrder(editSection, spinPageDimX);
        QWidget::setTabOrder(spinPageDimX, spinPageDimY);
        QWidget::setTabOrder(spinPageDimY, spinSignitures);
        QWidget::setTabOrder(spinSignitures, spinPagesPerSig);
        QWidget::setTabOrder(spinPagesPerSig, spinSpineDim);
        QWidget::setTabOrder(spinSpineDim, spinWeight);
        QWidget::setTabOrder(spinWeight, scrollAreaDescription);
        QWidget::setTabOrder(scrollAreaDescription, spinExtra);
        QWidget::setTabOrder(spinExtra, comboStatus);
        QWidget::setTabOrder(comboStatus, comboBookType);
        QWidget::setTabOrder(comboBookType, pushButtonGenStoreDiscription);
        QWidget::setTabOrder(pushButtonGenStoreDiscription, buttonCostBreakdown);
        QWidget::setTabOrder(buttonCostBreakdown, comboMode);
        QWidget::setTabOrder(comboMode, editExtra);
        QWidget::setTabOrder(editExtra, spinPages);
        QWidget::setTabOrder(spinPages, spinCost);

        menubar->addAction(menuOptions->menuAction());
        menuOptions->addAction(actionSaveClose);
        menuOptions->addAction(menuNew_Entry->menuAction());
        menuOptions->addAction(actionLoad);
        menuOptions->addAction(actionMove);
        menuOptions->addSeparator();
        menuOptions->addAction(actionSetting);
        menuNew_Entry->addAction(actionNewCurrent);
        menuNew_Entry->addAction(actionNewNew);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Book Tracker", nullptr));
        actionLoad->setText(QApplication::translate("MainWindow", "Load Entry", nullptr));
        actionClose->setText(QApplication::translate("MainWindow", "Close Entry", nullptr));
        actionOpen_Folder->setText(QApplication::translate("MainWindow", "Open Entry Folder", nullptr));
        actionSettings->setText(QApplication::translate("MainWindow", "Settings", nullptr));
        actionNewCurrent->setText(QApplication::translate("MainWindow", "New entry on current batch", nullptr));
        actionNewNew->setText(QApplication::translate("MainWindow", "New entry on new batch", nullptr));
        actionSaveClose->setText(QApplication::translate("MainWindow", "Save and Close", nullptr));
        actionMove->setText(QApplication::translate("MainWindow", "Move Entry", nullptr));
        actionSetting->setText(QApplication::translate("MainWindow", "Settings", nullptr));
        label->setText(QApplication::translate("MainWindow", "BookID:", nullptr));
        labelBookID->setText(QApplication::translate("MainWindow", "<None>", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "BatchID:", nullptr));
        labelBatchID->setText(QApplication::translate("MainWindow", "<None>", nullptr));
        label_24->setText(QApplication::translate("MainWindow", "Cost", nullptr));
        spinCost->setPrefix(QApplication::translate("MainWindow", "$", nullptr));
        comboMode->setItemText(0, QApplication::translate("MainWindow", "Edit Mode", nullptr));
        comboMode->setItemText(1, QApplication::translate("MainWindow", "ReadOnly Mode", nullptr));

        pushButtonGenStoreDiscription->setText(QApplication::translate("MainWindow", "Generate Store Description", nullptr));
        buttonCostBreakdown->setText(QApplication::translate("MainWindow", "Cost Breakdown", nullptr));
        label_13->setText(QApplication::translate("MainWindow", "Spine Dimension", nullptr));
        spinSpineDim->setSuffix(QApplication::translate("MainWindow", " in", nullptr));
        groupBox_9->setTitle(QApplication::translate("MainWindow", "Extra Information", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "Extra Cost", nullptr));
        spinExtra->setPrefix(QApplication::translate("MainWindow", "$", nullptr));
        spinExtra->setSuffix(QString());
        label_12->setText(QApplication::translate("MainWindow", "Spine/Ribbon/Thread", nullptr));
        label_21->setText(QApplication::translate("MainWindow", "End Page Color/Inner Cover", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "Paper Material", nullptr));
        label_19->setText(QApplication::translate("MainWindow", "Section", nullptr));
        label_22->setText(QApplication::translate("MainWindow", "Book Type", nullptr));
        comboBookType->setItemText(0, QApplication::translate("MainWindow", "Traditional", nullptr));
        comboBookType->setItemText(1, QApplication::translate("MainWindow", "Coptic", nullptr));
        comboBookType->setItemText(2, QApplication::translate("MainWindow", "Coptic (2 needle)", nullptr));
        comboBookType->setItemText(3, QApplication::translate("MainWindow", "StabStich", nullptr));
        comboBookType->setItemText(4, QApplication::translate("MainWindow", "Quarter", nullptr));
        comboBookType->setItemText(5, QApplication::translate("MainWindow", "Long Stich", nullptr));

        label_16->setText(QApplication::translate("MainWindow", "Weight", nullptr));
        spinWeight->setSuffix(QApplication::translate("MainWindow", " oz", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Cover Material", nullptr));
        spinPageDimY->setSuffix(QApplication::translate("MainWindow", " in", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "Page Dimensions", nullptr));
        label_17->setText(QApplication::translate("MainWindow", "Height", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "Cover Dimensions", nullptr));
        label_15->setText(QApplication::translate("MainWindow", "Width", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "x", nullptr));
        spinPageDimX->setSuffix(QApplication::translate("MainWindow", " in", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "x", nullptr));
        spinCoverDimY->setSuffix(QApplication::translate("MainWindow", " in", nullptr));
        spinCoverDimX->setSuffix(QApplication::translate("MainWindow", " in", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Status", nullptr));
        comboStatus->setItemText(0, QApplication::translate("MainWindow", "No photo", nullptr));
        comboStatus->setItemText(1, QApplication::translate("MainWindow", "Drafted", nullptr));
        comboStatus->setItemText(2, QApplication::translate("MainWindow", "Drafted (w photo)", nullptr));
        comboStatus->setItemText(3, QApplication::translate("MainWindow", "Published", nullptr));
        comboStatus->setItemText(4, QApplication::translate("MainWindow", "Sold", nullptr));

        label_14->setText(QApplication::translate("MainWindow", "Each Signiture", nullptr));
        label_20->setText(QApplication::translate("MainWindow", "Signitures", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "Page Count", nullptr));
        label_23->setText(QApplication::translate("MainWindow", "Box Letter", nullptr));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", nullptr));
        menuNew_Entry->setTitle(QApplication::translate("MainWindow", "New Entry", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
