// Fill out your copyright notice in the Description page of Project Settings.


#include "CActionData.h"
#include "CAction.h"
#include "CNPCAction.h"
#include "Global.h"

void UCActionData::BeginPlay(UCActionBase* InOwnerAction, TArray<FActionData>& OutActionDatas)
{
	for (int32 i = 0; i < ActionDatas.Num(); i++)
	{
		OutActionDatas.Add(ActionDatas[i]);
	}
}