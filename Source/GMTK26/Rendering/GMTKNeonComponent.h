// Copyright (c) Noümen Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/PointLightComponent.h"
#include "GMTKNeonComponent.generated.h"

class UMaterialInstanceDynamic;
class UMaterialInterface;
class UMeshComponent;

UCLASS()
class GMTK26_API UGMTKNeonComponent : public UPointLightComponent
{
	GENERATED_BODY()

protected:
	UGMTKNeonComponent();

public:
	UMaterialInstanceDynamic* ApplyTo(UMeshComponent* Mesh);
	void SetNeonColor(const FLinearColor& NewColor);
	void SetEmissiveStrength(float NewStrength);

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> NeonMaterial;

	UPROPERTY(EditAnywhere)
	FLinearColor NeonColor = FLinearColor(0.02f, 0.65f, 1.0f);

	UPROPERTY(EditAnywhere)
	float EmissiveStrength = 7.0f;

private:
	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;
};
