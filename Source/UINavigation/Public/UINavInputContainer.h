﻿// Copyright (C) 2019 Gonçalo Marques - All Rights Reserved

#pragma once

#include "Data/AxisType.h"
#include "Data/InputCollisionData.h"
#include "Data/InputRebindData.h"
#include "Data/InputRestriction.h"
#include "Data/RevertRebindReason.h"
#include "Data/TargetColumn.h"
#include "Blueprint/UserWidget.h"
#include "Data/InputContainerEnhancedActionData.h"
#include "EnhancedActionKeyMapping.h"
#include "UINavInputContainer.generated.h"

/**
* This class contains the logic for aggregating several input boxes
*/

UCLASS()
class UINAVIGATION_API UUINavInputContainer : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	void SetupInputBoxes();
	void CreateInputBoxes();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "UINav Input")
	class UPanelWidget* InputBoxesPanel = nullptr;

	class UUINavWidget* ParentWidget = nullptr;

public:

	virtual void NativeConstruct() override;

	/**
	*	Called when a new input box is added
	*/
	UFUNCTION(BlueprintNativeEvent, Category = UINavWidget)
	void OnAddInputBox(class UUINavInputBox* NewInputBox);

	virtual void OnAddInputBox_Implementation(class UUINavInputBox* NewInputBox);

	/*
	*	Called when key was successfully rebinded
	*/
	UFUNCTION(BlueprintNativeEvent, Category = UINavWidget)
	void OnKeyRebinded(FName InputName, FKey OldKey, FKey NewKey);

	virtual void OnKeyRebinded_Implementation(FName InputName, FKey OldKey, FKey NewKey);

	/*
	*	Called when a rebind was cancelled, specifying the reason for the revert
	*/
	UFUNCTION(BlueprintNativeEvent, Category = UINavWidget)
	void OnRebindCancelled(ERevertRebindReason RevertReason, FKey PressedKey);

	virtual void OnRebindCancelled_Implementation(ERevertRebindReason RevertReason, FKey PressedKey);

	/**
	*	Called when the player presses a key being used by another action
	*/
	bool RequestKeySwap(const FInputCollisionData& InputCollisionData, const int CurrentInputIndex, const int CollidingInputIndex) const;

	UFUNCTION(BlueprintCallable, Category = "UINav Input")
	void ResetKeyMappings();

	ERevertRebindReason CanRegisterKey(class UUINavInputBox* InputBox, const FKey NewKey, const int Index, int& OutCollidingActionIndex, int& OutCollidingKeyIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UINav Input")
	bool CanUseKey(class UUINavInputBox* InputBox, const FKey CompareKey, int& OutCollidingActionIndex, int& OutCollidingKeyIndex) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UINav Input")
	bool RespectsRestriction(const FKey CompareKey, const int Index);

	void ResetInputBox(const FName InputName, const EAxisType AxisType);

	UUINavInputBox* GetInputBoxInDirection(UUINavInputBox* InputBox, const EUINavigation Direction);
	
	UUINavInputBox* GetOppositeInputBox(const FInputContainerEnhancedActionData& ActionData);
	UUINavInputBox* GetOppositeInputBox(const FName& InputName, const EAxisType AxisType);

	void GetAxisPropertiesFromMapping(const FEnhancedActionKeyMapping& ActionMapping, bool& bOutPositive, EInputAxis& OutAxis) const;

	//Fetches the index offset from the TargetColumn variable for both the top and bottom of the Input Container
	int GetOffsetFromTargetColumn(const bool bTop) const;

	void GetInputRebindData(const int InputIndex, FInputRebindData& RebindData) const;

	void GetEnhancedInputRebindData(const int InputIndex, FInputRebindData& RebindData) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UINav Input")
	FORCEINLINE ETargetColumn GetTargetColumn() const { return TargetColumn; }

	//-----------------------------------------------------------------------

	class UUINavPCComponent* UINavPC = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "UINav Input")
	int NumberOfInputs = 0;

	UPROPERTY(BlueprintReadOnly, Category = "UINav Input")
	int KeysPerInput = 0;

	UPROPERTY(BlueprintReadOnly, Category = "UINav Input")
	TArray<UUINavInputBox*> InputBoxes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UINav Input")
	TMap<UInputMappingContext*, FInputContainerEnhancedActionDataArray> EnhancedInputs;
	
	/*
	The restrictions for the type of input associated with each column
	in the Input Container
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UINav Input")
	TArray<EInputRestriction> InputRestrictions;

	/*
	A list of the keys that the player should only be able to use for the inputs
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UINav Input")
	TArray<FKey> KeyWhitelist;
	
	/*
	A list of the keys that the player shouldn't be able to use for the inputs.
	Only used if KeyWhitelist is empty.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UINav Input")
	TArray<FKey> KeyBlacklist =
	{
		EKeys::Escape,
		EKeys::LeftCommand,
		EKeys::RightCommand,
	};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UINav Input")
	TSubclassOf<class UUINavInputBox> InputBox_BP;

	/*
	The widget class of the widget that will tell the player that 2 keys can be swapped.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UINav Input")
	TSubclassOf<class USwapKeysWidget> SwapKeysWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UINav Input")
	int SpawnKeysWidgetZOrder = 0;
	
	/*
	Indicates whether unused input boxes will hidden or collapsed
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UINav Input")
	bool bCollapseInputBoxes = false;

	//Indicates which column to navigate to when navigating to this Input Container
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UINav Input")
	ETargetColumn TargetColumn = ETargetColumn::Left;

	//The text used for empty key buttons
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UINav Input")
	FText EmptyKeyText = FText::FromString(TEXT("Unbound"));

	//The text used for notifying the player to press a key
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UINav Input")
	FText PressKeyText = FText::FromString(TEXT("Press Any Key"));

};
