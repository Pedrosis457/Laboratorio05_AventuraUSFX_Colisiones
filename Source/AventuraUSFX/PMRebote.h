// LABORATORIO 04 - Movimiento 11: rebote con gravedad, como una pelota.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PMRebote.generated.h"

/**
 * No usa una formula cerrada: integra la gravedad en cada Tick y rebota al
 * tocar la altura del ancla. Es el unico movimiento con estado acumulado.
 */
UCLASS()
class AVENTURAUSFX_API APMRebote : public APlataforma
{
	GENERATED_BODY()

public:
	APMRebote();

protected:
	virtual void Mover(float DeltaTime) override;

	virtual void SortearParametros() override;

	UPROPERTY(EditAnywhere, Category = "Plataforma|Rebote")
	float Gravedad;

	UPROPERTY(EditAnywhere, Category = "Plataforma|Rebote")
	float ImpulsoDeRebote;

	UPROPERTY(VisibleAnywhere, Category = "Plataforma|Rebote")
	float AlturaActual;

	UPROPERTY(VisibleAnywhere, Category = "Plataforma|Rebote")
	float VelocidadVertical;
};
