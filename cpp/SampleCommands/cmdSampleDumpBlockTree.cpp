#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDumpBlockTree command
//

class CCommandSampleDumpBlockTree : public CRhinoCommand
{
public:
  CCommandSampleDumpBlockTree() = default;
  ~CCommandSampleDumpBlockTree() = default;
  UUID CommandUUID() override
  {
    // {2FE8CA38-48C7-48F7-8173-663D3A950D72}
    static const GUID SampleDumpBlockTreeCommand_UUID =
    { 0x2FE8CA38, 0x48C7, 0x48F7, { 0x81, 0x73, 0x66, 0x3D, 0x3A, 0x95, 0x0D, 0x72 } };
    return SampleDumpBlockTreeCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDumpBlockTree"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;

private:
  void DumpInstanceDefinition(const CRhinoInstanceDefinition* idef, ON_TextLog& dump, bool bRoot);
};

// The one and only CCommandSampleDumpBlockTree object
static class CCommandSampleDumpBlockTree theSampleDumpBlockTreeCommand;

CRhinoCommand::result CCommandSampleDumpBlockTree::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  const CRhinoInstanceDefinitionTable& idef_table = doc->m_instance_definition_table;
  int idef_count = idef_table.InstanceDefinitionCount();
  if (idef_count == 0)
  {
    RhinoApp().Print("No instance definitions found.\n");
    return CRhinoCommand::nothing;
  }

  ON_wString writer;
  ON_TextLog dump(writer);
  dump.SetIndentSize(4);

  for (int i = 0; i < idef_count; i++)
    DumpInstanceDefinition(idef_table[i], dump, true);

  RhinoApp().Print(L"%ls\n", static_cast<const wchar_t*>(writer));

  return CRhinoCommand::success;
}

void CCommandSampleDumpBlockTree::DumpInstanceDefinition(const CRhinoInstanceDefinition* idef, ON_TextLog& dump, bool bRoot)
{
  if (idef && !idef->IsDeleted())
  {
    ON_wString node = (bRoot) ? L"\x2500" : L"\x2514";
    dump.Print(L"%ls Instance definition %d = %ls\n", static_cast<const wchar_t*>(node), idef->Index(), static_cast<const wchar_t*>(idef->Name()));

    const int idef_object_count = idef->ObjectCount();
    if (idef_object_count)
    {
      dump.PushIndent();
      for (int i = 0; i < idef->ObjectCount(); i++)
      {
        const CRhinoObject* obj = idef->Object(i);
        if (obj)
        {
          const CRhinoInstanceObject* iref = CRhinoInstanceObject::Cast(obj);
          if (iref)
            DumpInstanceDefinition(iref->InstanceDefinition(), dump, false);
          else
            dump.Print(L"\x2514 Object %d = %ls\n", i, obj->ShortDescription(false));
        }
      }
      dump.PopIndent();
    }
  }
}

//
// END SampleDumpBlockTree command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
