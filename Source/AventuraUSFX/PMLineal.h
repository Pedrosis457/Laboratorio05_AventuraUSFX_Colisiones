// LABORATORIO 04 - Movimiento 1: vaiven en linea recta horizontal.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PMLineal.generated.h"

/** Va y viene sobre una recta horizontal, como un ascensor acostado. */
UCLASS()
class AVENTURAUSFX_API APMLineal : public APlataforma
{
	GENERATED_BODY()

public:
	APMLineal();

protected:
	virtual void Mover(float DeltaTime) override;
};
