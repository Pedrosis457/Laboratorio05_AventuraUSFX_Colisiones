// LABORATORIO 04 - Movimiento 4: recorrido en forma de ocho (lemniscata).

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PMOcho.generated.h"

/** Dibuja un ocho horizontal alrededor de su ancla. */
UCLASS()
class AVENTURAUSFX_API APMOcho : public APlataforma
{
	GENERATED_BODY()

public:
	APMOcho();

protected:
	virtual void Mover(float DeltaTime) override;
};
