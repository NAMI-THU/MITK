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

// mitk image
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
  connect(m_Controls.m_TrackingDeviceSelectionWidget,
          SIGNAL(NavigationDataSourceSelected(mitk::NavigationDataSource::Pointer)),
          this,
          SLOT(OnSetupNavigation()));
  connect(m_Controls.m_TrackingGrabber, SIGNAL(clicked()), this, SLOT(OnTrackingGrabberPushed()));
}

void ARStrokeTreatmentView::OnTrackingGrabberPushed()
{
  if (!m_TrackingActive)
  {
    m_Updatetimer = new QTimer(this);
    m_TrackingActive = true;
    m_Controls.m_TrackingGrabber->setText("Disable Tracking");
    m_TrackingSource = m_Controls.m_TrackingDeviceSelectionWidget->GetSelectedNavigationDataSource();
    connect(m_Updatetimer, SIGNAL(timeout()), this, SLOT(UpdateTrackingData()));
    m_Updatetimer->start(100);
  }
  if (m_TrackingActive)
  {
    m_Updatetimer = NULL;
    m_TrackingActive = false;
    m_Controls.m_TrackingGrabber->setText("Activate Tracking");
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
