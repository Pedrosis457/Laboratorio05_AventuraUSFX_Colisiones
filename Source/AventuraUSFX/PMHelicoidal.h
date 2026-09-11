// LABORATORIO 04 - Movimiento 10: helice, sube girando como un tornillo.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PMHelicoidal.generated.h"

/** Combina la orbita circular con un ascenso: sube y baja girando. */
UCLASS()
class AVENTURAUSFX_API APMHelicoidal : public APlataforma
{
	GENERATED_BODY()

public:
	APMHelicoidal();

protected:
	virtual void Mover(float DeltaTime) override;

	virtual void SortearParametros() override;

	/** Altura total que recorre la helice. */
	UPROPERTY(EditAnywhere, Category = "Plataforma|Helicoidal")
	float AlturaDeHelice;

	UPROPERTY(EditAnywhere, Category = "Plataforma|Helicoidal")
	float FrecuenciaVertical;
};
