/********************************************************************************
** Form generated from reading UI file 'QmitkMITKIGTTrackingToolboxViewControls.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QMITKMITKIGTTRACKINGTOOLBOXVIEWCONTROLS_H
#define UI_QMITKMITKIGTTRACKINGTOOLBOXVIEWCONTROLS_H

#include <QmitkToolTrackingStatusWidget.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QmitkNavigationToolCreationWidget.h"
#include "QmitkTrackingDeviceConfigurationWidget.h"

QT_BEGIN_NAMESPACE

class Ui_QmitkMITKIGTTrackingToolboxViewControls
{
public:
    QWidget *m_simpleWidget;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_25;
    QPushButton *m_ConnectSimpleMode;
    QPushButton *m_StartTrackingSimpleMode;
    QSpacerItem *horizontalSpacer_21;
    QPushButton *m_advancedUI;
    QGridLayout *gridLayout_2;
    QTabWidget *m_MainWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_4;
    QFrame *m_ConfigurationWidgetFrame;
    QGridLayout *gridLayout;
    QmitkTrackingDeviceConfigurationWidget *m_ConfigurationWidget;
    QFrame *m_TrackingToolsFrame;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QStackedWidget *m_TrackingToolsWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QLabel *m_ToolLabel;
    QSpacerItem *horizontalSpacer;
    QmitkToolTrackingStatusWidget *m_TrackingToolsStatusWidget;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_7;
    QVBoxLayout *verticalLayout;
    QPushButton *m_AutoDetectTools;
    QPushButton *m_AddSingleTool;
    QPushButton *m_LoadTools;
    QPushButton *m_ResetTools;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_10;
    QmitkNavigationToolCreationWidget *m_NavigationToolCreationWidget;
    QFrame *m_TrackingControlsFrame;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QLabel *m_TrackingControlLabel;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_11;
    QPushButton *m_ConnectDisconnectButton;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *m_StartStopTrackingButton;
    QHBoxLayout *horizontalLayout_11;
    QSpacerItem *horizontalSpacer_9;
    QLabel *m_TrackingFrozenLabel;
    QPushButton *m_FreezeUnfreezeTrackingButton;
    QSpacerItem *verticalSpacer;
    QWidget *tab_2;
    QGridLayout *gridLayout_3;
    QGroupBox *m_UpdateRateOptionsGroupBox;
    QVBoxLayout *verticalLayout_14;
    QHBoxLayout *horizontalLayout_6;
    QLabel *m_OptionsUpdateRateLabel;
    QSpacerItem *horizontalSpacer_5;
    QSpinBox *m_UpdateRate;
    QCheckBox *m_UseDifferentUpdateRates;
    QHBoxLayout *horizontalLayout_8;
    QLabel *m_OptionsRenderUpdateRateLabel;
    QSpacerItem *horizontalSpacer_3;
    QSpinBox *m_RenderUpdateRate;
    QHBoxLayout *horizontalLayout_10;
    QLabel *m_OptionsLogUpdateRateLabel;
    QSpacerItem *horizontalSpacer_8;
    QSpinBox *m_LogUpdateRate;
    QGroupBox *m_OtherOptionsGroupBox;
    QVBoxLayout *verticalLayout_12;
    QCheckBox *m_ShowToolQuaternions;
    QCheckBox *m_SimpleUI;
    QLabel *label_6;
    QCheckBox *m_InverseMode;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_7;
    QCheckBox *m_ShowTrackingVolume;
    QLabel *label_5;
    QComboBox *m_VolumeSelectionBox;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_5;
    QCheckBox *showHideToolProjectionCheckBox;
    QComboBox *m_toolselector;
    QCheckBox *showHideToolAxisCheckBox;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_15;
    QCheckBox *m_EnableOpenIGTLinkMicroService;
    QLabel *label_7;
    QComboBox *m_OpenIGTLinkDataFormat;
    QCheckBox *m_DisableAllTimers;
    QLabel *m_RenderWarningLabel;
    QSpacerItem *verticalSpacer_3;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QLineEdit *m_LoggingFileName;
    QPushButton *m_ChooseFile;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *m_LoggingLimit;
    QSpacerItem *horizontalSpacer_6;
    QSpinBox *m_LoggedFramesLimit;
    QRadioButton *m_CsvFormat;
    QRadioButton *m_XmlFormat;
    QCheckBox *m_SkipInvalidData;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_6;
    QLabel *m_LoggingLabel;
    QLabel *m_LoggedFramesLabel;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *m_StartLogging;
    QPushButton *m_StopLogging;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *QmitkMITKIGTTrackingToolboxViewControls)
    {
        if (QmitkMITKIGTTrackingToolboxViewControls->objectName().isEmpty())
            QmitkMITKIGTTrackingToolboxViewControls->setObjectName(QString::fromUtf8("QmitkMITKIGTTrackingToolboxViewControls"));
        QmitkMITKIGTTrackingToolboxViewControls->resize(370, 739);
        QmitkMITKIGTTrackingToolboxViewControls->setMinimumSize(QSize(0, 0));
        m_simpleWidget = new QWidget(QmitkMITKIGTTrackingToolboxViewControls);
        m_simpleWidget->setObjectName(QString::fromUtf8("m_simpleWidget"));
        m_simpleWidget->setGeometry(QRect(9, 742, 303, 70));
        gridLayout_4 = new QGridLayout(m_simpleWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalLayout_25 = new QHBoxLayout();
        horizontalLayout_25->setSpacing(6);
        horizontalLayout_25->setObjectName(QString::fromUtf8("horizontalLayout_25"));
        m_ConnectSimpleMode = new QPushButton(m_simpleWidget);
        m_ConnectSimpleMode->setObjectName(QString::fromUtf8("m_ConnectSimpleMode"));
        m_ConnectSimpleMode->setMinimumSize(QSize(70, 50));

        horizontalLayout_25->addWidget(m_ConnectSimpleMode);

        m_StartTrackingSimpleMode = new QPushButton(m_simpleWidget);
        m_StartTrackingSimpleMode->setObjectName(QString::fromUtf8("m_StartTrackingSimpleMode"));
        m_StartTrackingSimpleMode->setMinimumSize(QSize(70, 50));

        horizontalLayout_25->addWidget(m_StartTrackingSimpleMode);

        horizontalSpacer_21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_25->addItem(horizontalSpacer_21);

        m_advancedUI = new QPushButton(m_simpleWidget);
        m_advancedUI->setObjectName(QString::fromUtf8("m_advancedUI"));
        m_advancedUI->setMinimumSize(QSize(70, 50));

        horizontalLayout_25->addWidget(m_advancedUI);


        gridLayout_4->addLayout(horizontalLayout_25, 0, 0, 1, 1);

        gridLayout_2 = new QGridLayout(QmitkMITKIGTTrackingToolboxViewControls);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        m_MainWidget = new QTabWidget(QmitkMITKIGTTrackingToolboxViewControls);
        m_MainWidget->setObjectName(QString::fromUtf8("m_MainWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_4 = new QVBoxLayout(tab);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        m_ConfigurationWidgetFrame = new QFrame(tab);
        m_ConfigurationWidgetFrame->setObjectName(QString::fromUtf8("m_ConfigurationWidgetFrame"));
        m_ConfigurationWidgetFrame->setFrameShape(QFrame::NoFrame);
        m_ConfigurationWidgetFrame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(m_ConfigurationWidgetFrame);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m_ConfigurationWidget = new QmitkTrackingDeviceConfigurationWidget(m_ConfigurationWidgetFrame);
        m_ConfigurationWidget->setObjectName(QString::fromUtf8("m_ConfigurationWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_ConfigurationWidget->sizePolicy().hasHeightForWidth());
        m_ConfigurationWidget->setSizePolicy(sizePolicy);

        gridLayout->addWidget(m_ConfigurationWidget, 0, 0, 1, 1);


        verticalLayout_4->addWidget(m_ConfigurationWidgetFrame);

        m_TrackingToolsFrame = new QFrame(tab);
        m_TrackingToolsFrame->setObjectName(QString::fromUtf8("m_TrackingToolsFrame"));
        m_TrackingToolsFrame->setFrameShape(QFrame::NoFrame);
        m_TrackingToolsFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(m_TrackingToolsFrame);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(m_TrackingToolsFrame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        font.setKerning(true);
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout_2->addWidget(label_2);

        m_TrackingToolsWidget = new QStackedWidget(m_TrackingToolsFrame);
        m_TrackingToolsWidget->setObjectName(QString::fromUtf8("m_TrackingToolsWidget"));
        m_TrackingToolsWidget->setLineWidth(0);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout_11 = new QVBoxLayout(page);
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setContentsMargins(11, 11, 11, 11);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(page);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_3->addWidget(label_4);

        m_ToolLabel = new QLabel(page);
        m_ToolLabel->setObjectName(QString::fromUtf8("m_ToolLabel"));

        horizontalLayout_3->addWidget(m_ToolLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout_11->addLayout(horizontalLayout_3);

        m_TrackingToolsStatusWidget = new QmitkToolTrackingStatusWidget(page);
        m_TrackingToolsStatusWidget->setObjectName(QString::fromUtf8("m_TrackingToolsStatusWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_TrackingToolsStatusWidget->sizePolicy().hasHeightForWidth());
        m_TrackingToolsStatusWidget->setSizePolicy(sizePolicy1);

        verticalLayout_11->addWidget(m_TrackingToolsStatusWidget);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalSpacer_7 = new QSpacerItem(13, 49, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_7);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        m_AutoDetectTools = new QPushButton(page);
        m_AutoDetectTools->setObjectName(QString::fromUtf8("m_AutoDetectTools"));
        m_AutoDetectTools->setMinimumSize(QSize(120, 0));

        verticalLayout->addWidget(m_AutoDetectTools);

        m_AddSingleTool = new QPushButton(page);
        m_AddSingleTool->setObjectName(QString::fromUtf8("m_AddSingleTool"));
        m_AddSingleTool->setMinimumSize(QSize(120, 0));

        verticalLayout->addWidget(m_AddSingleTool);

        m_LoadTools = new QPushButton(page);
        m_LoadTools->setObjectName(QString::fromUtf8("m_LoadTools"));
        m_LoadTools->setMinimumSize(QSize(120, 0));

        verticalLayout->addWidget(m_LoadTools);

        m_ResetTools = new QPushButton(page);
        m_ResetTools->setObjectName(QString::fromUtf8("m_ResetTools"));
        m_ResetTools->setMinimumSize(QSize(120, 0));

        verticalLayout->addWidget(m_ResetTools);


        horizontalLayout_9->addLayout(verticalLayout);


        verticalLayout_11->addLayout(horizontalLayout_9);

        m_TrackingToolsWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        verticalLayout_10 = new QVBoxLayout(page_2);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        m_NavigationToolCreationWidget = new QmitkNavigationToolCreationWidget(page_2);
        m_NavigationToolCreationWidget->setObjectName(QString::fromUtf8("m_NavigationToolCreationWidget"));

        verticalLayout_10->addWidget(m_NavigationToolCreationWidget);

        m_TrackingToolsWidget->addWidget(page_2);

        verticalLayout_2->addWidget(m_TrackingToolsWidget);


        verticalLayout_4->addWidget(m_TrackingToolsFrame);

        m_TrackingControlsFrame = new QFrame(tab);
        m_TrackingControlsFrame->setObjectName(QString::fromUtf8("m_TrackingControlsFrame"));
        m_TrackingControlsFrame->setFrameShape(QFrame::NoFrame);
        m_TrackingControlsFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(m_TrackingControlsFrame);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_3 = new QLabel(m_TrackingControlsFrame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout_3->addWidget(label_3);

        m_TrackingControlLabel = new QLabel(m_TrackingControlsFrame);
        m_TrackingControlLabel->setObjectName(QString::fromUtf8("m_TrackingControlLabel"));

        verticalLayout_3->addWidget(m_TrackingControlLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_11);

        m_ConnectDisconnectButton = new QPushButton(m_TrackingControlsFrame);
        m_ConnectDisconnectButton->setObjectName(QString::fromUtf8("m_ConnectDisconnectButton"));
        m_ConnectDisconnectButton->setMinimumSize(QSize(142, 0));

        horizontalLayout->addWidget(m_ConnectDisconnectButton);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        m_StartStopTrackingButton = new QPushButton(m_TrackingControlsFrame);
        m_StartStopTrackingButton->setObjectName(QString::fromUtf8("m_StartStopTrackingButton"));
        m_StartStopTrackingButton->setMinimumSize(QSize(142, 0));

        horizontalLayout_2->addWidget(m_StartStopTrackingButton);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_9);

        m_TrackingFrozenLabel = new QLabel(m_TrackingControlsFrame);
        m_TrackingFrozenLabel->setObjectName(QString::fromUtf8("m_TrackingFrozenLabel"));

        horizontalLayout_11->addWidget(m_TrackingFrozenLabel);

        m_FreezeUnfreezeTrackingButton = new QPushButton(m_TrackingControlsFrame);
        m_FreezeUnfreezeTrackingButton->setObjectName(QString::fromUtf8("m_FreezeUnfreezeTrackingButton"));
        m_FreezeUnfreezeTrackingButton->setEnabled(true);
        m_FreezeUnfreezeTrackingButton->setMinimumSize(QSize(142, 0));

        horizontalLayout_11->addWidget(m_FreezeUnfreezeTrackingButton);


        verticalLayout_3->addLayout(horizontalLayout_11);


        verticalLayout_4->addWidget(m_TrackingControlsFrame);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        m_MainWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_3 = new QGridLayout(tab_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        m_UpdateRateOptionsGroupBox = new QGroupBox(tab_2);
        m_UpdateRateOptionsGroupBox->setObjectName(QString::fromUtf8("m_UpdateRateOptionsGroupBox"));
        verticalLayout_14 = new QVBoxLayout(m_UpdateRateOptionsGroupBox);
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setContentsMargins(11, 11, 11, 11);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        m_OptionsUpdateRateLabel = new QLabel(m_UpdateRateOptionsGroupBox);
        m_OptionsUpdateRateLabel->setObjectName(QString::fromUtf8("m_OptionsUpdateRateLabel"));

        horizontalLayout_6->addWidget(m_OptionsUpdateRateLabel);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);

        m_UpdateRate = new QSpinBox(m_UpdateRateOptionsGroupBox);
        m_UpdateRate->setObjectName(QString::fromUtf8("m_UpdateRate"));
        m_UpdateRate->setMaximum(100);
        m_UpdateRate->setValue(10);

        horizontalLayout_6->addWidget(m_UpdateRate);


        verticalLayout_14->addLayout(horizontalLayout_6);

        m_UseDifferentUpdateRates = new QCheckBox(m_UpdateRateOptionsGroupBox);
        m_UseDifferentUpdateRates->setObjectName(QString::fromUtf8("m_UseDifferentUpdateRates"));

        verticalLayout_14->addWidget(m_UseDifferentUpdateRates);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        m_OptionsRenderUpdateRateLabel = new QLabel(m_UpdateRateOptionsGroupBox);
        m_OptionsRenderUpdateRateLabel->setObjectName(QString::fromUtf8("m_OptionsRenderUpdateRateLabel"));
        m_OptionsRenderUpdateRateLabel->setEnabled(false);

        horizontalLayout_8->addWidget(m_OptionsRenderUpdateRateLabel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_3);

        m_RenderUpdateRate = new QSpinBox(m_UpdateRateOptionsGroupBox);
        m_RenderUpdateRate->setObjectName(QString::fromUtf8("m_RenderUpdateRate"));
        m_RenderUpdateRate->setEnabled(false);
        m_RenderUpdateRate->setMinimum(0);
        m_RenderUpdateRate->setMaximum(100);
        m_RenderUpdateRate->setValue(30);

        horizontalLayout_8->addWidget(m_RenderUpdateRate);


        verticalLayout_14->addLayout(horizontalLayout_8);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        m_OptionsLogUpdateRateLabel = new QLabel(m_UpdateRateOptionsGroupBox);
        m_OptionsLogUpdateRateLabel->setObjectName(QString::fromUtf8("m_OptionsLogUpdateRateLabel"));
        m_OptionsLogUpdateRateLabel->setEnabled(false);

        horizontalLayout_10->addWidget(m_OptionsLogUpdateRateLabel);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_8);

        m_LogUpdateRate = new QSpinBox(m_UpdateRateOptionsGroupBox);
        m_LogUpdateRate->setObjectName(QString::fromUtf8("m_LogUpdateRate"));
        m_LogUpdateRate->setEnabled(false);
        m_LogUpdateRate->setMinimum(1);
        m_LogUpdateRate->setMaximum(120);
        m_LogUpdateRate->setSingleStep(10);
        m_LogUpdateRate->setValue(60);

        horizontalLayout_10->addWidget(m_LogUpdateRate);


        verticalLayout_14->addLayout(horizontalLayout_10);


        gridLayout_3->addWidget(m_UpdateRateOptionsGroupBox, 1, 0, 1, 2);

        m_OtherOptionsGroupBox = new QGroupBox(tab_2);
        m_OtherOptionsGroupBox->setObjectName(QString::fromUtf8("m_OtherOptionsGroupBox"));
        verticalLayout_12 = new QVBoxLayout(m_OtherOptionsGroupBox);
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        m_ShowToolQuaternions = new QCheckBox(m_OtherOptionsGroupBox);
        m_ShowToolQuaternions->setObjectName(QString::fromUtf8("m_ShowToolQuaternions"));

        verticalLayout_12->addWidget(m_ShowToolQuaternions);

        m_SimpleUI = new QCheckBox(m_OtherOptionsGroupBox);
        m_SimpleUI->setObjectName(QString::fromUtf8("m_SimpleUI"));

        verticalLayout_12->addWidget(m_SimpleUI);

        label_6 = new QLabel(m_OtherOptionsGroupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout_12->addWidget(label_6);

        m_InverseMode = new QCheckBox(m_OtherOptionsGroupBox);
        m_InverseMode->setObjectName(QString::fromUtf8("m_InverseMode"));

        verticalLayout_12->addWidget(m_InverseMode);


        gridLayout_3->addWidget(m_OtherOptionsGroupBox, 4, 0, 1, 2);

        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_7 = new QVBoxLayout(groupBox);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        m_ShowTrackingVolume = new QCheckBox(groupBox);
        m_ShowTrackingVolume->setObjectName(QString::fromUtf8("m_ShowTrackingVolume"));
        m_ShowTrackingVolume->setEnabled(true);
        m_ShowTrackingVolume->setChecked(false);

        verticalLayout_7->addWidget(m_ShowTrackingVolume);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_7->addWidget(label_5);

        m_VolumeSelectionBox = new QComboBox(groupBox);
        m_VolumeSelectionBox->setObjectName(QString::fromUtf8("m_VolumeSelectionBox"));

        verticalLayout_7->addWidget(m_VolumeSelectionBox);


        gridLayout_3->addWidget(groupBox, 2, 0, 1, 2);

        groupBox_5 = new QGroupBox(tab_2);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_5 = new QGridLayout(groupBox_5);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        showHideToolProjectionCheckBox = new QCheckBox(groupBox_5);
        showHideToolProjectionCheckBox->setObjectName(QString::fromUtf8("showHideToolProjectionCheckBox"));
        showHideToolProjectionCheckBox->setEnabled(true);

        gridLayout_5->addWidget(showHideToolProjectionCheckBox, 0, 0, 1, 1);

        m_toolselector = new QComboBox(groupBox_5);
        m_toolselector->setObjectName(QString::fromUtf8("m_toolselector"));

        gridLayout_5->addWidget(m_toolselector, 0, 1, 1, 1);

        showHideToolAxisCheckBox = new QCheckBox(groupBox_5);
        showHideToolAxisCheckBox->setObjectName(QString::fromUtf8("showHideToolAxisCheckBox"));
        showHideToolAxisCheckBox->setEnabled(false);

        gridLayout_5->addWidget(showHideToolAxisCheckBox, 1, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_5, 5, 0, 1, 2);

        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_15 = new QVBoxLayout(groupBox_2);
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setContentsMargins(11, 11, 11, 11);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        m_EnableOpenIGTLinkMicroService = new QCheckBox(groupBox_2);
        m_EnableOpenIGTLinkMicroService->setObjectName(QString::fromUtf8("m_EnableOpenIGTLinkMicroService"));
        m_EnableOpenIGTLinkMicroService->setChecked(true);

        verticalLayout_15->addWidget(m_EnableOpenIGTLinkMicroService);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout_15->addWidget(label_7);

        m_OpenIGTLinkDataFormat = new QComboBox(groupBox_2);
        m_OpenIGTLinkDataFormat->addItem(QString());
        m_OpenIGTLinkDataFormat->addItem(QString());
        m_OpenIGTLinkDataFormat->addItem(QString());
        m_OpenIGTLinkDataFormat->addItem(QString());
        m_OpenIGTLinkDataFormat->setObjectName(QString::fromUtf8("m_OpenIGTLinkDataFormat"));

        verticalLayout_15->addWidget(m_OpenIGTLinkDataFormat);


        gridLayout_3->addWidget(groupBox_2, 3, 0, 1, 2);

        m_DisableAllTimers = new QCheckBox(tab_2);
        m_DisableAllTimers->setObjectName(QString::fromUtf8("m_DisableAllTimers"));

        gridLayout_3->addWidget(m_DisableAllTimers, 0, 0, 1, 1);

        m_RenderWarningLabel = new QLabel(tab_2);
        m_RenderWarningLabel->setObjectName(QString::fromUtf8("m_RenderWarningLabel"));
        m_RenderWarningLabel->setEnabled(true);
        m_RenderWarningLabel->setTextFormat(Qt::AutoText);

        gridLayout_3->addWidget(m_RenderWarningLabel, 0, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_3, 6, 0, 1, 1);

        m_MainWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_8 = new QVBoxLayout(tab_3);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(tab_3);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        m_LoggingFileName = new QLineEdit(tab_3);
        m_LoggingFileName->setObjectName(QString::fromUtf8("m_LoggingFileName"));

        horizontalLayout_4->addWidget(m_LoggingFileName);

        m_ChooseFile = new QPushButton(tab_3);
        m_ChooseFile->setObjectName(QString::fromUtf8("m_ChooseFile"));

        horizontalLayout_4->addWidget(m_ChooseFile);


        verticalLayout_8->addLayout(horizontalLayout_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        m_LoggingLimit = new QCheckBox(tab_3);
        m_LoggingLimit->setObjectName(QString::fromUtf8("m_LoggingLimit"));

        horizontalLayout_7->addWidget(m_LoggingLimit);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);

        m_LoggedFramesLimit = new QSpinBox(tab_3);
        m_LoggedFramesLimit->setObjectName(QString::fromUtf8("m_LoggedFramesLimit"));
        m_LoggedFramesLimit->setMinimum(1);
        m_LoggedFramesLimit->setMaximum(9999);
        m_LoggedFramesLimit->setValue(300);

        horizontalLayout_7->addWidget(m_LoggedFramesLimit);


        verticalLayout_8->addLayout(horizontalLayout_7);

        m_CsvFormat = new QRadioButton(tab_3);
        m_CsvFormat->setObjectName(QString::fromUtf8("m_CsvFormat"));
        m_CsvFormat->setChecked(true);

        verticalLayout_8->addWidget(m_CsvFormat);

        m_XmlFormat = new QRadioButton(tab_3);
        m_XmlFormat->setObjectName(QString::fromUtf8("m_XmlFormat"));

        verticalLayout_8->addWidget(m_XmlFormat);

        m_SkipInvalidData = new QCheckBox(tab_3);
        m_SkipInvalidData->setObjectName(QString::fromUtf8("m_SkipInvalidData"));

        verticalLayout_8->addWidget(m_SkipInvalidData);

        groupBox_4 = new QGroupBox(tab_3);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_6 = new QVBoxLayout(groupBox_4);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        m_LoggingLabel = new QLabel(groupBox_4);
        m_LoggingLabel->setObjectName(QString::fromUtf8("m_LoggingLabel"));

        verticalLayout_6->addWidget(m_LoggingLabel);

        m_LoggedFramesLabel = new QLabel(groupBox_4);
        m_LoggedFramesLabel->setObjectName(QString::fromUtf8("m_LoggedFramesLabel"));

        verticalLayout_6->addWidget(m_LoggedFramesLabel);


        verticalLayout_8->addWidget(groupBox_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        m_StartLogging = new QPushButton(tab_3);
        m_StartLogging->setObjectName(QString::fromUtf8("m_StartLogging"));

        horizontalLayout_5->addWidget(m_StartLogging);

        m_StopLogging = new QPushButton(tab_3);
        m_StopLogging->setObjectName(QString::fromUtf8("m_StopLogging"));

        horizontalLayout_5->addWidget(m_StopLogging);


        verticalLayout_8->addLayout(horizontalLayout_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_2);

        m_MainWidget->addTab(tab_3, QString());

        gridLayout_2->addWidget(m_MainWidget, 0, 0, 1, 1);


        retranslateUi(QmitkMITKIGTTrackingToolboxViewControls);

        m_MainWidget->setCurrentIndex(0);
        m_TrackingToolsWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QmitkMITKIGTTrackingToolboxViewControls);
    } // setupUi

    void retranslateUi(QWidget *QmitkMITKIGTTrackingToolboxViewControls)
    {
        QmitkMITKIGTTrackingToolboxViewControls->setWindowTitle(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "QmitkTemplate", nullptr));
        m_ConnectSimpleMode->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Connect", nullptr));
        m_StartTrackingSimpleMode->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Start\n"
"Tracking", nullptr));
        m_advancedUI->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Advanced\n"
"Mode", nullptr));
        label_2->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Tracking Tools", nullptr));
        label_4->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "ToolStorage:", nullptr));
        m_ToolLabel->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "<none>", nullptr));
        m_AutoDetectTools->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Auto Detection", nullptr));
        m_AddSingleTool->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Add Single Tool", nullptr));
        m_LoadTools->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Load Tool Storage", nullptr));
        m_ResetTools->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Reset", nullptr));
        label_3->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Tracking Control", nullptr));
        m_TrackingControlLabel->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Status: disconnected", nullptr));
        m_ConnectDisconnectButton->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Connect", nullptr));
        m_StartStopTrackingButton->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Start Tracking", nullptr));
        m_TrackingFrozenLabel->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "<html><head/><body><p><span style=\" color:#ff0000;\">Tracking Frozen!</span></p></body></html>", nullptr));
        m_FreezeUnfreezeTrackingButton->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Freeze Tracking", nullptr));
        m_MainWidget->setTabText(m_MainWidget->indexOf(tab), QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Tracking", nullptr));
        m_UpdateRateOptionsGroupBox->setTitle(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Update Rate Options", nullptr));
        m_OptionsUpdateRateLabel->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Update Rate [per second]", nullptr));
        m_UseDifferentUpdateRates->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Use different Render and Log Update Rates", nullptr));
        m_OptionsRenderUpdateRateLabel->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Render Update Rate [fps]", nullptr));
        m_OptionsLogUpdateRateLabel->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Log Update Rate [per second]", nullptr));
        m_OtherOptionsGroupBox->setTitle(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Other Options", nullptr));
        m_ShowToolQuaternions->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Show Tool Quaternions", nullptr));
        m_SimpleUI->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Simple UI", nullptr));
        label_6->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Caution, only for backward compatibility:", nullptr));
        m_InverseMode->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Inverse mode (Quaternions are stored inverse)", nullptr));
        groupBox->setTitle(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Tracking Volume Options", nullptr));
        m_ShowTrackingVolume->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Show Tracking Volume", nullptr));
        label_5->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Select Model:", nullptr));
        groupBox_5->setTitle(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Tool Visualization Options", nullptr));
        showHideToolProjectionCheckBox->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Show Tool Projection", nullptr));
        showHideToolAxisCheckBox->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Show Tool Axis", nullptr));
        groupBox_2->setTitle(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Open IGT Link", nullptr));
        m_EnableOpenIGTLinkMicroService->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Enable Open IGT Link MicroService", nullptr));
        label_7->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Select Open IGT Link Data Format:", nullptr));
        m_OpenIGTLinkDataFormat->setItemText(0, QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "TRANSFORM", nullptr));
        m_OpenIGTLinkDataFormat->setItemText(1, QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "QTDATA", nullptr));
        m_OpenIGTLinkDataFormat->setItemText(2, QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "TDATA", nullptr));
        m_OpenIGTLinkDataFormat->setItemText(3, QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "POSITION", nullptr));

        m_DisableAllTimers->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Disable All Timers", nullptr));
        m_RenderWarningLabel->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "<html><head/><body><p align=\"right\"><span style=\" color:#ff0000;\">Rendering Disabled!</span></p></body></html>", nullptr));
        m_MainWidget->setTabText(m_MainWidget->indexOf(tab_2), QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Options", nullptr));
        label->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Filename:", nullptr));
        m_ChooseFile->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Choose File", nullptr));
        m_LoggingLimit->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Limit Number Of Logged Frames:", nullptr));
        m_CsvFormat->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "CSV format", nullptr));
        m_XmlFormat->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "XML format", nullptr));
        m_SkipInvalidData->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Skip invalid data", nullptr));
        groupBox_4->setTitle(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Logging Status", nullptr));
        m_LoggingLabel->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Logging OFF", nullptr));
        m_LoggedFramesLabel->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Logged Frames: 0", nullptr));
        m_StartLogging->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Start Logging", nullptr));
        m_StopLogging->setText(QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Stop Logging", nullptr));
        m_MainWidget->setTabText(m_MainWidget->indexOf(tab_3), QApplication::translate("QmitkMITKIGTTrackingToolboxViewControls", "Logging", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QmitkMITKIGTTrackingToolboxViewControls: public Ui_QmitkMITKIGTTrackingToolboxViewControls {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMITKMITKIGTTRACKINGTOOLBOXVIEWCONTROLS_H
