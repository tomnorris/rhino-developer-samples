#include "StdAfx.h"
#include "SampleCommandsPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePointCursor command
//

#pragma region SamplePointCursor command

class CCommandSamplePointCursor : public CRhinoCommand
{
public:
  CCommandSamplePointCursor() = default;
  ~CCommandSamplePointCursor() = default;
  UUID CommandUUID() override
  {
    // {63AA9B30-7EAD-415E-AB07-690530E40746}
    static const GUID SamplePointCursorCommand_UUID =
    { 0x63AA9B30, 0x7EAD, 0x415E, { 0xAB, 0x7, 0x69, 0x5, 0x30, 0xE4, 0x7, 0x46 } };
    return SamplePointCursorCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SamplePointCursor"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};
static class CCommandSamplePointCursor theSamplePointCursorCommand;

CRhinoCommand::result CCommandSamplePointCursor::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Location of point object");
  gp.AcceptNothing(FALSE);

  HCURSOR hCursor = SampleCommandsPlugIn().SampleCursor();
  if (0 != hCursor)
    gp.SetGetPointCursor(hCursor);

  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint point = gp.Point();

  doc->AddPointObject(point);
  doc->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePointCursor command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
