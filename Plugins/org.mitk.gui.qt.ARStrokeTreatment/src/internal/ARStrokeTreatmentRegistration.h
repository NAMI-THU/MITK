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

#ifndef ARStrokeTreatmentRegistration_h
#define ARStrokeTreatmentRegistration_h

#include <QmitkAbstractView.h>

/**
  \brief ARStrokeTreatmentView

  \warning  This class is not yet documented. Use "git blame" and ask the author to provide basic documentation.

  \sa QmitkAbstractView
  \ingroup ${plugin_target}_internal
*/

class ARStrokeTreatmentRegistration : public QmitkAbstractView
{
  // this is needed for all Qt objects that should have a Qt meta-object
  // (everything that derives from QObject and wants to have signal/slots)
  Q_OBJECT

public:
  void SetFocus() override;

  static const std::string VIEW_ID;

  void CreateQtPartControl(QWidget *parent) override;

  ARStrokeTreatmentRegistration();
  ~ARStrokeTreatmentRegistration() override;

protected:
  void InitializeRegistration();
};

#endif // ARStrokeTreatmentRegistration_h