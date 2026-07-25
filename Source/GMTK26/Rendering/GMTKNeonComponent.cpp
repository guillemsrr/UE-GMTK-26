// Copyright (c) Noümen Interactive. All Rights Reserved.

#include "GMTKNeonComponent.h"

#include "Components/MeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	const FName NeonColorParameter(TEXT("NeonColor"));
	const FName EmissiveStrengthParameter(TEXT("EmissiveStrength"));
}

UGMTKNeonComponent::UGMTKNeonComponent()
{
	IntensityUnits = ELightUnits::Lumens;
	Intensity = 2400.0f;
	AttenuationRadius = 600.0f;
	CastShadows = true; //TODO: false for low-end PC? test Megalights?

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> NeonMaterialAsset(
		TEXT("/Game/Visuals/M_Neon.M_Neon"));
	NeonMaterial = NeonMaterialAsset.Object;
}

UMaterialInstanceDynamic* UGMTKNeonComponent::ApplyTo(UMeshComponent* Mesh)
{
	SetLightColor(NeonColor);

	if (!MaterialInstance)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create(NeonMaterial, this);
		MaterialInstance->SetVectorParameterValue(NeonColorParameter, NeonColor);
		MaterialInstance->SetScalarParameterValue(EmissiveStrengthParameter, EmissiveStrength);
	}

	const int32 MaterialCount = Mesh->GetNumMaterials();
	for (int32 MaterialIndex = 0; MaterialIndex < MaterialCount; ++MaterialIndex)
	{
		Mesh->SetMaterial(MaterialIndex, MaterialInstance);
	}

	return MaterialInstance;
}

void UGMTKNeonComponent::SetNeonColor(const FLinearColor& NewColor)
{
	NeonColor = NewColor;
	SetLightColor(NewColor);

	if (MaterialInstance)
	{
		MaterialInstance->SetVectorParameterValue(NeonColorParameter, NewColor);
	}
}

void UGMTKNeonComponent::SetEmissiveStrength(float NewStrength)
{
	EmissiveStrength = NewStrength;

	if (MaterialInstance)
	{
		MaterialInstance->SetScalarParameterValue(EmissiveStrengthParameter, NewStrength);
	}
}
