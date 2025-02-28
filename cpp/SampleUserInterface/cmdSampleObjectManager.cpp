#include "StdAfx.h"
#include "SampleObjectManagerDialog.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleObjectManager command
//

#pragma region SampleObjectManager command

class CCommandSampleObjectManager : public CRhinoCommand
{
public:
  CCommandSampleObjectManager() = default;
  ~CCommandSampleObjectManager() = default;
  UUID CommandUUID() override
  {
    // {415ABCAE-E332-466A-B139-2963E81166C1}
    static const GUID SampleObjectManagerCommand_UUID =
    { 0x415ABCAE, 0xE332, 0x466A, { 0xB1, 0x39, 0x29, 0x63, 0xE8, 0x11, 0x66, 0xC1 } };
    return SampleObjectManagerCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleObjectManager"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleObjectManager object.  
static class CCommandSampleObjectManager theSampleObjectManagerCommand;

CRhinoCommand::result CCommandSampleObjectManager::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  ON_UUID tabId = CSampleObjectManagerDialog::ID();

  if (context.IsInteractive())
  {
    CRhinoTabbedDockBarDialog::OpenDockbarTab(*doc, tabId);
  }
  else
  {
    bool bVisible = CRhinoTabbedDockBarDialog::IsTabVisible(*doc, tabId);

    ON_wString str;
    str.Format(L"%ls is %ls. New value", LocalCommandName(), bVisible ? L"visible" : L"hidden");

    CRhinoGetOption go;
    go.SetCommandPrompt(str);
    int h_option = go.AddCommandOption(RHCMDOPTNAME(L"Hide"));
    int s_option = go.AddCommandOption(RHCMDOPTNAME(L"Show"));
    int t_option = go.AddCommandOption(RHCMDOPTNAME(L"Toggle"));
    go.GetOption();
    if (go.CommandResult() != CRhinoCommand::success)
      return go.CommandResult();

    const CRhinoCommandOption* option = go.Option();
    if (0 == option)
      return CRhinoCommand::failure;

    int option_index = option->m_option_index;

    if (h_option == option_index && bVisible)
      CRhinoTabbedDockBarDialog::ShowDockbarTab(*doc, tabId, false, false, nullptr);
    else if (s_option == option_index && !bVisible)
      CRhinoTabbedDockBarDialog::ShowDockbarTab(*doc, tabId, true, true, nullptr);
    else if (t_option == option_index)
      CRhinoTabbedDockBarDialog::ShowDockbarTab(*doc, tabId, !bVisible, !bVisible, nullptr);
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleObjectManager command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
