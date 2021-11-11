
#include "HelloNode.h"
#include "SGraphNodeSaySomething.h"


#include "ToolMenus.h"

#include "BlueprintNodeSpawner.h"	// BlueprintGraph
#include "BlueprintActionDatabaseRegistrar.h"	// BlueprintGraph
#include "EdGraphSchema_K2.h"	// BlueprintGraph
#include "K2Node_CallFunction.h"	// BlueprintGraph
#include "K2Node_MakeArray.h"	// BlueprintGraph
#include "KismetCompiler.h"	// KismetCompiler
#include "EdGraph/EdGraphNode.h"
#include "ScopedTransaction.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Kismet2/BlueprintEditorUtils.h"

#include "HelloWorld.h"	// MyBlueprintNode




#define LOCTEXT_NAMESPACE "UHello"



void UHello::AllocateDefaultPins() {

	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	for (const FName& PinName : ArgPinNames) {
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, PinName);
	}
}

void UHello::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);

	if (Context->bIsDebugging)
		return;

	FToolMenuSection& Section = Menu->AddSection("UHello", NSLOCTEXT("K2Nodes", "MakeSetHeader", "MakeSet"));

	if (Context->Pin)
	{
		if (Context->Pin->Direction == EGPD_Input && Context->Pin->ParentPin == nullptr)
		{
			Section.AddMenuEntry(
				"RemovePin",
				LOCTEXT("RemovePin", "Remove set element pin"),
				LOCTEXT("RemovePinTooltip", "Remove this element pin"),
				FSlateIcon(),
				FUIAction(
					FExecuteAction::CreateUObject(const_cast<UHello*>(this), &UHello::RemoveInputPin, const_cast<UEdGraphPin*>(Context->Pin))
				)
			);
		}
	}


}

void UHello::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const {
	UClass* ActionKey = GetClass();

	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

void UHello::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) {
	Super::ExpandNode(CompilerContext, SourceGraph);

	UEdGraphPin* ExecPin = GetExecPin();
	UEdGraphPin* ThenPin = GetThenPin();
	if (ExecPin && ThenPin) {

		// create a CallFunction node
		FName MyFunctionName = GET_FUNCTION_NAME_CHECKED(UHelloWorld, SaySomething_Internal);

		UK2Node_CallFunction* CallFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		CallFuncNode->FunctionReference.SetExternalMember(MyFunctionName, UHelloWorld::StaticClass());
		CallFuncNode->AllocateDefaultPins();

		// move exec pins
		CompilerContext.MovePinLinksToIntermediate(*ExecPin, *(CallFuncNode->GetExecPin()));
		CompilerContext.MovePinLinksToIntermediate(*ThenPin, *(CallFuncNode->GetThenPin()));

		// create a "Make Array" node to compile all args
		UK2Node_MakeArray* MakeArrayNode = CompilerContext.SpawnIntermediateNode<UK2Node_MakeArray>(this, SourceGraph);
		MakeArrayNode->AllocateDefaultPins();

		// Connect Make Array output to function arg
		UEdGraphPin* ArrayOut = MakeArrayNode->GetOutputPin();
		UEdGraphPin* FuncArgPin = CallFuncNode->FindPinChecked(TEXT("InWords"));
		ArrayOut->MakeLinkTo(FuncArgPin);

		// This will set the "Make Array" node's type, only works if one pin is connected.
		MakeArrayNode->PinConnectionListChanged(ArrayOut);

		// connect all arg pin to Make Array input
		for (int32 i = 0; i < ArgPinNames.Num(); i++) {

			// Make Array node has one input by default
			if (i > 0)
				MakeArrayNode->AddInputPin();

			// find the input pin on the "Make Array" node by index.
			const FString PinName = FString::Printf(TEXT("[%d]"), i);
			UEdGraphPin* ArrayInputPin = MakeArrayNode->FindPinChecked(PinName);

			// move input word to array 
			UEdGraphPin* MyInputPin = FindPinChecked(ArgPinNames[i], EGPD_Input);
			CompilerContext.MovePinLinksToIntermediate(*MyInputPin, *ArrayInputPin);
		}// end of for
	}

	// break any links to the expanded node
	BreakAllNodeLinks();
}

UEdGraphPin* UHello::GetThenPin() const
{
	UEdGraphPin* Pin = FindPin(UEdGraphSchema_K2::PN_Then);
	check(Pin == nullptr || Pin->Direction == EGPD_Output); // If pin exists, it must be output
	return Pin;
}

TSharedPtr<SGraphNode> UHello::CreateVisualWidget() {
	return SNew(SGraphNodeSaySomething, this);
}

void UHello::AddPinToNode()
{
	Modify();

	TMap<FString, FStringFormatArg> FormatArgs = {
			{TEXT("Count"), ArgPinNames.Num()}
	};

	FName NewPinName(*FString::Format(TEXT("Word {Count}"), FormatArgs));
	ArgPinNames.Add(NewPinName);

	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, NewPinName);
}

void UHello::RemoveInputPin(UEdGraphPin* Pin)
{
	FScopedTransaction Transaction(FText::FromString("SaySomething_RemoveInputPin"));
	Modify();

	ArgPinNames.Remove(Pin->GetFName());

	RemovePin(Pin);
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}