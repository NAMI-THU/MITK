/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

// Blueberry
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>

// Qmitk
#include "ARStrokeTreatmentView.h"

// Qt
#include <QMessageBox>
#include <QmitkDataStorageComboBox.h>

// mitk image
#include <QTimer>
#include <mitkIRenderingManager.h>
#include <mitkImage.h>

const std::string ARStrokeTreatmentView::VIEW_ID = "org.mitk.views.arstroketreatment";

void ARStrokeTreatmentView::SetFocus()
{
  // m_Controls.buttonPerformImageProcessing->setFocus();
}

void ARStrokeTreatmentView::CreateQtPartControl(QWidget *parent)
{
  // create GUI widgets from the Qt Designer's .ui file
  m_Controls.setupUi(parent);
  // connect(m_Controls.buttonPerformImageProcessing, &QPushButton::clicked, this,
  // &ARStrokeTreatmentView::DoImageProcessing);
  CreateConnections();
}

// Constructor and Destructor

void ARStrokeTreatmentView::OnTrackingGrabberPushed()
{
  if (!m_TrackingActive)
  {
    m_Updatetimer = new QTimer(this);
    m_TrackingActive = true;
    m_Controls.m_TrackingDataGrabber->setText("Disable Tracking");
    m_TrackingSource = m_Controls.m_TrackingDeviceSelectionWidget->GetSelectedNavigationDataSource();
    connect(m_Updatetimer, SIGNAL(timeout()), this, SLOT(UpdateTrackingData()));
    m_Updatetimer->start(100);
  }
  if (m_TrackingActive)
  {
    m_Updatetimer = NULL;
    m_TrackingActive = false;
    m_Controls.m_TrackingDataGrabber->setText("Activate Tracking");
    m_TrackingSource = NULL;
    m_Updatetimer = NULL;
  }
  return;
}

void ARStrokeTreatmentView::UpdateTrackingData()
{
  MITK_INFO << m_TrackingSource->GetOutput()->GetPosition();
  return;
}

void ARStrokeTreatmentView::OnSelectionChanged(berry::IWorkbenchPart::Pointer /*source*/,
                                               const QList<mitk::DataNode::Pointer> &nodes)
{
  // iterate all selected objects, adjust warning visibility
  foreach (mitk::DataNode::Pointer node, nodes)
  {
    if (node.IsNotNull() && dynamic_cast<mitk::Image *>(node->GetData()))
    {
      // m_Controls.labelWarning->setVisible(false);
      return;
    }
  }

  // m_Controls.labelWarning->setVisible(true);
}

void ARStrokeTreatmentView::OnStartTrackingGrabbing()
{
  if (!m_GrabbingTrackingData)
  {
    m_GrabbingTrackingData = true;
    m_Controls.m_StartGrabbing->setText("Stop Tracking");
    m_UpdateTimer->start(100);
  }
  else
  {
    m_UpdateTimer->stop();
    m_GrabbingTrackingData = false;
    m_Controls.m_StartGrabbing->setText("Start Tracking");
  }
}

void ARStrokeTreatmentView::OnStartVideoGrabbing()
{
  if (!m_GrabbingVideoData)
  {
    m_GrabbingVideoData = true;
    m_Controls.m_StartGrabbing->setText("Stop Video Grabbing");
    m_VideoCapture = new cv::VideoCapture(); // open the default camera
    m_VideoCapture->open(m_Controls.m_InputID->value());
    if (!m_VideoCapture->isOpened())
    {
      return;
    } // check if we succeeded

    if (true) //m_Controls.m_SeparateWindow->isChecked()
    {
      cv::namedWindow("Video", 1);
      while (m_GrabbingVideoData)
      {
        cv::Mat frame;
        *m_VideoCapture >> frame; // get a new frame from camera
        imshow("Video", frame);
        // Example to write a frame to a file:
        // imwrite("C:/temp/output.jpg", frame);
        // Press 'c' to stop the stream
        if (cv::waitKey(30) == 'c')
          break;
      }
    }
    else if (m_Controls.m_MITKImage->isChecked())
    {
      mitk::DataNode::Pointer imageNode = mitk::DataNode::New();
      imageNode->SetName("Open CV Example Image Stream");
      imageNode->SetData(mitk::Image::New());
      m_ConversionFilter = mitk::OpenCVToMitkImageFilter::New();
      this->GetDataStorage()->Add(imageNode);
      OnUpdateImage();
      // Initialize view on Image
      mitk::IRenderWindowPart *renderWindow = this->GetRenderWindowPart();
      if (renderWindow != NULL)
        renderWindow->GetRenderingManager()->InitializeViews(
          imageNode->GetData()->GetGeometry(), mitk::RenderingManager::REQUEST_UPDATE_ALL, true);

      m_UpdateTimer->setInterval(20);
      m_UpdateTimer->start();
    }
  }
  else
  {
    m_UpdateTimer->stop();
    m_GrabbingVideoData = false;
    m_Controls.m_StartGrabbing->setText("Start Video Grabbing");
    cv::destroyWindow("Video");
    mitk::DataNode::Pointer imageNode = this->GetDataStorage()->GetNamedNode("Open CV Example Image Stream");
    this->GetDataStorage()->Remove(imageNode);
    m_VideoCapture->release();
    delete m_VideoCapture;
  }
}

void ARStrokeTreatmentView::OnUpdateImage()
{
}

void ARStrokeTreatmentView::DoImageProcessing()
{
  QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
  if (nodes.empty())
    return;

  mitk::DataNode *node = nodes.front();

  if (!node)
  {
    // Nothing selected. Inform the user and return
    QMessageBox::information(nullptr, "Template", "Please load and select an image before starting image processing.");
    return;
  }

  // here we have a valid mitk::DataNode

  // a node itself is not very useful, we need its data item (the image)
  mitk::BaseData *data = node->GetData();
  if (data)
  {
    // test if this data item is an image or not (could also be a surface or something totally different)
    mitk::Image *image = dynamic_cast<mitk::Image *>(data);
    if (image)
    {
      std::stringstream message;
      std::string name;
      message << "Performing image processing for image ";
      if (node->GetName(name))
      {
        // a property called "name" was found for this DataNode
        message << "'" << name << "'";
      }
      message << ".";
      MITK_INFO << message.str();

      // actually do something here...
    }
  }
}

void ARStrokeTreatmentView::CreateConnections() {
  connect(m_Controls.m_TrackingDeviceSelectionWidget,
          SIGNAL(NavigationDataSourceSelected(mitk::NavigationDataSource::Pointer)),
          this,
          SLOT(OnSetupNavigation()));
  connect(m_Controls.m_TrackingDataGrabber, SIGNAL(clicked()), this, SLOT(OnTrackingGrabberPushed()));
  connect(m_UpdateTimer, SIGNAL(timeout()), this, SLOT(OnUpdateImage()));
  connect(m_Controls.m_TrackingDataLabel, SIGNAL(clicked()), this, SLOT(OnStartTrackingGrabbing()));
  connect(m_Controls.m_StartVideoGrabbing, SIGNAL(clicked()), this, SLOT(OnStartVideoGrabbing()));
  return;
}

void ARStrokeTreatmentView::TestText()
{
  MITK_INFO << "TestText succesfully printed! Yay!";
  return;
}
