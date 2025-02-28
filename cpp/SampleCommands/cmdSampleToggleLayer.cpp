#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleToggleLayer command
//

#pragma region SampleToggleLayer command

class CCommandSampleToggleLayer : public CRhinoCommand
{
public:
  CCommandSampleToggleLayer() = default;
  ~CCommandSampleToggleLayer() = default;
  UUID CommandUUID() override
  {
    // {72EAC2EB-E568-4C91-A1F4-3EA3DC71E537}
    static const GUID SampleToggleLayerCommand_UUID =
    { 0x72EAC2EB, 0xE568, 0x4C91, { 0xA1, 0xF4, 0x3E, 0xA3, 0xDC, 0x71, 0xE5, 0x37 } };
    return SampleToggleLayerCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleToggleLayer"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleToggleLayer object
static class CCommandSampleToggleLayer theSampleToggleLayerCommand;

CRhinoCommand::result CCommandSampleToggleLayer::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Layer to toggle visiblity");
  gs.GetString();
  if (gs.CommandResult() != CRhinoCommand::success)
    return gs.CommandResult();

  ON_wString layer_name = gs.String();
  layer_name.TrimLeftAndRight();
  if (layer_name.IsEmpty())
    return CRhinoCommand::nothing;

  CRhinoLayerTable& layer_table = doc->m_layer_table;
  int layer_index = layer_table.FindLayerFromFullPathName(layer_name, -1);
  if (layer_index < 0)
  {
    RhinoApp().Print(L"The \"%ls\" layer was not found.\n", static_cast<const wchar_t*>(layer_name));
    return CRhinoCommand::nothing;
  }

  bool bVisible = layer_table[layer_index].PersistentVisibility() ? false : true; // toggle...

  ON_Layer onlayer = layer_table[layer_index];
  onlayer.SetVisible(bVisible);
  onlayer.SetPersistentVisibility(bVisible);

  layer_table.ModifyLayer(onlayer, layer_index, true);
  doc->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleToggleLayer command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
