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

#include "mitkDispatcher.h"

mitk::Dispatcher::Dispatcher()
{
  // TODO Auto-generated constructor stub

}

/*
 * Sets/Adds the EventInteractor that is associated with the DataNode to the Dispatcher Queue.
 * If there already exists an EventInteractor that has a reference to the same DataNode, is is removed.
 *
 */
void mitk::Dispatcher::SetEventInteractor(const DataNode* dataNode)
{
  std::list<mitk::EventInteractor::Pointer>::iterator it = m_Interactors.begin();
  while (it != m_Interactors.end())
  {
    if (it->GetPointer()->GetDataNode() == dataNode)
    {
      m_Interactors.remove(*it);
      break;
    }
    it++;
  }

  mitk::EventInteractor::Pointer eventInteractor = dynamic_cast<mitk::EventInteractor*>(dataNode->GetInteractor());
  if (eventInteractor.IsNotNull())
  {
    m_Interactors.push_back(eventInteractor);
  }
}

void mitk::Dispatcher::RemoveEventInteractor(const DataNode* dataNode)
{
  std::list<mitk::EventInteractor::Pointer>::iterator it = m_Interactors.begin();
  while (it != m_Interactors.end())
  {
    if (it->GetPointer()->GetDataNode() == dataNode)
    {
      m_Interactors.remove(*it);
      break;
    }
    it++;
  }
}

mitk::Dispatcher::~Dispatcher()
{
  // TODO Auto-generated destructor stub
}

bool mitk::Dispatcher::ProcessEvent(mitk::Event event)
{
  mitk::Event p = event;
  m_Interactors.sort();
  //p = NULL;
  return false;
}

