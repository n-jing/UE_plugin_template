// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloWorldEditor.h"
#include "BlueprintNodeSpawner.h"	// BlueprintGraph
#include "BlueprintActionDatabaseRegistrar.h"	// BlueprintGraph
#include "EdGraphSchema_K2.h"	// BlueprintGraph
#include "K2Node_CallFunction.h"	// BlueprintGraph

#include "HelloWorld.h"	// MyBlueprintNode


void UHelloWorldEditor::AllocateDefaultPins() {

	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
}

void UHelloWorldEditor::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const {
	UClass* ActionKey = GetClass();

	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

void UHelloWorldEditor::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) {
	Super::ExpandNode(CompilerContext, SourceGraph);

	UEdGraphPin* ExecPin = GetExecPin();
	UEdGraphPin* ThenPin = GetThenPin();
	if (ExecPin && ThenPin) {

		// create a CallFunction node
		FName MyFunctionName = GET_FUNCTION_NAME_CHECKED(UHelloWorld, SayHello_Internal);

		UK2Node_CallFunction* CallFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		CallFuncNode->FunctionReference.SetExternalMember(MyFunctionName, UHelloWorld::StaticClass());
		CallFuncNode->AllocateDefaultPins();

		// move pins
		CompilerContext.MovePinLinksToIntermediate(*ExecPin, *(CallFuncNode->GetExecPin()));
		CompilerContext.MovePinLinksToIntermediate(*ThenPin, *(CallFuncNode->GetThenPin()));
	}

	// break any links to the expanded node
	BreakAllNodeLinks();
}

UEdGraphPin* UHelloWorldEditor::GetThenPin() const
{
	UEdGraphPin* Pin = FindPin(UEdGraphSchema_K2::PN_Then);
	check(Pin == nullptr || Pin->Direction == EGPD_Output); // If pin exists, it must be output
	return Pin;
}