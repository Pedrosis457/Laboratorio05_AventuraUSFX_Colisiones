// LABORATORIO 04 - Movimiento 7: gira sobre si misma sin desplazarse.

#include "PMRotatoria.h"
#include "Components/StaticMeshComponent.h"

APMRotatoria::APMRotatoria()
{
	Tipo = TEXT("PMRotatoria");
	Velocidad = 120.0f;          // grados por segundo de giro
	VelocidadDeCabeceo = 35.0f;  // grados por segundo de cabeceo

	ConfigurarApariencia(FVector(2.80f, 0.90f, 0.35f), FLinearColor(0.85f, 0.85f, 0.90f));
}

void APMRotatoria::Mover(float DeltaTime)
{
	// Rotacion acumulada: aqui el movimiento no es traslacion sino giro.
	AddActorLocalRotation(FRotator(0.0f, Velocidad * Sentido * DeltaTime, 0.0f));

	// El cabeceo se aplica a la malla, para que no interfiera con el giro.
	if (Malla)
	{
		Malla->AddLocalRotation(FRotator(VelocidadDeCabeceo * DeltaTime, 0.0f, 0.0f));
	}
}
