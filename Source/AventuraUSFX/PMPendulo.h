// LABORATORIO 04 - Movimiento 6: pendulo colgado de un punto alto.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PMPendulo.generated.h"

/** Cuelga de un pivote imaginario y oscila de un lado a otro. */
UCLASS()
class AVENTURAUSFX_API APMPendulo : public APlataforma
{
	GENERATED_BODY()

public:
	APMPendulo();

protected:
	virtual void Mover(float DeltaTime) override;

	virtual void SortearParametros() override;

	/** Largo de la cuerda imaginaria, en centimetros. */
	UPROPERTY(EditAnywhere, Category = "Plataforma|Pendulo")
	float LongitudDeCuerda;

	/** Apertura maxima de la oscilacion, en grados. */
	UPROPERTY(EditAnywhere, Category = "Plataforma|Pendulo")
	float AnguloMaximo;
};
